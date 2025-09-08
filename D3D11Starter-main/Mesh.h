#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include <DirectXMath.h>

// Use a namespace for the library.
using namespace DirectX;

class Mesh
{
public:
	Mesh();
	~Mesh();
	void CreateGeometry();

private:

};

