// Define the unique identifer for the shader include file.
// Each .hlsli needs a unique identifier with a safe guard definer.
#ifndef _GGP_SHADER_INCLUDES_FILE_
#define _GGP_SHADER_INCLUDES_FILE_

// Add the directX 11 header files.
//#include <d3d11.h>
//#include <DirectXMath.h>

// Define 5 major light types for the light structs.
#define LIGHT_TYPE_DIRECTIONAL		0
#define LIGHT_TYPE_POINT			1
#define LIGHT_TYPE_SPOT				2

// Create a define for the maximum specular exponent.
#define MAX_SPECULAR_EXPONENT 250.0f

struct Lights
{
    int type; // Which kind of light? 0, 1 or 2 (see above)
    float3 direction; // Directional and Spot lights need a direction
    float range; // Point and Spot lights have a max range for attenuation
    float3 position; // Point and Spot lights have a position in space
    float intensity; // All lights need an intensity
    float3 color; // All lights need a color
    float spotInnerAngle; // Inner cone angle (in radians) – Inside this, full light!
    float spotOuterAngle; // Outer cone angle (radians) – Outside this, no light!
    float2 padding; // Purposefully padding to hit the 16-byte boundary.
};

// Create an attentuate method for point and spot light so that light
// lessens with range and does not keep traveling infinetely.
// Using the light range and the world position of the pixel in contact
// with the light.
float Attenuate(Lights light, float3 worldPosition)
{
	// Get the distance of light to the input pixel world surface position.
    float dist = distance(light.position, worldPosition);
	
	// Get the diminishing light by distance by attenuating.
	// If distance to the light & surface is greater than light range.
    float attenuate = saturate(1.0f - (dist * dist / (light.range * light.range)));
	
	// Mutiply the attenuate value by itself to cut of infinite distance and
	// reduce brightness exponetially to 0 over distance.
    return attenuate * attenuate;
}

// Include and move all structs from the vertex and pixel shaders here:

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float3 localPosition : POSITION; // XYZ position
    float2 uv : TEXCOORD; // The uv texture coordinate.
    float3 normal : NORMAL; // The normal direction of the vertex.
	
	// Add a vertex to input struct for the tangent.
    float3 tangent : TANGENT;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
	
	// Add the world position of the pixel.
    float3 worldPosition : POSITION;
	
	// Add the the tangent to the output for pixel shader.
    float3 tangent : TANGENT;
};

struct VertexToPixel_SkyBox
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition : SV_POSITION;
    float4 sampleDir : DIRECTION;
};

// Create a directional light method.
float3 DirectionalLight(
Lights light,
float3 inputNormal,
float3 normalizedLightDirection,
float3 inputPixelWorldPosition,
float4 cameraPosition,
float roughness,
float3 surfaceColor,
float maxSpecularReflection)
{
	// DIFFUSE LIGHT:
	// Calculate the overall diffuse color of light 1.
	// Get the dot product of the light:
	// Using the and the normalize input pixel normal and normalize -direction of the light.
	// Using saturate or max or clamp to make so that the light does not fall beyond 0.
	// Does the pixel normal direction point to the same direction as the light?
	// How much light is shined on the given pixels?
    float Ndot1 = saturate(dot(inputNormal, -normalizedLightDirection));
	
	// Create a diffuse term color by combining the normalize dot light with:
	// The light intensity
	// The light color
	// The surface color.
    float3 diffuseTermColor = Ndot1 * light.intensity * light.color * surfaceColor;
	
	// SPECULAR LIGHT:
	// USING THE DIRECTIONAL LIGHT:
	
	// Get the view vector or direction of the camera using camera position and the world position of the pixel
	// normal that the light is hitting.
    float3 viewVectorDirOfTheCamera = cameraPosition.xyz - inputPixelWorldPosition;
	
	// Normalize the view vector direction to resolve non-unit direction.
    float3 normalVVDirOfCam = normalize(viewVectorDirOfTheCamera);
	
	// Get the reflection of the light using the normalized light direction and input normal.
    float3 lightReflection = reflect(normalizedLightDirection, inputNormal);
	
	// Get the max specular.
    //float maxSpecularReflection = MAX_SPECULAR_EXPONENT;
	// Create a specular reflection using the dot product of the 
	// light reflection and the view vector camera direction.
    float specularLightReflectToTheCamera = pow(max(dot(lightReflection, normalVVDirOfCam), 0.0f), maxSpecularReflection / roughness);
	
	// If the surfece is facing away from the light make its specular 0.
    specularLightReflectToTheCamera *= any(diffuseTermColor);
	
	// Create a final color by adding the:
	// pixel normal surface color for the unlit part of the object
	// to the diffuse part of the object.
    float3 finalColor = surfaceColor + diffuseTermColor + specularLightReflectToTheCamera;
	
	// Return the final 
    return finalColor;
}

// Create a point light method.
float3 PointLight(
Lights light,
float3 inputNormal,
float3 inputPixelWorldPosition,
float4 cameraPosition,
float roughness,
float3 surfaceColor,
float maxSpecularReflection)
{
	// Point light uses position and direction of the light from the the pixel position.
    float3 dirOfLightPosFromPixelPos = light.position - inputPixelWorldPosition;
	
	// Normalize the light direction.
    float3 normalizedLightDirection = normalize(-dirOfLightPosFromPixelPos);
	
	// Return the directional light color but use the normalized direction of the light
	// and the pixel world surface position.
    return DirectionalLight(
	light, 
	inputNormal, 
	normalizedLightDirection, 
	inputPixelWorldPosition, 
	cameraPosition, 
	roughness, 
	surfaceColor, 
	maxSpecularReflection);
}

// Create a spot light method.
float3 SpotLight(
Lights light,
float3 inputNormal,
float3 normalizedLightDirection,
float3 inputPixelWorldPosition,
float4 cameraPosition,
float roughness,
float3 surfaceColor,
float maxSpecularReflection)
{
	// Get the direction of light from the pixel.
    float3 dirOfLightFromThePixel = inputPixelWorldPosition - light.position;
	
	// Normalize the direction of the position to the light.
    float3 normalizedLightPosDir = normalize(dirOfLightFromThePixel);
	
	// Normalize the direction to the light.
    float3 normalizeLightDir = normalize(normalizedLightDirection);
	
	// Get the normalized dot product(cos angle) of light direction and the spot light direction.
    float pixelAngle = saturate(dot(normalizedLightPosDir, normalizeLightDir));
	
	// Get the cos angle of both inner and outer radian range of the light direction.
    float cosOfOuterRange = cos(light.spotOuterAngle);
    float cosOfInnerrange = cos(light.spotInnerAngle);
	
	// Calculate the falloff(gap) range of both angles.
    float fallOfRange = cosOfOuterRange - cosOfInnerrange;
	
	// Give the light a linear falloff dim starting from the gap of the falloff range.
	// to create a spot light range.
    float linearSpotLightFalloff = saturate((cosOfOuterRange - pixelAngle) / fallOfRange);
	
	// Spot light is just a point light with a falloff range of light.
	// Use the point light method and multiply by the fall offrange.
    float3 finalColor = PointLight(
	light,
	inputNormal,
	inputPixelWorldPosition,
	cameraPosition,
	roughness,
	surfaceColor,
	maxSpecularReflection) * linearSpotLightFalloff;
	
    return finalColor;
}

// Helper functions:

// Create a Color Change class that changes color based on the float value it is provided.
float4 ColorChange(float4 value, float2 time)
{
	// Create four colors.
    float4 color1 = float4(0.95f, 0.45f, 0.65f, 0.0f);
    float4 color2 = float4(1.00f, 0.45f, 0.45f, 0.0f);
    float4 color3 = float4(0.75f, 0.45f, 1.00f, 0.0f);
    float4 color4 = float4(1.00f, 0.40f, 0.75f, 0.0f);
	
	// Return a random value using the color that uses a cos and the value with time.
    float4 x = color1 + color2 * cos(color3 + color4 + value + time.r);
	
	// Return the new color.
    return x;
}

// Using Professor Chris Casioli Helper code.
float random(float2 s)
{
    return frac(sin(dot(s, float2(12.9898, 78.233))) * 43758.5453123);
}

// Create a helper function for normalizing the direction of light.

// Close the if guard defination.
#endif