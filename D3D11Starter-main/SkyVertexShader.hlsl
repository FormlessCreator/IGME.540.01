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
    VertexToPixel_SkyBox output;
    
    // Set the view translation matrix to 0;
    // Create a version of the veiw matrix without its translation.
    matrix viewNoTranslation = viewMatrix;
    viewNoTranslation._14 = 0.0f;
    viewNoTranslation._24 = 0.0f;
    viewNoTranslation._34 = 0.0f;
    
    // Calculate the output postion of the sky box pixel position
    // with just the view and projection matrix.
    matrix viewProjection = mul(projectionMatrix, viewNoTranslation);
    
    // Apply view projection to the input position.
    // we do not want the position of the skybox to be in world space but to be in
    // the local position of the camera.
    output.screenPosition = mul(viewProjection, float4(input.localPosition, 1.0f));
    
    // Set the output screen of the camera depth to be always the homogenous coordinate.
    output.screenPosition.z = output.screenPosition.w;
    
    // Since the camera position is always 0, subtracting the position of the sky box
    // with the camera position to find the direction of the sky box input position will
    // be useless as it always gives us the position of the sky box as the direction.
    // Instead of subtracting just use the position of the skybox as the direction.
    output.sampleDir = input.localPosition;
    
	return output;
}