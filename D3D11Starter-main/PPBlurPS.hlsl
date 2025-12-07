// use the shader include file.
#include "ShaderIncludeFile.hlsli"

// Create a CBH for the blur pixel information.
cbuffer ExternalData : register(b0)
{
    int blurRadius;         // The neighbouring pixel directional range.
    float pixelWidth;       // The window width.
    float pixelHeight;      // The window height
}

// Add the PP Pixel Texture and the PP Sampler.
Texture2D Pixels : register(t0);
SamplerState ClampSampler : register(s0);

float4 main(VertexToPixelForPP input) : SV_TARGET
{
	// Get the sample of the texture for the pixel.
    float4 pixelColor = Pixels.Sample(ClampSampler, input.uv);
	
	// Return the color to test.
	return pixelColor;
    //return float4(1, 1, 1, 0);
}