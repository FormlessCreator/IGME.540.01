#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <memory>

// Include the mesh class.
#include "Mesh.h"

class Game
{
public:
	// Basic OOP setup
	Game();
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Intialize the ImGui first.
	void Initialize();

	// Create a helper method that helps update get data and create an ImGui window.
	void updateHelper();

	// Create a build UI update helper method to build a customized game UI.
	void buildImGuiCustomizedUI();

	// Create a method that applies background & border color for the UI helper.
	// void applyUIColor();

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders();
	void CreateGeometry();

	// Create a count pointer for ImGui initialization.
	int* count;

	// Create float arrays for background and border color.
	//float* previousBgColor;
	float* bgColor;
	//float* previousBorderColor;
	float* borderColor;

	// Create color picker for window.
	float* colorPicker;

	// Create boolean variables to draw the background & border color window.
	bool* drawBgColor;
	bool* drawBorderColor;

	// Variable to show the demo window.
	bool* showDemoWindow;

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// Create a shared pointer for the list triangles.
	std::shared_ptr<Mesh> triangle;
	std::shared_ptr<Mesh> square;
	std::shared_ptr<Mesh> rightTriangle;
};

