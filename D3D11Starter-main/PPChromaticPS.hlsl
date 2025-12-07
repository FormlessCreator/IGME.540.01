// use the shader include file.
#include "ShaderIncludeFile.hlsli"

// Add the PP Pixel Texture and the PP Sampler.
Texture2D Pixels : register(t0);
SamplerState ClampSampler : register(s0);

float4 main(VertexToPixelForPP input) : SV_TARGET
{
	// Get the sample of the texture for the pixel.
    //float4 pixelColor = Pixels.Sample(ClampSampler, input.uv);
	
	// Create and RGB color offset.
    float3 colorOffset = float3(0.009f, 0.006f, -0.006f);
	
	// Create a coordinate at the middle of the screen.
    float2 positionXY = float2(0.0f, 0.0f);
	
	// Create a texture color;
    float2 textureSize;
	
	// Get the width and height of the texture.
    Pixels.GetDimensions(textureSize.x, textureSize.y);
	
	// Get the texture coordinate.
    float2 textureCoord = input.position.xy / textureSize;
	
	// Get a new direction.
    float2 direction = textureCoord - positionXY;
	
	// Create a color sample.
    float4 aberrationColor;
	
	// Add aberation color to the color.
    aberrationColor.r = Pixels.Sample(ClampSampler, textureCoord + (direction * colorOffset.r)).r;
    aberrationColor.g = Pixels.Sample(ClampSampler, textureCoord + (direction * colorOffset.g)).g;
    aberrationColor.b = Pixels.Sample(ClampSampler, textureCoord + (direction * colorOffset.b)).b;
    aberrationColor.a = 0.1f;
	
	// Return pixel aberration sample.
    return aberrationColor;
	
	// Return the color to test.
    //return float4(1, 0, 0, 1);
}