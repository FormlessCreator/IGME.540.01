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

	// Add the inverse transpose matrix of the wSpace to get the right 
	// rotation and scaling for the normals of the all objects in the
	// world spacein relation to the camera.
	DirectX::XMMATRIX worldInverseTransposeMatrix;
};

struct PixelDataStruct
{
	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT2 time;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 offset;
};

