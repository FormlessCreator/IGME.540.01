#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

// Add the light header struct.
#include "Lights.h"

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

struct SkyBufferStruct
{
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

struct PixelDataStruct
{
	// Add padding to fit HLSL 16 bytes standard.
	DirectX::XMFLOAT4 colorTint;

	DirectX::XMFLOAT2 time;
	DirectX::XMFLOAT2 timePad;

	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 scalePadding;

	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 offsetPadding;

	// Add and pass the current camera position and the entity's roughness for
	// the only the pixel shader.
	DirectX::XMFLOAT4 cameraCurrentPosition;

	DirectX::XMFLOAT2 roughness;
	DirectX::XMFLOAT2 roughnessPadding;

	DirectX::XMFLOAT4 ambientColor;

	// Add the Light struct object in the pixel shader struct.
	Lights directionalLight1;

	// Create an array of lights.
	Lights lightArray[5];

	/*Lights directionalLight2;
	Lights pointLight1;
	Lights spotLight1;
	Lights spotLight2;*/
};

// Create buffer struct for the shadow vertex shader CB data.
struct ShadowVSData
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 cameraView;
	DirectX::XMFLOAT4X4 cameraProjection;
};

