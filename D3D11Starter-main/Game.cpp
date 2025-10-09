#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Include Mesh class.
#include "Mesh.h"
#include <memory>
#include <vector>

// Include buffer struct header.
#include "BufferStructs.h"
#include "Transform.h"

// Include entity class.
#include "Entity.h"

#include <DirectXMath.h>

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	const std::wstring ps = L"PixelShader.cso";
	const std::wstring debugUVShader = L"DebugUVsPS.cso";
	const std::wstring debugNormalShader = L"DebugNormalsPS.cso";
	const std::wstring customPShader = L"CustomPS.cso";

	// Create material class to hold vertex & input shaders with the input layout.
	pShader = std::make_shared<Material>(vertexShader, pixelShader, inputLayout, ps, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	materialForShaders1 = std::make_shared<Material>(vertexShader, debugNormalsPS, inputLayout, debugNormalShader, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	materialForShaders2 = std::make_shared<Material>(vertexShader, debugUVsPS, inputLayout, debugUVShader, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	customMaterialForShaders = std::make_shared<Material>(vertexShader, customPS, inputLayout, customPShader, XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f));

	// Make material directly:
	//Material material1(vertexShader, pixelShader, inputLayout, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	//testMaterial = Material(vertexShader, pixelShader, inputLayout, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later.
	//LoadShaders();

	// Add all the materials to a material vector.
	listOfMaterials.push_back(customMaterialForShaders);
	listOfMaterials.push_back(pShader);
	listOfMaterials.push_back(materialForShaders1);
	listOfMaterials.push_back(materialForShaders2);

	// Create a for loop that loads all the materials to:
	// Load their unique shader hlsl.
	// Create the same vertex shader and unique pixel buffer for the material.
	for (int i = 0; i < listOfMaterials.size(); i++)
	{
		listOfMaterials[i]->LoadVertexShader();
		listOfMaterials[i]->LoadPixelShader();
	}
	
	// Load the material for shaders1 vertices.
	//listOfMaterials[1].get()->LoadVertexShader();
	//listOfMaterials[1].get()->LoadPixelShader();

	//// Load material 2.
	//materialForShaders2.get()->SetVertexShader(listOfMaterials[1].get()->GetVertexShader());
	//materialForShaders2.get()->SetPixelShader(listOfMaterials[1].get()->GetPixelShader());
	//materialForShaders2.get()->SetInputlayout(listOfMaterials[1].get()->GetInputLayout());

	//// Load custom material for shaders.
	//customMaterialForShaders.get()->SetVertexShader(listOfMaterials[1].get()->GetVertexShader());
	//customMaterialForShaders.get()->SetPixelShader(listOfMaterials[1].get()->GetPixelShader());
	//customMaterialForShaders.get()->SetInputlayout(listOfMaterials[1].get()->GetInputLayout());

	CreateGeometry();

	// Initialize count to 0.
	count = 0;

	// Intialize the current and previous background & border color.
	//previousBgColor = new float[4] { 0.0f, 0.0f, 0.0f, 0.0f };
	bgColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	//previousBorderColor = new float[4];
	borderColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	// Set the color picker to defualt .f values.
	// Convert the float value to colors between 0 - 1.
	// By normalization.
	colorPicker =  XMFLOAT4(
		(233.0f / 255.0f), 
		(188.0f / 255.0f), 
		(188.0f / 255.0f), 
		(0.0f / 255.0f));

	// Set boolean variables to false by casting.
	drawBgColor = false;
	drawBorderColor = false;

	// Set the demo window to true as defualt.
	showDemoWindow = false;

	// Initialize the camera class with the window aspect ratio.
	float aspectRatio = Window::AspectRatio();
	XMFLOAT3 startingPoint = XMFLOAT3(0.0f, 6.0f, -30.0f);
	XMFLOAT3 startingOrientation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fov = XM_PIDIV4;
	float nearClip = 0.1f;
	float farClip = 1000.f;
	float cameraMovementSpeed = 2.0f;
	float cameraMouseLookSpeed = 0.02f;
	bool isCameraPerspective = true;

	// Create camera 1.
	camera1 = std::make_shared<Camera>
		(aspectRatio, startingPoint, startingOrientation, fov, nearClip,
			farClip, cameraMovementSpeed, cameraMouseLookSpeed, isCameraPerspective);

	// Create camera 2.
	XMFLOAT3 startingPoint2 = XMFLOAT3(0.0f, 6.0f, -30.0f);
	XMFLOAT3 startingOrientation2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float fov2 = XM_PI / 6;
	camera2 = std::make_shared<Camera>(
		aspectRatio, startingPoint2, startingOrientation2, fov2, nearClip,
		farClip, cameraMovementSpeed, cameraMouseLookSpeed, isCameraPerspective);

	// Add the cameras to the camera.
	cameras.push_back(camera1);
	cameras.push_back(camera2);

	// Intialize the swap camera to false.
	swapCamera = false;

	// If the swap camera is false set the active camera to camara 2.
	activeCamera = camera2;

	// Create a unique float pointer for the colour tint and offset data.
	colorData = std::make_unique<float[]>(4);
	colorData[0] = 1.0f;
	colorData[1] = 0.0f;
	colorData[2] = 0.0f;
	colorData[3] = 1.0f;

	offsetData = std::make_unique<float[]>(3);
	offsetData[0] = 0.0f;
	offsetData[1] = 0.0f;
	offsetData[2] = 0.0f;

	// Initialize the constant buffer.
	// Get the data size of the constant buffer struct for to create a constant
	// buffer in memory.
	unsigned int dataSize = sizeof(BufferStructs);

	// Adjust the size to always be a multiple of 16.
	dataSize = ((dataSize + 15) / 16) * 16;

	// Create a set of instructions that describes constant buffer object.
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = dataSize;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;

	// Create the constant buffer.
	Graphics::Device->CreateBuffer(&cbDesc, 0, constantBuffer.GetAddressOf());

	// Bind the constant buffer to set it active.
	Graphics::Context->VSSetConstantBuffers(
		0,
		1,
		constantBuffer.GetAddressOf());

	// -------------------------------------------------------------------------
	// Initialize the pixel shader constant buffer.
	// Get the data size of the constant buffer struct for to create a constant
	// buffer in memory.
	unsigned int psdataSize = sizeof(PixelDataStruct);

	// Adjust the size to always be a multiple of 16.
	psdataSize = ((dataSize + 15) / 16) * 16;

	// Create a set of instructions that describes constant buffer object.
	D3D11_BUFFER_DESC pssDesc = {};
	pssDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pssDesc.ByteWidth = psdataSize;
	pssDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pssDesc.Usage = D3D11_USAGE_DYNAMIC;

	// Create the pixel shader constant buffer.
	Graphics::Device->CreateBuffer(&pssDesc, 0, psConstantBuffer.GetAddressOf());

	// Bind the pixel shader constant buffer to set it active.
	// using Context->PSSetConstantBuffers.
	Graphics::Context->PSSetConstantBuffers(
		0,
		1,
		psConstantBuffer.GetAddressOf());

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		//Graphics::Context->IASetInputLayout(inputLayout.Get());

		// Using the new material for shaders set the input laout.
		//Graphics::Context->IASetInputLayout(materialForShaders1.get()->GetInputLayout().Get());

		// Set the active vertex and pixel shaders
		//  - Once you start applying different shaders to different objects,
		//    these calls will need to happen multiple times per frame
		//Graphics::Context->VSSetShader(vertexShader.Get(), 0, 0);
		//Graphics::Context->PSSetShader(pixelShader.Get(), 0, 0);
		// Using the material for shaders, set the shaders.
		//Graphics::Context->VSSetShader(materialForShaders1.get()->GetVertexShader().Get(), 0, 0);
		//Graphics::Context->PSSetShader(materialForShaders1.get()->GetPixelShader().Get(), 0, 0);
	}
}


// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Game::Initialize()
{
	// Initialize ImGui once using a count pointer.
	if (count == 0)
	{
		// Initialize ImGui itself & platform/renderer backends
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Window::Handle());
		ImGui_ImplDX11_Init(Graphics::Device.Get(), Graphics::Context.Get());

		// Pick a style (uncomment one of these 3)
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();

		// Increase the count by one.
		(count)++;
	}
}

void Game::updateHelper()
{
	// Feed fresh data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = ImGui::GetIO().DeltaTime;
	io.DisplaySize.x = (float)Window::Width();
	io.DisplaySize.y = (float)Window::Height();

	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Determine new input capture
	Input::SetKeyboardCapture(io.WantCaptureKeyboard);
	Input::SetMouseCapture(io.WantCaptureMouse);

	// If boolean variable is true, show the demo window, else don't show window.
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow();
	}
}

void Game::buildImGuiCustomizedUI()
{
	// Get the style of the ImGui.
	ImGuiStyle& style = ImGui::GetStyle();

	// Get the background & border vector float color values from the style.
	ImVec4 bg = style.Colors[ImGuiCol_WindowBg];
	ImVec4 border = style.Colors[ImGuiCol_Border];

	// Create a new window.
	ImGui::Begin("Customized UI window");

	// Create an window app detail.
	// Create a collapsing header named [ Application Information ].
	if(ImGui::TreeNode("Application Information"))
	{
		// Get the frame rate of the application and display it.
		ImGui::Text("Framerate: %.6f fps", ImGui::GetIO().Framerate);

		// Create another tree node to change the background color and window color.
		// To change background color and window color.
		// Get the window width and height and display it.
		ImGui::Text("Window Client Width: %d", Window::Width());
		ImGui::Text("Window Client Height: %d", Window::Height());

		// Create tree node for changing the window color using color picker.
		if (ImGui::TreeNode("Change window background color"))
		{
			/* Create a float array(which is a non - static member pointer within this scope).
			 * To get the changed float(*) values in colouredit4 using an if statement if a
			 * a color is changed. */
			float color[4] = { colorPicker.x, colorPicker.y, colorPicker.z, colorPicker.w };

			// Create a new window color for the app using an if color edit4 statement.
			if (ImGui::ColorEdit4("Window Color", color))
			{
				// Convert the float pointer to Xmfloat.
				colorPicker = XMFLOAT4(color[0], color[1], color[2], color[3]);
			}

			// Create a tree pop to end node if closed
			ImGui::TreePop();
		}

		// Create a show/hidden demo window button.
		if (ImGui::Button("Show ImGui Demo Window"))
		{
			// Toggle the boolean variable to true or false.
			showDemoWindow = !showDemoWindow;
		}

		// Create tree pop to close the collapsing header tree node.
		ImGui::TreePop();
	}

	// Create a mini game that uses ImGui buttons to change a float variable.
	// The float variable drops with time and the user can click buttons.
	// If the user is fast enough to click the buttons. The customized UI window appears.
	// Create a static float variable to hold the value number of button clicked.
	static float value = 0.0f;
	static bool showTestWindow = false;

	// Create a single struct variable in which its members can be changed.
	//BufferStructs shaderDataChange = {};

	// If showTestWindow is false, run the mini game.
	if (!showTestWindow)
	{
		// If value is less than 10.0f.
		if (value < 4.0f)
		{
			// If the value is less than 0, set it to 0.
			if (value < 0.0f)
			{
				value = 0.0f;
			}

			// Create a button called Click me to unlock the customized UI window.
			if (ImGui::Button("Test I: Click as fast as you can to unlock the Edit Custom ImGUI UI - Test II & III"))
			{
				// Increase the value by 0.5f.
				value += 0.5f;

				// If the value is >= 4.0f, set the window to true.
				if(value >= 4.0f)
				{
					// Set showTestWindow to true.
					showTestWindow = true;
				}
			}

			// Reduce the value by every milliseconds.
			value -= ImGui::GetIO().DeltaTime;
		}
	}

	// Draw a pictograph of the value using a histogram.
	ImGui::PlotHistogram("<-- Clicker: Test I", &value, 1, 0, nullptr, 0.0f, 4.0f, ImVec2(0, 100));

	// If test window is true, show the test window.
	if (showTestWindow)
	{
		// Create another tree node to change the custom UI background & Outline color.
		if (ImGui::TreeNode("[Unlocked] - Edit Custom ImGUI UI Test"))
		{
			// Create a UI color node.
			if (ImGui::TreeNode("Background UI Color - Test II"))
			{
				// Create a scope member non-static float array variable.
				float color[4] = { bgColor.x, bgColor.y, bgColor.z, bgColor.w };

				// Use if color edit4 to actively change the bg color values.
				if (ImGui::ColorEdit4("Background Color", color))
				{
					// Convert the changed color to xmfloat4 values for bgcolor.
					bgColor = XMFLOAT4(color[0], color[1], color[2], color[3]);
				}

				// If the apply background color is preessed.
				if (ImGui::Button("Apply new background color"))
				{
					// Apply the new background color to the ImGui style.
					style.Colors[ImGuiCol_WindowBg] = ImVec4(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
				}

				// Create a tree pop to close child header tree node.
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("UI Border Color - Test III"))
			{
				// Create scope float array for color to be changed to the prevoius array.
				float color[4] = { borderColor.x, borderColor.y, borderColor.z, borderColor.w };

				// Use color edit4 to get and change the border color values
				if(ImGui::ColorEdit4("Border Color", color))
				{
					// Convert changed color values to bgcolor xmfloat values.
					borderColor = XMFLOAT4(color[0], color[1], color[2], color[3]);
				}

				// If the apply border color is pressed.
				if (ImGui::Button("Apply new border color"))
				{
					// Apply the new border color to the ImGui style.
					style.Colors[ImGuiCol_Border] = ImVec4(borderColor.x, borderColor.y, borderColor.z, borderColor.w);
				}

				// Create a tree pop to close child header tree node.
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("PlotLines of Framerate - Test IV"))
			{
				// Create static variables to get the frame rate.
				static const int frameArraySize = 120;
				static float frameRateHolder[frameArraySize] = { 0 };
				static int currentFrame = 0;

				// Get the current frame rate and store it in the array.
				// I do not use average frame rate due to reducing the performance of the application.
				frameRateHolder[(currentFrame % frameArraySize)] = ImGui::GetIO().Framerate;
				currentFrame = (currentFrame + 1) % frameArraySize;

				// Use Lines to visualize frame rate.
				ImGui::PlotLines("<--- Frame Rate: Test IV", frameRateHolder, frameArraySize, currentFrame, nullptr, 0.0f, 120.0f, ImVec2(0, 60));
				ImGui::TreePop();
			}

			// Create a tree pop to close child header tree node.
			ImGui::TreePop();
		}
	}

	// Create Tree node for Meshes.
	/*if (ImGui::TreeNode("Mesh"))
	{
		if (ImGui::TreeNode("Triangle"))
		{
			ImGui::Text("Triangle: %d", triangle->GetIndexCount() / 3);
			ImGui::Text("Vertices: %d", triangle->GetVertexCount());
			ImGui::Text("Indices: %d", triangle->GetIndexCount());
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Right-angle Triangle"))
		{
			ImGui::Text("Triangle: %d", rightTriangle->GetIndexCount() / 3);
			ImGui::Text("Vertices: %d", rightTriangle->GetVertexCount());
			ImGui::Text("Indices: %d", rightTriangle->GetIndexCount());
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Diamond square"))
		{
			ImGui::Text("Triangle: %d", square->GetIndexCount() / 3);
			ImGui::Text("Vertices: %d", square->GetVertexCount());
			ImGui::Text("Indices: %d", square->GetIndexCount());
			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}*/

	// Create new menu for entity tracking.
	if (ImGui::TreeNode("Entities"))
	{
		// Create a loop of entities to be displayed.
		for (int i = 0; i < listOfEntities.size(); i++)
		{
			std::string number = "Entity " + std::to_string(i + 1);

			if (ImGui::TreeNode(number.c_str()))
			{
				// Get the transform address to get its data.
				Transform& entityTransform = listOfEntities[i].GetTransform();

				// Create Push ID to make an edited variable of transform 
				// element to be unique to each entity for each loop.
				ImGui::PushID(i);

				// Create the xmfloat3 data of position, scale and rotation.
				XMFLOAT3 position = XMFLOAT3(entityTransform.GetPosition());
				XMFLOAT3 scale = XMFLOAT3(entityTransform.GetScale());
				XMFLOAT3 rotation = XMFLOAT3(entityTransform.GetPitchYawRoll());

				// Create a drag float that chages the transformation of the entities.
				ImGui::DragFloat3("Position", &position.x, 0.1f);
				ImGui::DragFloat3("Scale", &scale.x, 0.1f);
				ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);

				// Set the tranform data to the new values.
				entityTransform.SetPosition(position);
				entityTransform.SetRotation(rotation);
				entityTransform.SetScale(scale);

				// Remove the Id after loop ends and changes are made.
				ImGui::PopID();

				// Pop the tree.
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("Swap Active Camera"))
	{
		XMFLOAT3 position = activeCamera.get()->GetTransform().GetPosition();
		
		// Get string value of the float.
		std::string stringPos = "Position: " +
			std::to_string(position.x) + "f, " +
			std::to_string(position.y) + "f, " +
			std::to_string(position.z) + "f";
		

		// Convert bool to string.
		std::string trueOrfalse = " ";
		if (activeCamera.get()->GetPerspective())
		{
			trueOrfalse = "True";
		}
		else
		{
			trueOrfalse = "False";
		}

		ImGui::Text("Active Camera Details:");
		ImGui::Text("%s", stringPos.c_str());
		ImGui::Text("Fov: %f", activeCamera.get()->GetFov());
		ImGui::Text("IsPerspective: %s", trueOrfalse.c_str());


		// If swapbutton Previous is tapped.
		if (ImGui::Button("Previous"))
		{
			// Set the swap Camera variable to false.
			this->swapCamera = false;
		}

		// If swapbutton Next is tapped.
		if (ImGui::Button("Next"))
		{
			// Set the swap Camera variable to true.
			this->swapCamera = true;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Constant Buffer Data Changes"))
	{
		if (ImGui::TreeNode("Vertices offset"))
		{
			// Get the float[] data content with Sliderfloat3().
			ImGui::SliderFloat3("Vertices Offset", offsetData.get(), -1.0f, 1.0f);
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Vertices Color"))
		{
			// Edit the color data of the buffer shader and update the unique ptr data.
			ImGui::ColorEdit4("Vertices Color", colorData.get());
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	// End the created window.
	ImGui::End();
}

void Game::SwapCamera(bool swapCamera)
{
	// If number of camera is less than 0 end method.
	if (cameras.empty())
	{
		return;
	}

	if (swapCamera)
	{
		// Set active camera to camera2.
		activeCamera = cameras[1];
	}
	else
	{
		// Set active camera to camera 1.
		activeCamera = cameras[0];
	}

	//// Create int for cameta index.
	//int cameraIndex = 0;

	//// Use a for loop to check the current index of the camera.
	//for (int x = 0; x < cameras.size(); x++)
	//{
	//	// If the active camera is the same as the camera index.
	//	if (activeCamera == cameras[x])
	//	{
	//		// Camera index is set loop number.
	//		cameraIndex = x;
	//	}
	//}

	//if (swapCamera)
	//{
	//	// Set active camera to camera2.
	//	int cameraNumber = cameraIndex + 1;

	//	if (cameraNumber >= cameras.size())
	//	{
	//		cameraNumber = 0;
	//	}

	//	activeCamera = cameras[cameraNumber];
	//}
	//else
	//{
	//	// Set active camera to camera2.
	//	int cameraNumber = cameraIndex - 1;

	//	if (cameraNumber < 0)
	//	{
	//		cameraNumber = cameras.size() - 1;
	//	}

	//	// Set active camera to camera 1.
	//	activeCamera = cameras[cameraNumber];
	//}
}



// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
//void Game::LoadShaders()
//{
//	// BLOBs (or Binary Large OBjects) for reading raw data from external files
//	// - This is a simplified way of handling big chunks of external data
//	// - Literally just a big array of bytes read from a file
//	ID3DBlob* pixelShaderBlob;
//	ID3DBlob* vertexShaderBlob;
//
//	// Loading shaders
//	//  - Visual Studio will compile our shaders at build time
//	//  - They are saved as .cso (Compiled Shader Object) files
//	//  - We need to load them when the application starts
//	{
//		// Read our compiled shader code files into blobs
//		// - Essentially just "open the file and plop its contents here"
//		// - Uses the custom FixPath() helper from Helpers.h to ensure relative paths
//		// - Note the "L" before the string - this tells the compiler the string uses wide characters
//		D3DReadFileToBlob(FixPath(L"PixelShader.cso").c_str(), &pixelShaderBlob);
//		D3DReadFileToBlob(FixPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);
//
//		// Create the actual Direct3D shaders on the GPU
//		Graphics::Device->CreatePixelShader(
//			pixelShaderBlob->GetBufferPointer(),	// Pointer to blob's contents
//			pixelShaderBlob->GetBufferSize(),		// How big is that data?
//			0,										// No classes in this shader
//			pixelShader.GetAddressOf());			// Address of the ID3D11PixelShader pointer
//
//		Graphics::Device->CreateVertexShader(
//			vertexShaderBlob->GetBufferPointer(),	// Get a pointer to the blob's contents
//			vertexShaderBlob->GetBufferSize(),		// How big is that data?
//			0,										// No classes in this shader
//			vertexShader.GetAddressOf());			// The address of the ID3D11VertexShader pointer
//	}
//
//
//	// Create an input layout 
//	//  - This describes the layout of data sent to a vertex shader
//	//  - In other words, it describes how to interpret data (numbers) in a vertex buffer
//	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
//	//  - Luckily, we already have that loaded (the vertex shader blob above)
//	{
//		D3D11_INPUT_ELEMENT_DESC inputElements[2] = {};
//
//		// Set up the first element - a position, which is 3 float values
//		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
//		inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
//		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element
//
//		// Set up the second element - a color, which is 4 more float values
//		inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 4x 32-bit floats
//		inputElements[1].SemanticName = "COLOR";							// Match our vertex shader input!
//		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element
//
//		// Create the input layout, verifying our description against actual shader code
//		Graphics::Device->CreateInputLayout(
//			inputElements,							// An array of descriptions
//			2,										// How many elements in that array?
//			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
//			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
//			inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
//	}
//}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	/*XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);*/

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in CPU memory
	//    over to a Direct3D-controlled data structure on the GPU (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself

	// .. / .. /
	// Create meshes using the imported .obj data.
	cube = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/cube.obj").c_str());
	cylinder = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/cylinder.obj").c_str());
	helix = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/helix.obj").c_str());
	quad = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/quad.obj").c_str());
	quad_Double_Sided = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/quad_double_sided.obj").c_str());
	sphere = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/sphere.obj").c_str());
	torus = std::make_shared<Mesh>(FixPath("../../Assets/Meshes/torus.obj").c_str());

	// Create entities using the first list of materials.
	entity1 = Entity(*(cube.get()), listOfMaterials[0]);
	entity2 = Entity(*(cylinder.get()), listOfMaterials[0]);
	entity3 = Entity(*(helix.get()), listOfMaterials[0]);

	// Create two entities that have the same shared mesh as entity3
	entity4 = Entity(*(quad.get()), listOfMaterials[0]);
	entity5 = Entity(*(quad_Double_Sided.get()), listOfMaterials[0]);
	entity6 = Entity(*(sphere.get()), listOfMaterials[0]);
	entity7 = Entity(*(torus.get()), listOfMaterials[0]);

	// Use mesh to create and push in three distinct entities with different meshes.
	// Create a for loop the that push the entities 3 times.
	for (int x = 0; x < 4; x++)
	{
		// Create a shared pointer material that holds a different material for each loop.
		std::shared_ptr<Material> loopMaterial;

		if (x == 1)
		{
			loopMaterial = listOfMaterials[x];
		}

		if (x == 2)
		{
			loopMaterial = listOfMaterials[x];
		}

		if (x == 3)
		{
			loopMaterial = listOfMaterials[x];
		}

		// If the loop is greater than 0 *and less than 3*.
		if (x > 0)
		{
			// Set loop material type for each group of looped entities.
			// Create entities.
			entity1.SetMaterial(loopMaterial);
			entity2.SetMaterial(loopMaterial);
			entity3.SetMaterial(loopMaterial);
			entity4.SetMaterial(loopMaterial);
			entity5.SetMaterial(loopMaterial);
			entity6.SetMaterial(loopMaterial);
			entity7.SetMaterial(loopMaterial);
		}
		
		
		// Push the entities with a changed material in the list.
		listOfEntities.push_back(entity7);
		listOfEntities.push_back(entity6);
		listOfEntities.push_back(entity5);
		listOfEntities.push_back(entity4);
		listOfEntities.push_back(entity3);
		listOfEntities.push_back(entity2);
		listOfEntities.push_back(entity1);
	}

	// For each entities group in the list, transform their y position by 3.
	for (int i = 0; i < 4; i++)
	{
		// Get the index of the entities.
		int indexMultiple = 7 * i;

		// Move the y of the enties position by 4 each time.
		float y = static_cast<float>(4 * i);

		// Transform the meshes position to their new position.
		listOfEntities[0 + indexMultiple].GetTransform().SetPosition(-9, y, 0);
		listOfEntities[1 + indexMultiple].GetTransform().SetPosition(-6, y, 0);
		listOfEntities[2 + indexMultiple].GetTransform().SetPosition(-3, y, 0);
		listOfEntities[3 + indexMultiple].GetTransform().SetPosition(0, y, 0);
		listOfEntities[4 + indexMultiple].GetTransform().SetPosition(3, y, 0);
		listOfEntities[5 + indexMultiple].GetTransform().SetPosition(6, y, 0);
		listOfEntities[6 + indexMultiple].GetTransform().SetPosition(9, y, 0);
	}
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Update the matrix of each camera.
	// if the camera pointer is not null.
	if (cameras.size() <= 0)
	{
		for (int i = 0; i < cameras.size(); i++)
		{
			// Update the update projection matrix with the new window aspect.
			float aspectRatio = Window::AspectRatio();
			cameras[i].get()->UpdateProjectionMatrix(aspectRatio);
		}
	}

	// if the camera pointer is not null.
	if (activeCamera != nullptr)
	{
		// Update the update projection matrix with the new window aspect.
		float aspectRatio = Window::AspectRatio();
		activeCamera.get()->UpdateProjectionMatrix(aspectRatio);
	}
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();

	// Call the update helper.
	updateHelper();

	// Call the Build UI update helper.
	buildImGuiCustomizedUI();

	// SwapCamera();Call the swap camera function.
	SwapCamera(this->swapCamera);

	// Pass in the total time:
	tTime = totalTime;

	//// Make an oscilating number.
	//float wave = static_cast<float>(sin(totalTime * 4) * 0.5 + 2.0f);
	//
	//// Set entity 1 transform scale to back to 1 each time to reset scale.
	//// This will work but it make this a bit slow. 
	////listOfEntities[4].GetTransform().SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

	//// Make entity 1 scale up and down. Just set scale directly.
	//listOfEntities[4].GetTransform().SetScale(XMFLOAT3(wave, wave, wave));

	//float wave2 = static_cast<float>(sin(totalTime * 4) * 0.5 + 0.0f);
	//// Make entity 4 and 5 move back and forth each time.
	//listOfEntities[0].GetTransform().SetPosition(XMFLOAT3(wave2, 0.0f, 0.0f));
	//listOfEntities[1].GetTransform().SetPosition(XMFLOAT3(-wave2, 0.0f, 0.0f));

	//// Rotate the third square with time on its z axis.
	//listOfEntities[2].GetTransform().Rotate(XMFLOAT3(0.0f, 0.0f, static_cast<float>(deltaTime * 3.5)));

	// Update the input and view matrix camera each frame.
	// Get update the active camera each time.
	activeCamera.get()->Update(deltaTime);
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erase what's on screen) and depth buffer
		// Use the color picker values to create a float array.
		float color[4] = { colorPicker.x, colorPicker.y, colorPicker.z, colorPicker.w };

		Graphics::Context->ClearRenderTargetView(Graphics::BackBufferRTV.Get(),	color);
		Graphics::Context->ClearDepthStencilView(Graphics::DepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// Use a for each to draw the mesh.
	for (int i = 0; i < listOfEntities.size(); i++)
	{
		// Create a psConstantBuffer using the pixel shader struct.
		PixelDataStruct psCB1 = {};

		// Set the color tint of pixel shader cbuffer to the material color.
		psCB1.colorTint = listOfEntities[i].GetMaterial().get()->GetColorTint();
		psCB1.time = DirectX::XMFLOAT2(tTime, tTime);

		// Map out or get the data of the constant buffer to pause data use and
		// address moving in the GPU.
		D3D11_MAPPED_SUBRESOURCE mappedPSBuffer = {};
		Graphics::Context->Map(
			psConstantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedPSBuffer
		);

		// Copy the new struct data into the constant buffer with the approximate size.
		memcpy(mappedPSBuffer.pData, &psCB1, sizeof(psCB1));

		// Unmap or realease the address of the constant buffer for the GPU to use and
		// move the files if necessary.
		Graphics::Context->Unmap(psConstantBuffer.Get(), 0);

		// ---------------------------------------------------------------------------------------

		// Create two new variables that hold the new struct data for the constant buffer.
		// Using the buffer struct model.
		BufferStructs cbStruct = {};

		// Get the transform class world matrix.
		XMFLOAT4X4 entityTransformWorldMatrix = listOfEntities[i].GetTransform().GetWorldMatrix();

		// Store the loaded SIMD identity matrix of the transform class to the world matrix.
		//XMStoreFloat4x4(&worldMatrix, XMLoadFloat4x4(&entityTranform.GetWorldMatrix()));

		// Create a color tint.
		// cbStruct.colorTint = listOfEntities[i].GetMaterial().get()->GetColorTint();

		// Store the SIMD identity matrix to the world matrix.
		cbStruct.worldMatrix = XMLoadFloat4x4(&entityTransformWorldMatrix);

		// Get the view and projection matrix of our camera and set it to the constant buffer.
		XMFLOAT4X4 cameraViewMatrix = activeCamera.get()->GetViewMatrix();
		cbStruct.viewMatrix = XMLoadFloat4x4(&cameraViewMatrix);

		XMFLOAT4X4 cameraProjectionMatrix = activeCamera.get()->GetProjectionMatrix();
		cbStruct.projectionMatrix = XMLoadFloat4x4(&cameraProjectionMatrix);

		// Map out or get the data of the constant buffer to pause data use and
		// address moving in the GPU.
		D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
		Graphics::Context->Map(
			constantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedBuffer
		);

		// Copy the new struct data into the constant buffer with the approximate size.
		memcpy(mappedBuffer.pData, &cbStruct, sizeof(cbStruct));

		// Unmap or realease the address of the constant buffer for the GPU to use and
		// move the files if necessary.
		Graphics::Context->Unmap(constantBuffer.Get(), 0);

		// Draw the entities after their world matrix have be updated in the vertex shader
		// using the constant shader.
		listOfEntities[i].Draw();
	}

	// Call the triangle mesh draw.
	//square->Draw();
	//rightTriangle->Draw();
	//triangle->Draw();


	// Tells Imgui to gets its buffer data information and feed the data to another funtion.
	{
		ImGui::Render(); // Turns this frame’s UI into renderable triangles
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Draws it to the screen
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present at the end of the frame
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Re-bind back buffer and depth buffer after presenting
		Graphics::Context->OMSetRenderTargets(
			1,
			Graphics::BackBufferRTV.GetAddressOf(),
			Graphics::DepthBufferDSV.Get());
	}
}



