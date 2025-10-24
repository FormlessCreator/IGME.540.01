// Create a cbuffer struct for the pixel shader.
cbuffer PSExternalData1 : register(b0)
{
    float4 colorTint;
    float2 time;
}

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
	
	// Remove the color.
	//float4 color			: COLOR;
};

// Create a Color Change class that changes color based on the float value it is provided.
float4 ColorChange(float4 value)
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
// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
    // With some reference Kishimisu's tutorial: https://www.youtube.com/watch?v=f4s1h2YETNY
    // I added my own custom code on some of the tutorial code by adding additional code 
    // that outputs color using randomness, color changes, distance, sine, absolute value,
    // and other calculations.
    float randomFloat = random(float2(input.normal.x, input.normal.z));
    float magnitudeOfUv = distance(input.uv.x, input.uv.y);
    float4 output = ColorChange(magnitudeOfUv);
    magnitudeOfUv = sin(magnitudeOfUv * 8.0 + time.x + randomFloat) / 8.06;
    magnitudeOfUv =  abs(magnitudeOfUv) + frac((sin(randomFloat / 250.0f)));
    magnitudeOfUv = 0.03 / magnitudeOfUv;
    output *= magnitudeOfUv + colorTint;
	
    // Return the 
    return output;
	
	// Test:
	//return float4(input.uv, 0, 1);
    //return float4(input.normal, 1);
}