// use the shader include file.
#include "ShaderIncludeFile.hlsli"

// Add the PP Pixel Texture and the PP Sampler.
Texture2D Pixels : register(t0);
SamplerState ClampSampler : register(s0);

float4 main(VertexToPixelForPP input) : SV_TARGET
{
	// Get the sample of the texture for the pixel.
    float4 pixelColor = Pixels.Sample(ClampSampler, input.uv);
	
	// Return the color to test.
    return pixelColor;
}