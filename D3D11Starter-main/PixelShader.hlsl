// Add the include shader file here.
#include "ShaderIncludeFile.hlsli"

// Add a lights header.
//#include "Lights.h"

// Store the refrences to the texture and the sampler.
// Create a 2D texture surface data.
//Texture2D PavementSurfaceTexture : register(t0);
//Texture2D SolarCellSurfaceTexture : register(t1);
//Texture2D NormalMap : register(t2);

// Add the new textures to the pixel shader.
Texture2D Albedo : register(t0);
Texture2D MetalnessMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D ShadowMap : register(t4);

// Create a sampler state.
SamplerState BasicSampler : register(s0);

// Add a shadow sampler.
SamplerComparisonState ShadowSampler : register(s1);

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

	// Check the shadow map.
	// Perform a perspective divide our self.
    input.shadowMapPos /= input.shadowMapPos.w;
	
	// Convert the normalized cordinate of the perspective divide to UV's
	// for sampling (unpack).
    float2 shadowUV = input.shadowMapPos.xy * 0.5f + 0.5f;
	
	// Flip the y of the shadow UV.
    shadowUV.y = 1.0f - shadowUV.y;
	
	// Get the distance from the light to the pixel and the closet surface.
    float distanceOfLightToPixel = input.shadowMapPos.z;
	float distanceOfShadowMapFloor = ShadowMap.Sample(BasicSampler, shadowUV).r;
	
	// Use the shadow amount for the shadow map sample and comparison.
    float shadowAmount = ShadowMap.SampleCmpLevelZero(
	   ShadowSampler, shadowUV, distanceOfLightToPixel).r;
	
	// If the light to the pixel is less than the distance to the surface floor,
	// then there are shadows to display.
	// Use tenary operator for fast if statements.
    float d = distanceOfLightToPixel < distanceOfShadowMapFloor ? 1.0f : 0.0f;

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
	
	// Get the perpendicular 90 degree Bi-tangent using the cross product of T & N.
    float3 B = normalize(cross(T, N));
	
	// Create a 3x3 float matrix that creates a 3x3 world space of T = x or u, B = y or v, N = z.
    float3x3 TBN = float3x3(T, B, N);
	
	// Transform the normal of the unpacked texture to the TBN coordinates.
    float3 finalNormal = mul(unpackNormal, TBN);
	
	// Get the new input scale and offset.
	// Create a modified input uv using the new input scale and offset.
    input.uv = input.uv * scale + offset;
	
	// Get the roughness map sample and the metalness map sample.
    float roughnessTexture = RoughnessMap.Sample(BasicSampler, input.uv).r;
    float metalnessTexture = MetalnessMap.Sample(BasicSampler, input.uv).r;
	
	// Gamma correct the rougness and metal texture.
    //roughnessTexture = pow(roughnessTexture, 2.2f);
    //metalnessTexture = pow(metalnessTexture, 2.2f);
	
	// Create and get a texture color from the texture using the texture,
	// the sampler state and the given input uv coordinate.
    float3 surfaceColor = Albedo.Sample(BasicSampler, input.uv).rgb;
	
	// Gamma correct the albedo surface texture.
    surfaceColor = pow(surfaceColor, 2.2f);
	
	// No ambient light.
    surfaceColor = surfaceColor * colorTint.xyz;
	
	// Create a specular reflection for the albedo material texture color.
	// Specular color is the color of light reflected of the surface of a
	// material:
	// If it is dielectric the albedo specular color is often between the 
	// range of constant 0.04f or 0 as they do not tint their reflected light.
	// Metal often have a specular color of 1 as it reflect more tinted 
	// light of its surface albedo color.
	// We determine this based on the metalness of the pixel.
    float3 specularColor = lerp(0.04f, surfaceColor.rgb, metalnessTexture);
	
	// No ambient light.
    float3 totalLight = float3(0.0f, 0.0f, 0.0f);
	
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
                float3 directionalLightResult =
			CookDirectionalLight(
			light,
			finalNormal,
			normalizedLightDirection,
			input.worldPosition,
			cameraCurrentPosition,
			roughnessTexture,
			surfaceColor,
			specularColor,
			metalnessTexture); //* d;
			
			// If this is the light(light 3) needed for shadowing apply the shadow result to only
			// this light.
			if(i == 3)
            {
				// Multiply the light calculations by the sampled shadow amount.
                directionalLightResult *= d;
            }
			
			// This shadow map will only affect the third light.
                totalLight += directionalLightResult;
                break;
		
            case 1:
			// If the light is a point light, calculate light on the pixel surface
			// and add to all the lights in the scene.
                totalLight +=
			CookPointLight(
			light,
			finalNormal,
			input.worldPosition,
			cameraCurrentPosition,
			roughnessTexture,
			surfaceColor,
			specularColor,
			metalnessTexture) * Attenuate(light, input.worldPosition);
                break;
		
            case 2:
			// If the light is a spot light, calculate light on the pixel surface
			// and add to all the lights in the scene.
                totalLight +=
            CookSpotLight(
			light,
			finalNormal,
			normalizedLightDirection,
			input.worldPosition,
			cameraCurrentPosition,
			roughnessTexture,
			surfaceColor,
			specularColor,
			metalnessTexture) * Attenuate(light, input.worldPosition);
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
	// Adjust the total light for gamma correction.
    float3 gammaAdjustedColor = pow(totalLight, 1.0f / 2.2f);
	
	// Return a float4 color.
    //return float4(gammaAdjustedColor, 1.0f);
    return float4(shadowAmount.rrr, 1.0f);
    //return float4(distanceOfShadowMapFloor.xxx, 1.0f);
	
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
