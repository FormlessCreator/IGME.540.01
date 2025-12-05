// Use the Shader Include file here:
#include "ShaderIncludeFile.hlsli"

VertexToPixelForPP main( uint id : SV_VertexID )
{
	// Create the output to passover to the next pipeline.
    VertexToPixelForPP output;
	
	// Create a brachless Full Screen VS for quick if loop bitwise
	// vertex creation for the ID.
	// Instead of doing if(id == 3) {ouput.position = float4(3,1,0,1);
	// output.uv = float2(2,0);
    output.uv = float2(
				(id << 1) & 2, // id % 2 * 2.
				id & 2);
	
	// Normal coordinate of the screen with extra +2 reach to account for uv = (2, 2).
	// x = -1 ---------> +3
	// y = 1
	//	   |
	//     |
	//	   V
	//	  -3
    output.position = float4(output.uv, 0, 1);
    output.position.x = output.position * 2 - 1;  // Make the number 3 or -1.
    output.position.y = output.position * -2 + 1; // Make the number reverse 1 or -3 for y
	
	// Return the output.
	return output;
}