// Include the shader include file.
#include "ShaderIncludeFile.hlsli"

// Get the sky box texture cube and sampler state.
TextureCube SkyTexture : register(t0);
SamplerState BasicSampler : register(s0);

// Add the Vertex to pixel for the sky box input.
float4 main( VertexToPixel_SkyBox input) : SV_TARGET
{
	// Return the pixel color texture of the sampled sky texture cube direction.
    return SkyTexture.Sample(BasicSampler, input.sampleDir);
}