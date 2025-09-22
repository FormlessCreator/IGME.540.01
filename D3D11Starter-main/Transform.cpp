#include "Transform.h"
using namespace DirectX;

Transform::Transform()
{
	// Set and store the initial data for variables by loading.
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&scale, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	
	// Store up the matrix with an identity matrix.
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&worldInverseTransposeMatrix, DirectX::XMMatrixIdentity());
}

Transform::~Transform()
{
}

void Transform::SetScale(float x, float y, float z)
{
	// Store the new scale.
	XMStoreFloat3(&scale, XMVectorSet(x, y, z, 1.0f));
}

void Transform::SetScale(DirectX::XMFLOAT3 position)
{
	// Store the new scale position.
	XMStoreFloat3(&scale, XMVectorSet(position.x, position.y, position.z, 1.0f));
}

void Transform::SetRotation(float x, float y, float z)
{
	// Store the new rotation.
	XMStoreFloat3(&rotation, XMVectorSet(x, y, z, 0.0f));
}

void Transform::SetRotation(DirectX::XMFLOAT3 rotation)
{
	// Set the new rotation.
	XMStoreFloat3(&rotation, XMVectorSet(rotation.x, rotation.y, rotation.z, 0.0f));
}

void Transform::SetPosition(float x, float y, float z)
{
	// Set the new position values.
	XMStoreFloat3(&position, XMVectorSet(x, y, z, 0.0f));
}

void Transform::SetPosition(DirectX::XMFLOAT3 translate)
{
	// Set the new translated position.
	XMStoreFloat3(&position, XMVectorSet(translate.x, translate.y, translate.z, 0.0f));
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT3 Transform::GetPitchYawRoll()
{
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	// Create a xmmatrix variables to use for calculations.
	// Convert each floats variables to their respective matrix.
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	
	// Calculate the world matrix.
	XMMATRIX world = scaleMatrix * rotationMatrix * translationMatrix;

	// Store the world matrix in the float 4 by 4.
	XMStoreFloat4x4(&worldMatrix, world);

	// Return the stored matrix.
	return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetInverseTransposeMatrix()
{
	// Create a xmmatrix variable to use for calculations. 
	//XMMATRIX worldInTransMatrix = 
		//XMMatrixInverse(0, XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix)));
	// Create a xmmatrix variables to use for calculations.
	// Convert each floats variables to their respective matrix.
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

	// Calculate the world matrix.
	XMMATRIX world = scaleMatrix * rotationMatrix * translationMatrix;

	// Store in the world inverse transpose matrix.
	XMStoreFloat4x4(&worldInverseTransposeMatrix, 
		XMMatrixInverse(0, XMMatrixTranspose(world)));

	// Return the stored matrix.
	return worldInverseTransposeMatrix;
}

void Transform::Scale(float x, float y, float z)
{
	// Store the float values after vector calculations.
	// Store the new changed scale.
	// The w for scale should be 1.0f
	XMStoreFloat3(&scale, XMLoadFloat3(&scale) *
		XMVectorSet(x, y, z, 1.0f));
}

void Transform::Scale(DirectX::XMFLOAT3 scaleAmount)
{
	// Store the new changed scale.
	// The w for scale should be 1.0f
	XMStoreFloat3(&scale, XMLoadFloat3(&scale) * 
		XMVectorSet(
			scaleAmount.x,
			scaleAmount.y,
			scaleAmount.z,
			1.0f));
}

void Transform::Rotate(float x, float y, float z)
{
	// Store calculation in the rotation variable.
	XMStoreFloat3(&rotation,
		XMLoadFloat3(&rotation) + XMVectorSet(x, y, z, 0.0f));
}

void Transform::Rotate(DirectX::XMFLOAT3 rotationAmount)
{
	// Store calculation in the rotation variable.
		// Store calculation in the rotation variable.
	XMStoreFloat3(&rotation,
		XMLoadFloat3(&rotation) + 
		XMVectorSet(rotationAmount.x, rotationAmount.y, rotationAmount.z, 0.0f));
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	// Store calculation in the position variable.
	XMStoreFloat3(&position, XMLoadFloat3(&position) + XMVectorSet(x, y, z, 0.0f));
}

void Transform::MoveAbsolute(DirectX::XMFLOAT3 offset)
{
	// Store calculation in the position variable.
	XMStoreFloat3(&position, 
		XMLoadFloat3(&position) + XMVectorSet(offset.x, offset.y, offset.z, 0.0f));
}
