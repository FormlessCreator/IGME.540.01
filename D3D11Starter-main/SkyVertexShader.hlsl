// Add the include shader file here.
#include "ShaderIncludeFile.hlsli"

// Add a cbuffer
cbuffer SkyBoxData : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
}

VertexToPixel_SkyBox main( VertexShaderInput input )
{
    VertexShaderInput output;
    
    // Set the view translation matrix to 0;
    // Create a version of the veiw matrix without its translation.
    matrix viewWithoutTraslation = viewMatrix;
    viewWithoutTraslation._14 = 0.0f;
    viewWithoutTraslation._24 = 0.0f;
    viewWithoutTraslation._34 = 0.0f;
    
    // Calculate the output postion of the sky box pixel position
    // with just the view and projection matrix.
    matrix viewProjection = mul(projectionMatrix, viewWithoutTraslation);
    output.localPosition = mul(viewProjection, float4(input.localPosition, 1.0f));
    
	return output;
}