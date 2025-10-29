// Add a shader include file.
#include "ShaderIncludeFile.hlsli"

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
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
    // With some reference Kishimisu's tutorial: https://www.youtube.com/watch?v=f4s1h2YETNY
    // I added my own custom code on some of the tutorial code by adding additional code 
    // that outputs color using randomness, color changes, distance, sine, absolute value,
    // and other calculations.
    float randomFloat = random(float2(input.normal.x, input.normal.z));
    float magnitudeOfUv = distance(input.uv.x, input.uv.y);
    float4 output = ColorChange(magnitudeOfUv, time.x * 2.0f);
    magnitudeOfUv = sin(magnitudeOfUv * 8.0 + time.x + randomFloat) / 8.06;
    magnitudeOfUv =  abs(magnitudeOfUv) + frac((sin(randomFloat / 250.0f)));
    magnitudeOfUv = 0.03 / magnitudeOfUv;
    output *= magnitudeOfUv + colorTint;
	
    // Return the 
    return output;
	
	// Test:
	//return float4(input.uv, 0, 1);
    //return float4(input.normal, 1);
}