#pragma once
#include "Input.h"
#include "Transform.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera(
		float aspectRatio, 
		DirectX::XMFLOAT3 intialPosition,
		DirectX::XMFLOAT3 startingOrientation,
		float fieldOfView,
		float nearClipPlane,
		float farClipPlane,
		float cameraMovementSpeed,
		float cameraMouseLookSpeed,
		bool isCameraPerspective);
	~Camera();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	void UpdateProjectionMatrix(float aspectRatio);
	void UpdateViewMatrix();
	void Update(float dt);

private:
	Transform transform;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	float fov;
	float nearClip;
	float farClip;
	float movementSpeed;
	float mouseLookSpeed;
	float aspectRatio;
	bool isPerspective;
	bool hasViewChanged;
	bool hasProjectionChanged;
};

