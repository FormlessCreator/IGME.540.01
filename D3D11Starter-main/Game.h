#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <DirectXMath.h>

// Include the mesh class.
#include "Mesh.h"

// Include entity class.
#include "Entity.h"

// Add a camera class.
#include "Camera.h"

// Include a material class.
#include "Material.h"

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

	// Create helper for swaping camera.
	void SwapCamera(bool x);

	// Create a method that applies background & border color for the UI helper.
	// void applyUIColor();

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	//void LoadShaders();
	void CreateGeometry();

	// Create a count pointer for ImGui initialization.
	int count;

	// Create xmfloat arrays for background and border color.
	//float* previousBgColor;
	XMFLOAT4 bgColor;
	XMFLOAT4 borderColor;

	// Create color picker for window.
	XMFLOAT4 colorPicker;

	// Create boolean variables to draw the background & border color window.
	bool drawBgColor;
	bool drawBorderColor;

	// Variable to show the demo window.
	bool showDemoWindow;

	// Create unique pointer for the color and offset data off the constant buffer struct.
	std::unique_ptr<float[]> colorData;
	std::unique_ptr<float[]> offsetData;

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr


	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// Create 3 pixel shader that uses the uv data, normal data and a custom pixel shader.
	Microsoft::WRL::ComPtr<ID3D11PixelShader> debugUVsPS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> debugNormalsPS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> customPS;

	// Shaders and shader-related constructs will be now placed in the material class:
	// Create a shared pointer for material.
	std::shared_ptr<Material> pShader;
	std::shared_ptr<Material> materialForShaders1;
	std::shared_ptr<Material> materialForShaders2;
	std::shared_ptr<Material> customMaterialForShaders;

	// Create a material vector list to hold created shared pointer materials.
	std::vector <std::shared_ptr<Material>> listOfMaterials;

	// Create a test material that is not shared.
	//Material testMaterial;

	// Create a constant buffer.
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	// Create a pixel constant buffer using the pixeldata struct.
	Microsoft::WRL::ComPtr<ID3D11Buffer> psConstantBuffer;

	// Create a shared pointer for the list triangles.
	/*std::shared_ptr<Mesh> triangle;
	std::shared_ptr<Mesh> square;
	std::shared_ptr<Mesh> rightTriangle;*/

	// Create shared meshes for the .obj files.
	std::shared_ptr<Mesh> cube;
	std::shared_ptr<Mesh> cylinder;
	std::shared_ptr<Mesh> helix;
	std::shared_ptr<Mesh> quad;
	std::shared_ptr<Mesh> quad_Double_Sided;
	std::shared_ptr<Mesh> sphere;
	std::shared_ptr<Mesh> torus;

	// Create an five entity.
	Entity entity1;
	Entity entity2;
	Entity entity3;
	Entity entity4;
	Entity entity5;
	Entity entity6;
	Entity entity7;
	
	// Create a list of entities.
	std::vector<Entity> listOfEntities;

	// Add a shared ptr Camera Class.
	std::shared_ptr<Camera> camera1;
	std::shared_ptr<Camera> camera2;

	// Create a vector of shared camera pointers.
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
	bool swapCamera;

	// Get the current total time.
	float tTime;
};

