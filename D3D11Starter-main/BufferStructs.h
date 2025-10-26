#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

// using namespace DirectX;

struct BufferStructs
{
	//DirectX::XMFLOAT4 colorTint;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

struct PixelDataStruct
{
	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT2 time;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 offset;
};

