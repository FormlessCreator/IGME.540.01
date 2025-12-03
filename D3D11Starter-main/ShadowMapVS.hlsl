// Using shader include file:
#include "ShaderIncludeFile.hlsli"

cbuffer ExternalVSData : register(b0)
{
    matrix world;
    matrix lightView;
    matrix lightProjection;
    //matrix cameraView;
    //matrix cameraProjection;
}

float4 main(VertexShaderInput input): SV_POSITION
{

    // Get the wvp matrix of the object.
    matrix wvp = mul(lightProjection, mul(lightView, world));
    
    // Get the shadow.
    return mul(wvp, float4(input.localPosition, 1.0f));
    
    //// Get the Vertex to pixel output;
    //VertexToPixel output;
    
    //// Get the wvp matrix of the object.
    //matrix wvp = mul(cameraProjection, mul(cameraView, world));
    
    //// Get the wvp matrix of the object.
    //matrix shadowWVP = mul(lightProjection, mul(lightView, world));
    
    //// Get the camera.
    //output.worldPosition = mul(world, float4(input.localPosition, 1.0f)).xyz;
    //output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
    
    //// Just input.
    //output.uv = input.uv;
    //output.normal = input.normal;
    //output.tangent = input.tangent;
    
    //// Get the shadow.
    //output.shadowMapPos = mul(shadowWVP, float4(input.localPosition, 1.0f));
    
    //return output;
}