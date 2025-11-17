#include "Sky.h"
#include "PathHelpers.h"
#include <d3dcompiler.h>
#include <string>

// Add the WIC texture loader package.
#include "WICTextureLoader.h"

Sky::Sky(std::shared_ptr<Mesh> mesh,
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState,
	const wchar_t* right,
	const wchar_t* left,
	const wchar_t* up,
	const wchar_t* down,
	const wchar_t* front,
	const wchar_t* back, 
	const std::wstring& vertexFilePath,
	const std::wstring& pixelFilePath) // Pass in ps shader and vS
{
	// Get the device.
	device = Graphics::Device.Get();

	// Create a rasterizer and depth stencil state.
	CreateRasterizerState();
	CreateDepthStencilState();

	//skyRasterizeState = skyRasterizeState;

	// Initialize the variables.
	skyGeometryMesh = mesh;
	skySamplerState = samplerState;
	skySRV = CreateCubemap(right, left, up, down, front, back);

	// Load the Vertex and Pixel shader.
	LoadSkyVertexShader(vertexFilePath);
	LoadSkyPixelShader(pixelFilePath);

	//Microsoft::WRL::ComPtr<ID3D11Device> device;
	//Microsoft::WRL::ComPtr<ID3D11SamplerState> skySamplerState;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilState> skyDepthSS;
	//Microsoft::WRL::ComPtr<ID3D11RasterizerState> skyRasterizeState;
	//-Microsoft::WRL::ComPtr<ID3D11PixelShader> skyPixelShader;
	//-Microsoft::WRL::ComPtr<ID3D11VertexShader> skyVertexShader;
	//std::shared_ptr<Mesh> skyGeometryMesh;
}

Sky::~Sky()
{

}

void Sky::CreateRasterizerState()
{
	// Create a desc Rastarize object.
	D3D11_RASTERIZER_DESC rastDes = {};
	rastDes.FillMode = D3D11_FILL_SOLID;
	rastDes.CullMode = D3D11_CULL_FRONT;
	rastDes.DepthClipEnable = true;

	// Create a rasterizer state.
	device->CreateRasterizerState(&rastDes, skyRasterizeState.GetAddressOf());
}

void Sky::CreateDepthStencilState()
{
	// Craete a desc object for the depth stencil state.
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	device->CreateDepthStencilState(&depthDesc, skyDepthSS.GetAddressOf());
}

void Sky::LoadSkyPixelShader(const std::wstring& pixelShaderFilePath)
{
	// Load the vertex shader:
	// Using Chris Casioli code reference:
	// Create a binary large object to hold a read external pixel shader cso file information.
	ID3DBlob* pixelShaderBlob;

	// Loading shaders
	//  - Visual Studio will compile our shaders at build time
	//  - They are saved as .cso (Compiled Shader Object) files
	//  - We need to load them when the application starts
	{
		// Read our compiled shader code files into blobs
		// - Essentially just "open the file and plop its contents here"
		// - Uses the custom FixPath() helper from Helpers.h to ensure relative paths
		// - Note the "L" before the string - this tells the compiler the string uses wide characters
		D3DReadFileToBlob(FixPath(pixelShaderFilePath).c_str(), &pixelShaderBlob);
		//D3DReadFileToBlob(FixPath(L"PixelShader.cso").c_str(), &pixelShaderBlob);

		// Create the actual Direct3D shaders on the GPU
		Graphics::Device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),	// Pointer to blob's contents
			pixelShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			skyPixelShader.GetAddressOf());			// Address of the ID3D11PixelShader pointer
	}
}

void Sky::LoadSkyVertexShader(const std::wstring& vertexShaderFilePath)
{
	// -------------------------------------------------------------------------
	ID3DBlob* vertexShaderBlob;
	D3DReadFileToBlob(FixPath(vertexShaderFilePath).c_str(), &vertexShaderBlob);
	Graphics::Device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(), // Pointer to start of binary data
		vertexShaderBlob->GetBufferSize(), // How big is that data?
		0, // No classes in this shader
		skyVertexShader.GetAddressOf());

	// Bind the textures and sampler state for the pixel shaders to access.
	// Graphics::Context->PSGetShaderResources(0, 1, )

	// Optimize the shader class so you only initialize and use the same shader once.
	// Using Chris Casiolio code reference for the input layout:
	// Create an input layout 
	//  - This describes the layout of data sent to a vertex shader
	//  - In other words, it describes how to interpret data (numbers) in a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the vertex shader blob above)
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[4] = {};

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

		// Update the input layout element for tangent.
		inputElements[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElements[3].SemanticName = "TANGENT";
		inputElements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

		//// Set up the first element - a position, which is 3 float values
		//inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
		//inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
		//inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

		//// Add a sample direction input element.
		//// Create the input layout information for the sample direction.
		//inputElements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// The float float bit sizes format.
		//inputElements[1].SemanticName = "DIRECTION";						// The sample direction coordinate sematic name.
		//inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// Order in which the vertex normal byte info is read - After the previous element.

		// Create the input layout, verifying our description against actual shader code
		Graphics::Device->CreateInputLayout(
			inputElements,							// An array of descriptions
			4,										// How many elements in that array? // 2 -> 2 now!
			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
			skyInputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
	}
}

void Sky::Draw()
{
	// Set and bind the RS and DSS.
	Graphics::Context->RSSetState(skyRasterizeState.Get());
	Graphics::Context->OMSetDepthStencilState(skyDepthSS.Get(), 0);

	// Set and bind the VS and PS of the sky.
	Graphics::Context->VSSetShader(skyVertexShader.Get(), nullptr, 0);
	Graphics::Context->PSSetShader(skyPixelShader.Get(), nullptr, 0);

	// Set the input layout.
	Graphics::Context->IASetInputLayout(skyInputLayout.Get());

	// Set the primitive toplology.
	Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set and bid the Sampler and SRV.
	Graphics::Context->PSSetSamplers(0, 1, skySamplerState.GetAddressOf());
	Graphics::Context->PSSetShaderResources(0, 1, skySRV.GetAddressOf());

	// Draw the Mesh Cube.
	skyGeometryMesh->Draw();

	// Change them back to defualt settings.
	Graphics::Context->RSSetState(0);
	Graphics::Context->OMSetDepthStencilState(0, 0);
}

// --------------------------------------------------------
// Author: Chris Cascioli
// Purpose: Creates a cube map on the GPU from 6 individual textures
// 
// - You are allowed to directly copy/paste this into your code base
//   for assignments, given that you clearly cite that this is not
//   code of your own design.
//
// - Note: This code assumes you’re putting the function in Sky.cpp, 
//   you’ve included WICTextureLoader.h and you have an ID3D11Device 
//   ComPtr called “device”.  Make any adjustments necessary for
//   your own implementation.
// --------------------------------------------------------

// --- .CPP CODE ---

// --------------------------------------------------------
// Loads six individual textures (the six faces of a cube map), then
// creates a blank cube map and copies each of the six textures to
// another face.  Afterwards, creates a shader resource view for
// the cube map and cleans up all of the temporary resources.
// --------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Sky::CreateCubemap(
	const wchar_t* right,
	const wchar_t* left,
	const wchar_t* up,
	const wchar_t* down,
	const wchar_t* front,
	const wchar_t* back)
{
	// Load the 6 textures into an array.
	// - We need references to the TEXTURES, not SHADER RESOURCE VIEWS!
	// - Explicitly NOT generating mipmaps, as we don't need them for the sky!
	// - Order matters here!  +X, -X, +Y, -Y, +Z, -Z
	Microsoft::WRL::ComPtr<ID3D11Texture2D> textures[6] = {};
	CreateWICTextureFromFile(Graphics::Device.Get(), right, (ID3D11Resource**)textures[0].GetAddressOf(), 0);
	CreateWICTextureFromFile(Graphics::Device.Get(), left, (ID3D11Resource**)textures[1].GetAddressOf(), 0);
	CreateWICTextureFromFile(Graphics::Device.Get(), up, (ID3D11Resource**)textures[2].GetAddressOf(), 0);
	CreateWICTextureFromFile(Graphics::Device.Get(), down, (ID3D11Resource**)textures[3].GetAddressOf(), 0);
	CreateWICTextureFromFile(Graphics::Device.Get(), front, (ID3D11Resource**)textures[4].GetAddressOf(), 0);
	CreateWICTextureFromFile(Graphics::Device.Get(), back, (ID3D11Resource**)textures[5].GetAddressOf(), 0);

	// We'll assume all of the textures are the same color format and resolution,
	// so get the description of the first texture
	D3D11_TEXTURE2D_DESC faceDesc = {};
	textures[0]->GetDesc(&faceDesc);

	// Describe the resource for the cube map, which is simply 
	// a "texture 2d array" with the TEXTURECUBE flag set.  
	// This is a special GPU resource format, NOT just a 
	// C++ array of textures!!!
	D3D11_TEXTURE2D_DESC cubeDesc = {};
	cubeDesc.ArraySize = 6;            // Cube map!
	cubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // We'll be using as a texture in a shader
	cubeDesc.CPUAccessFlags = 0;       // No read back
	cubeDesc.Format = faceDesc.Format; // Match the loaded texture's color format
	cubeDesc.Width = faceDesc.Width;   // Match the size
	cubeDesc.Height = faceDesc.Height; // Match the size
	cubeDesc.MipLevels = 1;            // Only need 1
	cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; // This should be treated as a CUBE, not 6 separate textures
	cubeDesc.Usage = D3D11_USAGE_DEFAULT; // Standard usage
	cubeDesc.SampleDesc.Count = 1;
	cubeDesc.SampleDesc.Quality = 0;

	// Create the final texture resource to hold the cube map
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cubeMapTexture;
	Graphics::Device->CreateTexture2D(&cubeDesc, 0, cubeMapTexture.GetAddressOf());

	// Loop through the individual face textures and copy them,
	// one at a time, to the cube map texure
	for (int i = 0; i < 6; i++)
	{
		// Calculate the subresource position to copy into
		unsigned int subresource = D3D11CalcSubresource(
			0,  // Which mip (zero, since there's only one)
			i,  // Which array element?
			1); // How many mip levels are in the texture?

		// Copy from one resource (texture) to another
		Graphics::Context->CopySubresourceRegion(
			cubeMapTexture.Get(),  // Destination resource
			subresource,           // Dest subresource index (one of the array elements)
			0, 0, 0,               // XYZ location of copy
			textures[i].Get(),     // Source resource
			0,                     // Source subresource index (we're assuming there's only one)
			0);                    // Source subresource "box" of data to copy (zero means the whole thing)
	}

	// At this point, all of the faces have been copied into the 
	// cube map texture, so we can describe a shader resource view for it
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = cubeDesc.Format;         // Same format as texture
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE; // Treat this as a cube!
	srvDesc.TextureCube.MipLevels = 1;        // Only need access to 1 mip
	srvDesc.TextureCube.MostDetailedMip = 0;  // Index of the first mip we want to see

	// Make the SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeSRV;
	Graphics::Device->CreateShaderResourceView(cubeMapTexture.Get(), &srvDesc, cubeSRV.GetAddressOf());

	// Send back the SRV, which is what we need for our shaders
	return cubeSRV;
}