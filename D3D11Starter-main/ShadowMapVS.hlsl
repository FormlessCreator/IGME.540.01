// Using shader include file:
#include "ShaderIncludeFile.hlsli"

cbuffer ExternalVSData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}

VertexToPixel main(VertexShaderInput input)
{
    // Get the Vertex to pixel output;
    VertexToPixel output;
    
    // Get the wvp matrix of the light.
    matrix wvp = mul(projection, mul(view, world));
    
    // Change and return the new vertex position of the object
    // based on the light wvp.
    output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
    output.shadowMapPos = mul(wvp, float4(input.localPosition, 1.0f));
    return output;
}