#pragma once
#include <DirectXMath.h>
class Transform
{
public:
	Transform();
	~Transform();

	// Create Setters: Replace former raw transform data with new data information.
	void SetScale(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 position);
	void SetRotation(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 rotation);
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 translate);

	// Create Getter: Get data information.
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetPitchYawRoll();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetInverseTransposeMatrix();

	// Create Transformers: Actively transform or change raw data information.
	void Scale(float x, float y, float z);
	void Scale(DirectX::XMFLOAT3 scaleAmount);
	void Rotate(float x, float y, float z);
	void Rotate(DirectX::XMFLOAT3 rotationAmount);
	void MoveAbsolute(float x, float y, float z);
	void MoveAbsolute(DirectX::XMFLOAT3 offset);
	

private:
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 worldInverseTransposeMatrix;
};

