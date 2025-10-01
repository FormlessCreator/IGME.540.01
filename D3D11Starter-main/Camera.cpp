#include "Camera.h"
using namespace DirectX;

Camera::Camera(float aspectRatio, DirectX::XMFLOAT3 initialPosition, DirectX::XMFLOAT3 startingOrientation, float fieldOfView, float nearClipPlane, float farClipPlane, float cameraMovementSpeed, float cameraMouseLookSpeed, bool isCameraPerspective)
{
    // Initialize the camera transform data.
    transform = Transform();
    transform.SetPosition(initialPosition);
    transform.SetRotation(startingOrientation);

    // Intialize other variable:
    fov = fieldOfView;
    nearClip = nearClipPlane;
    farClip = farClipPlane;
    movementSpeed = cameraMovementSpeed;
    mouseLookSpeed = cameraMouseLookSpeed;
    this->aspectRatio = aspectRatio;

    // Set the bool isPerspective.
    isPerspective = isCameraPerspective;

    // Intialize the view and projection hasChanged variable.
    hasViewChanged = true;

    // Call the update view and projection matrix.
    UpdateViewMatrix();
    UpdateProjectionMatrix(this->aspectRatio);
}

Camera::~Camera()
{
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
    // Return the view matrix.
    return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
    // Return the projection matrix.
    return projectionMatrix;
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    // Set this aspect ratio to the new aspect ratio.
    this->aspectRatio = aspectRatio;

    // If the camera Perpective is true.
    if (isPerspective)
    {
        // Create a projection matrix using a perspective fov lh.
        XMMATRIX cameraProjectionMatrix = XMMatrixPerspectiveFovLH(
            fov,
            this->aspectRatio,
            nearClip,
            farClip
        );

        // Store the matrix in the projection matrix.
        XMStoreFloat4x4(&projectionMatrix, cameraProjectionMatrix);
    }
    else
    {
        // Create a projection matrix using ortrographic view.
        XMMATRIX cameraProjectionMatrix = XMMatrixOrthographicLH(
            this->aspectRatio,
            this->aspectRatio,
            nearClip,
            farClip
        );

        // Store the matrix in the projection matrix.
        XMStoreFloat4x4(&projectionMatrix, cameraProjectionMatrix);
    }
}

void Camera::UpdateViewMatrix()
{
    // Get the camera position, forward and getup.
    XMFLOAT3 position = transform.GetPosition();
    XMFLOAT3 forward = transform.GetForward();
    XMFLOAT3 up = transform.GetUp();
    XMMATRIX cameraViewmarix = XMMatrixLookToLH(
        XMLoadFloat3(&position),
        XMLoadFloat3(&forward),
        XMLoadFloat3(&up)
    );

    // Store the matrix in the view matrix.
    XMStoreFloat4x4(&viewMatrix, cameraViewmarix);
}

void Camera::Update(float dt)
{
    // Create input to control the camera.
    // Scale each movement by the deltatime.
    
    // Move the camera relatively.
    if (Input::KeyDown('W'))
    {
        // Move the camera transform relatively towards its forward.
        // With time and movement speed.
        XMFLOAT3 forward = transform.GetForward();
        XMVECTOR forwardWithTime = XMVectorScale(XMLoadFloat3(&forward), dt * movementSpeed);

        // Store forwardwithtime.
        XMFLOAT3 store;
        XMStoreFloat3(&store, forwardWithTime);

        // Move forward with time.
        transform.MoveRelative(store);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    if (Input::KeyDown('S'))
    {
        // Move the camera relatively towards its backward.
        // (opposite of its forward).
        XMFLOAT3 forward = transform.GetForward();
        XMVECTOR backwardwithTime = XMVectorScale(XMLoadFloat3(&forward), -1 * dt * movementSpeed);
        
        // Store.
        XMFLOAT3 store;
        XMStoreFloat3(&store, backwardwithTime);

        // Move.
        transform.MoveRelative(store);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    if (Input::KeyDown('A'))
    {
        // Move the camera relatively towards its right().
        XMFLOAT3 right = transform.GetRight();
        XMVECTOR leftWithTime = XMVectorScale(XMLoadFloat3(&right), -1 * dt * movementSpeed);

        // Store.
        XMFLOAT3 store;
        XMStoreFloat3(&store, leftWithTime);

        // Move.
        transform.MoveRelative(store);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    if (Input::KeyDown('D'))
    {
        // Move the camera relatively towards its right().
        XMFLOAT3 right = transform.GetRight();
        XMVECTOR rightWithTime = XMVectorScale(XMLoadFloat3(&right), dt * movementSpeed);

        // Store.
        XMFLOAT3 store;
        XMStoreFloat3(&store, rightWithTime);

        // Move.
        transform.MoveRelative(store);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    // Move the camera up along the y axis absolutely.
    if (Input::KeyDown(VK_SPACE))
    {
        // Move the camera relatively towards its upward.
        XMFLOAT3 up = transform.GetUp();
        XMVECTOR upWithTime = XMVectorScale(XMLoadFloat3(&up), dt * movementSpeed);

        // Store.
        XMFLOAT3 store;
        XMStoreFloat3(&store, upWithTime);

        // Move absolutely.
        transform.MoveAbsolute(store);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    // Move the camera down along the y axis absolutely.
    if (Input::KeyDown('X'))
    {
        // Get the opposite of tranform get up().
        XMFLOAT3 up = transform.GetUp();
        XMVECTOR downWithTime = XMVectorScale(XMLoadFloat3(&up), -1 * dt * movementSpeed);

        // store.
        XMFLOAT3 store;
        XMStoreFloat3(&store, downWithTime);

        // Move absolutely.
        transform.MoveAbsolute(store);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    // Collect the mouse down input data and use it to
    // rotate the camara transform. */
    if (Input::MouseLeftDown())
    {
        // Get the mouse cursor x and y position data.
        float mouseCursorX = Input::GetMouseXDelta() * 0.1f;
        float mouseCursorY = Input::GetMouseYDelta() * 0.1f;

        // Set the camera rotation the camara relatively to the mouse position.
        // Get the camera rotation
        XMFLOAT3 cameraRotation = transform.GetPitchYawRoll();

        // Create a x and y rot that updates the values of the camera rotation.
        float xRot = cameraRotation.x;
        float yRot = cameraRotation.y;

        // Update the rotation by the cursor x and y values with mouse look speed.
        xRot -= mouseCursorX * mouseLookSpeed;
        yRot += mouseCursorY * mouseLookSpeed;

        // Clamp the xRot to -PI/ 2 to PI / 2.
        float halfPi = XM_PIDIV2;

        // Clamp xrot so it does not move to the left infinitely.
        if (xRot > halfPi)
        {
            // Set x rot to 1/2 pi.
            xRot = halfPi;
        }
        
        // If x rot is less than -half Pi.
        if (xRot < -halfPi)
        {
            // Set it to -90.
            xRot = -halfPi;
        }

        // Set the camera new mouse rotation.
        transform.SetRotation(xRot, yRot, 0.0f);

        // Set the hasViewChange to false.
        hasViewChanged = true;
    }

    // If view has changed.
    if (hasViewChanged)
    {
        // Update the camera view matrix.
        UpdateViewMatrix();

        // Set the hasViewChange to false.
        hasViewChanged = false;
    }
}

float Camera::GetFov()
{
    return fov;
}

float Camera::GetPerspective()
{
    return isPerspective;
}

Transform& Camera::GetTransform()
{
    return transform;
}
