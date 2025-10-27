#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>

class Material
{
public:
	Material();
	Material(
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vs,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> ps,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> il,
		const std::wstring& psFileName,
		DirectX::XMFLOAT4 color);
	~Material();
	//Material(const Material& material) = delete;
	//Material& operator=(const Material& material) = delete;

	// Getters:
	DirectX::XMFLOAT4 GetColorTint();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout();

	// Setters:
	void SetColorTint(DirectX::XMFLOAT4 color);
	void SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> vs);
	void SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> ps);
	void SetInputlayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> il);

	// Load Vertex and Pixel Shaders
	//void LoadVertexShader();
	void LoadPixelShader();

	// Create method that add texture shader resources to the texture SRV array 
	// and the samplers array.
	void AddTextureSRV(unsigned int shaderRegisterIndex, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvData);
	void AddSampler(unsigned int shaderRegisterIndex, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerData);
	
	// Create a method that sets all the textue SRV and samplers active.
	void BindTexturesAndSamplers();

	// Get method for the scale and offset.
	DirectX::XMFLOAT2 GetTextureScale();
	DirectX::XMFLOAT2 GetTextureOffset();

	// Create a set for the texture scale and texture.
	DirectX::XMFLOAT2 SetTextureScale(DirectX::XMFLOAT2 scale);
	DirectX::XMFLOAT2 SetTextureOffset(DirectX::XMFLOAT2 offset);

	// Get the individual shader resources view in the comPtr array of the material.
	// By returning the address of the first pointer.
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceViewArray(unsigned int index);

	// Get the SRV current index.
	unsigned int GetCurrentSRVIndex();

private:
	DirectX::XMFLOAT4 colorTint;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::wstring pixelShaderFileName;

	// A material might need textures and various sampler to use so its right have a 
	// texture and sampler array or //unordered map: 
	// std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureSRVs;.
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRVs[128];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplers[16];

	// Store the current index of the texture shader resources and the sampler array.
	unsigned int currentSRVTextureIndex = 0;
	unsigned int currentSamplerIndex = 0;

	// Add variables to hold scaling and the offset for the given texture in materials.
	DirectX::XMFLOAT2 textureScale;
	DirectX::XMFLOAT2 textureOffset;
};

