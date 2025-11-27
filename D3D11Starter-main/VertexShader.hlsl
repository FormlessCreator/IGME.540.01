// Add the include shader file here.
#include "ShaderIncludeFile.hlsli"

// Create an external data for the constant buffer for the vertex
// shader to recognize it.
cbuffer VSExternalData : register(b0)
{
    //float4 tint;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	
	// Add the object inverse transpose matrix in the world space.
    matrix worldInverseTransposeMatrix;
}

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output; 

	// Here we're essentially passing the input position directly through to the next
	// stage (rasterizer), though it needs to be a 4-component vector now.  
	// - To be considered within the bounds of the screen, the X and Y components 
	//   must be between -1 and 1.  
	// - The Z component must be between 0 and 1.  
	// - Each of these components is then automatically divided by the W component, 
	//   which we're leaving at 1.0 for now (this is more useful when dealing with 
	//   a perspective projection matrix, which we'll get to in the future).
	
	// Get the world, to view, to projection matrix.
    matrix wvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
	
	// Add the input information to the output.
    output.uv = input.uv;
	
	// Add the tangent to the output.
	// The tangent need to be in world space.
    output.tangent = normalize(mul((float3x3) worldMatrix, input.tangent));
	
	// Change the input normal by calculating the object's correct normal
	// rotation and scaling in world space (Do not calculate Translation as
	// normal needs to be normalize before and after the out put stage).
	// IT matrix rotation unit is 1,1,1 always normalized.
	// IT matrix scale is always 1,1,1 normalized to.
    output.normal = mul((float3x3)worldInverseTransposeMatrix, input.normal);
	
	// Not correct! Normal of vertex does not not change or move with object's rotation.
    // output.normal = input.normal;
	
	// Not completely correct! Normal of vertex does not change or move with the object's
	// scale but it moves with the object's world space rotation matrix.
    //output.normal = mul((float3x3) worldMatrix, input.normal);

	// Pass the color through 
	// - The values will be interpolated per-pixel by the rasterizer
	// - We don't need to alter it here, but we do need to send it to the pixel shader
	//output.color = tint;
	
	// Normalize the normals for light calculation for the next input to distribute the
	// corrected scale version of normals that has be set to unit normals.
    output.normal = normalize(output.normal);
	
	// Get the world position of the vertex using the local position and the world matrix.
    output.worldPosition = mul(worldMatrix, float4(input.localPosition, 1.0f)).xyz;
	
	// To fix Linkage error.
    output.shadowMapPos = float4(0, 0, 0, 0);

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}