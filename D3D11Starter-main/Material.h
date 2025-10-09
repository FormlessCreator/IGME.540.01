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
	void LoadVertexShader();
	void LoadPixelShader();

private:
	DirectX::XMFLOAT4 colorTint;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::wstring pixelShaderFileName;
};

