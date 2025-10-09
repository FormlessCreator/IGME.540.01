#include "Material.h"
#include "Graphics.h"
#include "PathHelpers.h"
#include <d3dcompiler.h>
#include <string>

Material::Material()
{
	colorTint = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

Material::Material(
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs,
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps,
	Microsoft::WRL::ComPtr<ID3D11InputLayout> il,
	const std::wstring& psFileName,
	DirectX::XMFLOAT4 color)
{
	// Initialize the variables.
	vertexShader = vs;
	pixelShader = ps;
	inputLayout = il;
	colorTint = color;
	pixelShaderFileName = psFileName;
}

Material::~Material()
{
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
    return colorTint;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader()
{
    return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader()
{
    return pixelShader;
}

Microsoft::WRL::ComPtr<ID3D11InputLayout> Material::GetInputLayout()
{
    return inputLayout;
}

void Material::SetColorTint(DirectX::XMFLOAT4 color)
{
    colorTint = color;
}

void Material::SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> vs)
{
    vertexShader = vs;
}

void Material::SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> ps)
{
    pixelShader = ps;
}

void Material::SetInputlayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> il)
{
    inputLayout = il;
}

void Material::LoadVertexShader()
{
    // Load the vertex shader:
	// Using Chris Casioli code reference:
	// Create a binary large object to hold a read external vertex shader cso file information.
	ID3DBlob* vertexShaderBlob;

	// Loading shaders
	//  - Visual Studio will compile our shaders at build time
	//  - They are saved as .cso (Compiled Shader Object) files
	//  - We need to load them when the application starts
	{
		// Read our compiled shader code files into blobs
		// - Essentially just "open the file and plop its contents here"
		// - Uses the custom FixPath() helper from Helpers.h to ensure relative paths
		// - Note the "L" before the string - this tells the compiler the string uses wide characters
		D3DReadFileToBlob(FixPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);

		// Create a vertex shader using the blob infomation on the GPU.
		Graphics::Device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),	// Get a pointer to the blob's contents
			vertexShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			vertexShader.GetAddressOf());			// The address of the ID3D11VertexShader pointer
	}

	// Using Chris Casiolio code reference for the input layout:
	// Create an input layout 
	//  - This describes the layout of data sent to a vertex shader
	//  - In other words, it describes how to interpret data (numbers) in a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the vertex shader blob above)
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[3] = {};

		// Set up the first element - a position, which is 3 float values
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
		inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

		// Create the input layout information for the vertex buffer uv texture coordinate.
		inputElements[1].Format = DXGI_FORMAT_R32G32_FLOAT;					// The uv float bit sizes format.
		inputElements[1].SemanticName = "TEXCOORD";							// The uv texture coordinate sematic name.
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Order in which the vertex uv byte is read - After the previous element.

		// Create the input layout information for the normal direction.
		inputElements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// The float float bit sizes format.
		inputElements[2].SemanticName = "NORMAL";							// The uv texture coordinate sematic name.
		inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Order in which the vertex normal byte info is read - After the previous element.

		// Remove the input layout information for the color.
		// Set up the second element - a color, which is 4 more float values
		//inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 4x 32-bit floats
		//inputElements[1].SemanticName = "COLOR";							// Match our vertex shader input!
		//inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

		// Create the input layout, verifying our description against actual shader code
		Graphics::Device->CreateInputLayout(
			inputElements,							// An array of descriptions
			3,										// How many elements in that array? // 2 -> 3 now!
			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
			inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
	}
}

void Material::LoadPixelShader()
{
	// Load the vertex shader:
	// Using Chris Casioli code reference:
	// Create a binary large object to hold a read external pixel shader cso file information.
	ID3DBlob* pixelShaderBlob;

	// Create a com ptr for the pixel shader blob.
	//Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;

	// Loading shaders
	//  - Visual Studio will compile our shaders at build time
	//  - They are saved as .cso (Compiled Shader Object) files
	//  - We need to load them when the application starts
	{
		// Read our compiled shader code files into blobs
		// - Essentially just "open the file and plop its contents here"
		// - Uses the custom FixPath() helper from Helpers.h to ensure relative paths
		// - Note the "L" before the string - this tells the compiler the string uses wide characters
		//D3DReadFileToBlob(FixPath(pixelShaderFileName).c_str(), &pixelShaderBlob);
		D3DReadFileToBlob(FixPath(L"PixelShader.cso").c_str(), &pixelShaderBlob);

		// Create the actual Direct3D shaders on the GPU
		Graphics::Device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),	// Pointer to blob's contents
			pixelShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			pixelShader.GetAddressOf());			// Address of the ID3D11PixelShader pointer
	}
}

