// Using shader include file:
#include "ShaderIncludeFile.hlsli"

cbuffer ExternalVSData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}

float4 main( VertexShaderInput input ) : SV_POSITION
{
    // Get the wvp matrix of the light.
    matrix wvp = mul(projection, mul(view, world));
    
    // Change and return the new vertex position of the object
    // based on the light wvp.
    return mul(wvp, float4(input.localPosition, 1.0f));
}