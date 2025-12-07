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
    
    // Track the total color calculated and and the number of samples taken.
    float4 total = 0;
    int sampleCount = 0;
    
    // Loop through the pixel "boxes" of the given pixel.
    for (int x = -blurRadius; x <= blurRadius; x++)
    {
        for (int y = -blurRadius; y <= blurRadius; y++)
        {
            // Calculate the UV texture coordinate for the color of this sample.
            float2 uv = input.uv;
            uv += float2(x * pixelWidth, y * pixelHeight);
            
            // Use this new uv to get the color of the calculated uv sample.
            total += Pixels.Sample(ClampSampler, uv);
            
            // Increase the sample count.
            sampleCount++;
        }
    }
	
    // Return the color of the pixel which is the average 
    // of the total color and the sample count.
    return total / sampleCount;
    
	// Return the color to test.
    //return pixelColor;
}