#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory>

#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Use a namespace for the library.
using namespace DirectX;

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int* indices, int numberOfVerticies, int numberOfIndices);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetVertexCount();
	int GetIndexCount();
	void Draw();

private:
	// Buffer to hold graphic geomentry data for this mesh.
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Create a unique pointer for vertices and indicies buffer count.
	std::unique_ptr<unsigned int> vertexCount;
	std::unique_ptr<unsigned int> indexCount;
};

