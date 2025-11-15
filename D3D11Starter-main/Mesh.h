#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

// Add fstream and stdexcept for object character file loading.
#include <fstream>
#include <stdexcept>

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

	// Create a second overload constructor for mesh that accepts the file name to load mesh v, vt, vn & f information.
	Mesh(const char* name);

	// Other required mesh data information.
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetVertexCount();
	int GetIndexCount();

	// Add a method to create the tangent U texture for the geometry.
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);

	void Draw();

private:
	// Buffer to hold graphic geomentry data for this mesh.
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Create a unique pointer for vertices and indicies buffer count.
	unsigned int vertexCount = 0;
	unsigned int indexCount = 0;

	// Name of the mesh.
	std::string filePath = "";
};

