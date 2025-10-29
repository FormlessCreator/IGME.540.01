// Define the unique identifer for the shader include file.
// Each .hlsli needs a unique identifier with a safe guard definer.
#ifndef _GGP_SHADER_INCLUDES_FILE_
#define _GGP_SHADER_INCLUDES_FILE_

// Include and move all structs from the vertex and pixel shaders here:

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float3 localPosition : POSITION; // XYZ position
    float2 uv : TEXCOORD; // The uv texture coordinate.
    float3 normal : NORMAL; // The normal direction of the vertex.
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    //float2 scale			: SCALE;
    //float2 offset			: OFFSET;
	
	// Remove the color.
	//float4 color			: COLOR;
};

// Helper functions:

// Create a Color Change class that changes color based on the float value it is provided.
float4 ColorChange(float4 value, float2 time)
{
	// Create four colors.
    float4 color1 = float4(0.95f, 0.45f, 0.65f, 0.0f);
    float4 color2 = float4(1.00f, 0.45f, 0.45f, 0.0f);
    float4 color3 = float4(0.75f, 0.45f, 1.00f, 0.0f);
    float4 color4 = float4(1.00f, 0.40f, 0.75f, 0.0f);
	
	// Return a random value using the color that uses a cos and the value with time.
    float4 x = color1 + color2 * cos(color3 + color4 + value + time.r);
	
	// Return the new color.
    return x;
}

// Using Professor Chris Casioli Helper code.
float random(float2 s)
{
    return frac(sin(dot(s, float2(12.9898, 78.233))) * 43758.5453123);
}

// Close the if guard defination.
#endif