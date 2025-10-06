#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

class Material
{
public:
	Material(Microsoft::WRL::ComPtr<ID3D10VertexShader> vs, Microsoft::WRL::ComPtr<ID3D10PixelShader> ps, Microsoft::WRL::ComPtr<ID3D10InputLayout> il, DirectX::XMFLOAT4 color);
	~Material();
	Material(const Material& material) = delete;
	Material& operator=(const Material& material) = delete;

	// Getters:
	DirectX::XMFLOAT4 GetColorTint();
	ID3D10VertexShader* GetVertexShader();
	ID3D10PixelShader* GetPixelShader();
	ID3D10InputLayout* GetInputLayout();

	// Setters:
	void SetColorTint();
	void SetVertexShader();
	void SetPixelShader();
	void SetInputlayout();

	// Load Vertex and Pixel Shaders
	void LoadVertexShader();
	void LoadPixelShader();
	void LoadInputLayout();

private:
	DirectX::XMFLOAT4 colorTint;
	Microsoft::WRL::ComPtr<ID3D10VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D10PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D10InputLayout> inputLayout;
};

