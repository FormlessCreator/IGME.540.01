#include "Transform.h"
using namespace DirectX;

Transform::Transform()
{
	// Set and store the initial data for variables by loading.
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&scale, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
	
	// Store up the matrix with an identity matrix.
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&worldInverseTransposeMatrix, DirectX::XMMatrixIdentity());

	// If any method is used update the world and world inverse matrix.
	// If any changes is made to the tranform data, get the new world matrix.
	matrixUpdate = true;

	// Make set up IT matrix false.
	setUpITMatrix = true;
}

Transform::~Transform()
{
}

void Transform::SetScale(float x, float y, float z)
{
	// Store the new scale.
	XMStoreFloat3(&scale, XMVectorSet(x, y, z, 1.0f));
	
	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::SetScale(DirectX::XMFLOAT3 position)
{
	// Store the new scale position.
	XMStoreFloat3(&scale, XMVectorSet(position.x, position.y, position.z, 1.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::SetRotation(float x, float y, float z)
{
	// Store the new rotation.
	XMStoreFloat3(&rotation, XMVectorSet(x, y, z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::SetRotation(DirectX::XMFLOAT3 rotationInput)
{
	// Set the new rotation.
	XMStoreFloat3(&rotation, XMVectorSet(rotationInput.x, rotationInput.y, rotationInput.z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::SetPosition(float x, float y, float z)
{
	// Set the new position values.
	XMStoreFloat3(&position, XMVectorSet(x, y, z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::SetPosition(DirectX::XMFLOAT3 translate)
{
	// Set the new translated position.
	XMStoreFloat3(&position, XMVectorSet(translate.x, translate.y, translate.z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
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
	// If the matrix matrix is true as tranform data changes, calculate a new matrix.
	if (matrixUpdate)
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

		// Set the update matrix to false.
		matrixUpdate = false;


		//if (!setUpITMatrix)
		//{
		//	// Using the world.
		//	// Store in the world inverse transpose matrix.
		//	XMStoreFloat4x4(&worldInverseTransposeMatrix,
		//		XMMatrixInverse(0, XMMatrixTranspose(world)));

		//	// Make the bool setup true.
		//	setUpITMatrix = true;
		//}
	}

	// Else return the previously store matrix with no changes.
	// Return the stored matrix.
	return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetInverseTransposeMatrix()
{
	// If the matrix matrix is true as tranform data changes, calculate a new matrix.
	if (setUpITMatrix)
	{
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

		// Set the update matrix to false.
		setUpITMatrix = false;
	}

	//// Create a xmmatrix variables to use for calculations.
	//	// Convert each floats variables to their respective matrix.
	//XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	//XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	//XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

	//// Calculate the world matrix.
	//XMMATRIX world = scaleMatrix * rotationMatrix * translationMatrix;

	//// Store in the world inverse transpose matrix.
	//XMStoreFloat4x4(&worldInverseTransposeMatrix,
	//	XMMatrixInverse(0, XMMatrixTranspose(world)));

	//// Set the update matrix to false.
	//matrixUpdate = false;

	// Else return the previously store matrix with no changes.
	// Return the stored matrix.
	return worldInverseTransposeMatrix;
}

void Transform::Scale(float x, float y, float z)
{
	// Store the float values after vector calculations.
	// Store the new changed scale.
	// The w for scale should be 1.0f
	XMStoreFloat3(&scale, XMVectorMultiply(XMLoadFloat3(&scale),
		XMVectorSet(x, y, z, 1.0f)));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::Scale(DirectX::XMFLOAT3 scaleAmount)
{
	// Store the new changed scale.
	// The w for scale should be 1.0f
	XMStoreFloat3(&scale, XMVectorMultiply(XMLoadFloat3(&scale),
		XMVectorSet(
			scaleAmount.x,
			scaleAmount.y,
			scaleAmount.z,
			1.0f)));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::Rotate(float x, float y, float z)
{
	// Store calculation in the rotation variable.
	XMStoreFloat3(&rotation,
		XMLoadFloat3(&rotation) + XMVectorSet(x, y, z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::Rotate(DirectX::XMFLOAT3 rotationAmount)
{
	// Store calculation in the rotation variable.
		// Store calculation in the rotation variable.
	XMStoreFloat3(&rotation,
		XMVectorAdd(XMLoadFloat3(&rotation),
		XMVectorSet(rotationAmount.x, rotationAmount.y, rotationAmount.z, 0.0f)));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	// Store calculation in the position variable.
	XMStoreFloat3(&position, XMLoadFloat3(&position) + XMVectorSet(x, y, z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::MoveAbsolute(DirectX::XMFLOAT3 offset)
{
	// Store calculation in the position variable.
	XMStoreFloat3(&position, 
		XMLoadFloat3(&position) + XMVectorSet(offset.x, offset.y, offset.z, 0.0f));

	// Update matrix is true.
	matrixUpdate = true;
	setUpITMatrix = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
	// Move to new position in relative or with regards to its former transform rotation.
	// Create a vector set for the x, y, and z direction offset values.
	XMVECTOR newOffset = XMVectorSet(x, y, z, 0.0f);

	// Create a new quartenion of the current transform rotation.
	XMVECTOR currentRotQuaternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	// Create a new quarternion:
	// This rotates our new position offset by our current rotation values.
	// So the new transform position offset have the same rotation as it is
	// added to the previous position.
	XMVECTOR newRotatedPosition = XMVector3Rotate(newOffset, currentRotQuaternion);

	// Move the object transform position to the new rotated offset position.
	XMStoreFloat3(&position,
		XMLoadFloat3(&position) + newRotatedPosition);
}

void Transform::MoveRelative(DirectX::XMFLOAT3 offset)
{
	// Get the the current transform rotation quarternion.
	XMVECTOR currentRotQuarternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	// Rotate the loaded new offset by the current rotation of the transform.
	XMVECTOR newRotatedOffsetPosition = XMVector3Rotate(XMLoadFloat3(&offset), currentRotQuarternion);

	// Add the new rotated offset position to the current transform position and store it.
	XMStoreFloat3(&position,
		XMLoadFloat3(&position) + newRotatedOffsetPosition);
}

DirectX::XMFLOAT3 Transform::GetUp()
{
	// Create an normal xmvector for up.
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Get the rotation quarternion of the transformation.
	XMVECTOR rotQuaternion = XMQuaternionRotationRollPitchYaw(
		rotation.x,
		rotation.y,
		rotation.z);
	
	// Get the rotated Up
	XMVECTOR rotatedUp = XMVector3Rotate(
		up, rotQuaternion);

	// Create an xmfloat3 to store the updated rotated up.
	XMFLOAT3 storeRotatedUp;
	
	// Store the rotated up.
	XMStoreFloat3(&storeRotatedUp, rotatedUp);
	
	return storeRotatedUp;

	// Can create bool to check if rotation changes to calculate it later.
	// Can create variables to make it efficient.
}

DirectX::XMFLOAT3 Transform::GetForward()
{
	// Create a world forward vector.
	XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Get the quartenion of the transform rotation.
	XMVECTOR rotQuarternion = XMQuaternionRotationRollPitchYaw(
		rotation.x,
		rotation.y,
		rotation.z
	);

	// Rotate the world forward vector by the rotation quarternion.
	XMVECTOR rotatedForward = XMVector3Rotate(forward, rotQuarternion);

	// Create a xmfloat3 to store the rotated forward.
	XMFLOAT3 storeForward;

	// Store rotated forward.
	XMStoreFloat3(&storeForward, rotatedForward);

	// Return the store forward.
	return storeForward;
}

DirectX::XMFLOAT3 Transform::GetRight()
{
	// Create a xmvector for the world right.
	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	// Create a quarternion of the rotation of the object.
	XMVECTOR rotQuaternion = XMQuaternionRotationRollPitchYaw(
		rotation.x,
		rotation.y,
		rotation.z
	);

	// Rotate the world right by the rotational quarternion of the transform.
	XMVECTOR rotatedright = XMVector3Rotate(right, rotQuaternion);

	// Create a xmfloat to store the rotated right vector as float.
	XMFLOAT3 storeRotatedRight;
	XMStoreFloat3(&storeRotatedRight, rotatedright);

	// return the stored rotated right.
	return storeRotatedRight;
}


