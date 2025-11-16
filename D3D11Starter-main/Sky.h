#pragma once

// Add includes for header.
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"

class Sky
{
public:
	// Create Sky constructor and deconstructor.
	Sky(std::shared_ptr<Mesh> mesh,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState,
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);
	~Sky();

	// Create a rasterizer state.
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRasterizerState();

	// Create a depth stencil state.
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDepthStencilState();

	// Create a sky Draw method().
	void Draw();

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
	// --- HEADER ---
	// Helper for creating a cubemap from 6 individual textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> skySamplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> skyDepthSS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> skyRasterizeState;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> skyPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> skyVertexShader;
	std::shared_ptr<Mesh> skyGeometryMesh;
};

