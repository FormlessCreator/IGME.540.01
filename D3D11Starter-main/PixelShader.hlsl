// Add the include shader file here.
#include "ShaderIncludeFile.hlsli"

// Add a lights header.
//#include "Lights.h"

// Store the refrences to the texture and the sampler.
// Create a 2D texture surface data.
Texture2D PavementSurfaceTexture : register(t0);
Texture2D SolarCellSurfaceTexture : register(t1);
Texture2D NormalMap : register(t2);

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
	// Enter the light count.
    int lightCount = 5;
	
	// Normalize the input tangent.
    input.tangent = normalize(input.tangent);
	
	// Get the sample of the normal map texture.
    float3 normalFromTexture = NormalMap.Sample(BasicSampler, input.uv).rgb;
	
	// Unpack the per pixel normal from the texture sample.
    float3 unpackNormal = normalize(normalFromTexture * 2.0f - 1.0f);
	
	// Get the ambient color or average surface color for all the lights.
	// Normalize the input normal.
	// To create an non-linear interpolation of the pixel vertex normal.
    input.normal = normalize(input.normal);
	
	// Create a 90* Tangent, Bitangent and Normal matrix.
    float3 N = normalize(input.normal);
	
	// Get the tangent.
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
	
	// Get the 90 degree Bi-tangent using the cross product of T & N.
    float3 B = normalize(cross(T, N));
	
	// Create a 3x3 float matrix that creates a 3x3 world space of T = x or u, B = y or v, N = z.
    float3x3 TBN = float3x3(T, B, N);
	
	// Transform the normal of the unpacked texture to the TBN coordinates.
    float3 finalNormal = mul(unpackNormal, TBN);
	
	// Get the new input scale and offset.
	// Create a modified input uv using the new input scale and offset.
    input.uv = input.uv * scale + offset;
	
	// Create and get a texture color from the texture using the texture,
	// the sampler state and the given input uv coordinate.
    float3 surfaceColor = PavementSurfaceTexture.Sample(BasicSampler, input.uv).rgb;
	
	
	// Make the ambient color darker.
	// Create an CBH value to make ambient color darker or brighter.
    float4 darkerAmbientColor = float4(ambientColor.xyz / 1.0f, 1.0f);
	
	// AMBIENT LIGHT:
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
    // surfaceColor1 = ambientColor.xyz * surfaceColor1 * colorTint.xyz;
	// This is the surface color of the object for each individual light.
    surfaceColor = darkerAmbientColor.xyz * surfaceColor * colorTint.xyz;
	
	// Create a total lights final color that is the the ambient color of all
	// the light, the surface color and thier tint.
	// It starts with the ambient surface color then it adds up all other
	// light type calculation.
    float3 totalLight = darkerAmbientColor.xyz * surfaceColor * colorTint.xyz;
	
	// ------------------------------------------------------------------------------
	// Lighting Equations:
	
	// Create a for loop that gets the light in an array, does some calculation
	// based on its type using a switch statement and adds the light color to
	// the total light color combination for the pixel.
    for (int i = 0; i < 5; i++)
    {
		// Get the light in the current loop.
        Lights light = lightsArray[i];
		
		// Get the normalised light direction.
        float3 normalizedLightDirection = normalize(-light.direction);
		
		// Create a switch statement that gets the current light type.
        switch (light.type)
        {
		// If the light is a directional light, calculate light on the pixel surface
		// and add to all the lights in the scene.
            case 0:
                totalLight +=
			DirectionalLight(
			light,
			finalNormal,
			normalizedLightDirection,
			input.worldPosition,
			cameraCurrentPosition,
			roughness.x,
			surfaceColor,
			MAX_SPECULAR_EXPONENT);
                break;
		
            case 1:
			// If the light is a point light, calculate light on the pixel surface
			// and add to all the lights in the scene.
                totalLight +=
			PointLight(
			light,
			finalNormal,
			input.worldPosition,
			cameraCurrentPosition,
			roughness.x,
			surfaceColor,
			MAX_SPECULAR_EXPONENT) * Attenuate(light, input.worldPosition);
                break;
		
            case 2:
			// If the light is a spot light, calculate light on the pixel surface
			// and add to all the lights in the scene.
                totalLight +=
            SpotLight(
			light,
			finalNormal,
			normalizedLightDirection,
			input.worldPosition,
			cameraCurrentPosition,
			roughness.y,
			surfaceColor,
			MAX_SPECULAR_EXPONENT) * Attenuate(light, input.worldPosition);
                break;
        }
    }
	
	// Issues:
	// Here the final surface color is just the diffuse surface color and 
	// ambient color(for the unlit surface, no surface color texture).
	//float3 finalColor = ambientColor.xyz / 0.6f + diffuseTermColor;
    //float3 finalColor = darkerAmbientColor.xyz + diffuseTermColor;
	
	
	// ------------------------------------------------------------------------------
	
	// TOTAL LIGHT RETURN:
	// Return a float4 color.
    return float4(totalLight, 1.0f);
	
	// Test:
	// return float4(finalColor.xyz, 1.0f);
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