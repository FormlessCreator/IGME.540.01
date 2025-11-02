// Add the include shader file here.
#include "ShaderIncludeFile.hlsli"

// Add a lights header.
//#include "Lights.h"

// Store the refrences to the texture and the sampler.
// Create a 2D texture surface data.
Texture2D PavementSurfaceTexture : register(t0);
Texture2D SolarCellSurfaceTexture : register(t1);

// Create a sampler state.
SamplerState BasicSampler : register(s0);

// Create a cbuffer struct for the pixel shader.
cbuffer PSExternalData1 : register(b0)
{
    float4 colorTint;
	
    float2 time;
    float2 timePadding;
	
    float2 scale;
    float2 scalePadding;
	
    float2 offset;
    float2 offsetPadding;
	
	// Get the camera position and the entity material object.
    float4 cameraCurrentPosition;
	
    float2 roughness;
    float2 roughnessPadding;
	
    float4 ambientColor;
	
	// Add the light to the pixel shader CBH.
    Lights directionalLight1;
	
	// Add five lights array to match CBH:
    Lights lightsArray[5];
	
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Normalize the input normal.
	// To create an non-linear interpolation of the pixel vertex normal.
    input.normal = normalize(input.normal);
	
	// Get the new input scale and offset.
	// Create a modified input uv using the new input scale and offset.
    input.uv = input.uv * scale + offset;
	
	// Create and get a texture color from the texture using the texture,
	// the sampler state and the given input uv coordinate.
    float3 surfaceColor1 = PavementSurfaceTexture.Sample(BasicSampler, input.uv).rgb;
	
	// Make the ambient color darker.
	// Create an CBH value to make ambient color darker or brighter.
    float4 darkerAmbientColor = float4(ambientColor.xyz / 0.6f, 1.0f);
	
	// AMBIENT LIGHT:
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
    // surfaceColor1 = ambientColor.xyz * surfaceColor1 * colorTint.xyz;
    surfaceColor1 = darkerAmbientColor.xyz * surfaceColor1 * colorTint.xyz;
	
	// ------------------------------------------------------------------------------
	// Lighting Equations:
	
	// Get the direction of the light.
    float3 light1Dir = directionalLight1.direction;
	
	// Check to see if the light array directional light is working.
	// ...
	
	// Get the normalized direction of the light.
    float3 normalizeLight1Dir = normalize(light1Dir);
	
	// Get the pixel world position.
    float3 pixelWorldPosition = input.worldPosition;
	
	// Create a switch statement that gets the current light type.
    switch (directionalLight1.type)
    {
		// If the light is a directional light.
        case 0:
            break;
		
		case 1:
            break;
		
		case 2:
            break;
    }
	
	// DIFFUSE LIGHT:
	// Calculate the overall diffuse color of light 1.
	// Get the dot product of the light:
	// Using the and the normalize input pixel normal and normalize -direction of the light.
	// Using saturate or max or clamp to make so that the light does not fall beyond 0.
	// Does the pixel normal direction point to the same direction as the light?
	// How much light is shined on the given pixels?
    float Ndot1 = saturate(dot(input.normal, -normalizeLight1Dir));
	
	// Create a diffuse term color by combining the normalize dot light with:
	// The light intensity
	// The light color
	// The surface color.
    float3 diffuseTermColor = Ndot1 * directionalLight1.intensity * directionalLight1.color * surfaceColor1;
	
	// SPECULAR LIGHT:
	// USING THE DIRECTIONAL LIGHT:
	
	// Get the view vector or direction of the camera using camera position and the world position of the pixel
	// normal that the light is hitting.
    float3 viewVectorDirOfTheCamera = cameraCurrentPosition.xyz - pixelWorldPosition;
	
	// Normalize the view vector direction to resolve non-unit direction.
    float3 normalVVDirOfCam = normalize(viewVectorDirOfTheCamera);
	
	// Get the reflection of the light using the normalized light direction and input normal.
    float3 lightReflection = reflect(normalizeLight1Dir, input.normal);
	
	// Get the max specular.
    float maxSpecularReflection = MAX_SPECULAR_EXPONENT;
	
	// Create a specular reflection using the dot product of the 
	// light reflection and the view vector camera direction.
    float specularLightReflectToTheCamera = pow(max(dot(lightReflection, normalVVDirOfCam), 0.0f), maxSpecularReflection);
	
	// Create a final color by adding the:
	// pixel normal surface color for the unlit part of the object
	// to the diffuse part of the object.
    float3 finalColor = surfaceColor1 + diffuseTermColor + specularLightReflectToTheCamera;
	
	// Issues:
	// Here the final surface color is just the diffuse surface color and 
	// ambient color(for the unlit surface, no surface color texture).
	//float3 finalColor = ambientColor.xyz / 0.6f + diffuseTermColor;
    //float3 finalColor = darkerAmbientColor.xyz + diffuseTermColor;
	
	
	// ------------------------------------------------------------------------------
	
	// TOTAL LIGHT RETURN:
	// Return a float4 color.
    return float4(finalColor.xyz, 1.0f);
	
	// Test:
	//return float4(input.uv, 0, 1);
    //return float4(input.normal, 1);
    //return float4(directionalLight1.color, 1.0f);
    //return float4(directionalLight1.direction, 1.0f);
	//return float4(surfaceColor1.xyz, 1.0f);
    //return float4(input.worldPosition, 1.0f);
    //return float4(normalVVDirOfCam.xyz, 1.0f);
    //return float4(lightReflection.xyz, 1.0f);
    //return float4(specularLightReflectToTheCamera.xxx, 1.0f);
    //return float4(diffuseTermColor.xyz, 1.0f);
}