// Add a shader include file.
#include "ShaderIncludeFile.hlsli"

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
    float2 scale;
    float2 offset;
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
	// Modify the input uv texture by its material texture scale and offset.
    input.uv = input.uv * scale * offset;
	
	// Create and get a texture color from the texture using the texture,
	// the sampler state and the given input uv coordinate.
    float3 surfaceColor1 = PavementSurfaceTexture.Sample(BasicSampler, input.uv).rgb;
    float3 surfaceColor2 = SolarCellSurfaceTexture.Sample(BasicSampler, input.uv).rgb;
	
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	// Get the combination of both texture for the texture tint.
    // float3 surfaceCombination = surfaceColor1 * surfaceColor2;
	
	// Combine both texture together by adding.
    float3 surfaceCombination = surfaceColor1 + surfaceColor2;
	
	// Make the color brighter by adding new surface texture combination 
	// again (+ 1 brightness).
    surfaceCombination += surfaceCombination;
	
	// Make the color less bright(its intial combination brightness) or
	// or darker by subtracting.
    // surfaceCombination -= surfaceCombination;
	
	// Combine the surface with the color tint.
    surfaceCombination *= colorTint.xyz;
	
	// Return a float4 color.
    return float4(surfaceCombination, 1.0f);
	
	// Test:
	//return float4(input.uv, 0, 1);
    //return float4(input.normal, 1);
}