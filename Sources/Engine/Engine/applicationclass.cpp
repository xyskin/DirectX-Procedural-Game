////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_TerrainShader = 0;
	m_ColorShader = 0;
	m_Light = 0;
	m_LightShader = 0;
	m_Frustum = 0;
	m_QuadTree = 0;
	m_Vehicle = 0;
	m_renderLine = false;
	m_renderMesh = false;
	m_renderUI = true;
	m_vehicleFoward = 0;
	m_vehicleBack = 0;
	m_vehicleLeft = 0;
	m_vehicleRight = 0;
	m_TextureShader = 0;
	m_MiniMap = 0;
	m_Mouse = 0;
	m_start = false;
	m_check = false;
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_MainMenuObject = 0;
	m_skydome = 0;
	m_skydomeshader = 0;

	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_ReflectionShader = 0;
	m_Water = 0;
	m_WaterShader = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;

	char videoCard[128];
	int videoMemory;
	D3DXVECTOR3 vehiclePosition;
	int terrainWidth, terrainHeight;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	//m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	//m_Camera->SetLookAt(0.0f, 0.0f, 1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 50.0f;
	cameraY = 100.0f;
	cameraZ = 50.0f;

	//m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(m_Direct3D->GetDevice(), "../Engine/data/hm.bmp", "../Engine/data/cm.bmp", 20.0f, L"../Engine/data/dirt.dds",
		L"../Engine/data/normal.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);
	m_Position->SetRotation(0.0f, 180.0f, 0.0f);
	m_Position->SetObjectParts(PositionClass::Vehicle);

	m_vehicleFoward = new PositionClass;
	if (!m_vehicleFoward)
	{
		return false;
	}
	m_vehicleBack = new PositionClass;
	if (!m_vehicleBack)
	{
		return false;
	}
	m_vehicleLeft = new PositionClass;
	if (!m_vehicleLeft)
	{
		return false;
	}
	m_vehicleRight = new PositionClass;
	if (!m_vehicleRight)
	{
		return false;
	}

	m_vehicleFoward->SetPosition(cameraX, cameraY, cameraZ - 1);
	m_vehicleFoward->SetRotation(0.0f, 180.0f, 0.0f);
	m_vehicleFoward->SetObjectParts(PositionClass::Front);
	m_vehicleBack->SetPosition(cameraX, cameraY, cameraZ + 1);
	m_vehicleBack->SetRotation(0.0f, 180.0f, 0.0f);
	m_vehicleBack->SetObjectParts(PositionClass::Back);
	m_vehicleLeft->SetPosition(cameraX + 1, cameraY, cameraZ);
	m_vehicleLeft->SetRotation(0.0f, 180.0f, 0.0f);
	m_vehicleLeft->SetObjectParts(PositionClass::Left);
	m_vehicleRight->SetPosition(cameraX - 1, cameraY, cameraZ);
	m_vehicleRight->SetRotation(0.0f, 180.0f, 0.0f);
	m_vehicleRight->SetObjectParts(PositionClass::Right);

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader) {
		return false;
	}

	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, 0.0f);

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	// Create the quad tree object.
	m_QuadTree = new QuadTreeClass;
	if (!m_QuadTree)
	{
		return false;
	}

	// Initialize the quad tree object.
	result = m_QuadTree->Initialize(m_Terrain, m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the quad tree object.", L"Error", MB_OK);
		return false;
	}

	m_Vehicle = new VehicleClass;
	if (!m_Vehicle)
	{
		return false;
	}

	result = m_Vehicle->Initialize(m_Direct3D->GetDevice(), "../Engine/data/Vehicle.txt", L"../Engine/data/Vehicle.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vehicle object.", L"Error", MB_OK);
		return false;
	}

	m_MainMenuObject = new VehicleClass;
	if (!m_MainMenuObject)
	{
		return false;
	}

	result = m_MainMenuObject->Initialize(m_Direct3D->GetDevice(), "../Engine/data/Vehicle.txt", L"../Engine/data/Vehicle.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the vehicle object.", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Get the size of the terrain as the minimap will require this information.
	m_Terrain->GetTerrainSize(terrainWidth, terrainHeight);

	// Create the mini map object.
	m_MiniMap = new MiniMapClass;
	if (!m_MiniMap)
	{
		return false;
	}

	// Initialize the mini map object.
	result = m_MiniMap->Initialize(m_Direct3D->GetDevice(), hwnd, screenWidth, screenHeight, baseViewMatrix, (float)(terrainWidth - 1),
		(float)(terrainHeight - 1));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the mini map object.", L"Error", MB_OK);
		return false;
	}

	m_Mouse = new BitmapClass;
	if (!m_Mouse)
	{
		return false;
	}

	result = m_Mouse->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/mouse.dds", 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the mouse object.", L"Error", MB_OK);
		return false;
	}

	m_skydome = new SkyDomeClass;
	if (!m_skydome)
	{
		return false;
	}

	result = m_skydome->Initialize(m_Direct3D->GetDevice(), "../Engine/data/skydome.txt");
	if (!result)
	{
		return false;
	}

	m_skydomeshader = new SkyDomeShaderClass;
	if (!m_skydomeshader)
	{
		return false;
	}

	result = m_skydomeshader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the skydome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the reflection shader object.
	m_ReflectionShader = new ReflectionShaderClass;
	if (!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the reflection shader object.
	result = m_ReflectionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the water object.
	m_Water = new WaterClass;
	if (!m_Water)
	{
		return false;
	}

	// Initialize the water object.
	result = m_Water->Initialize(m_Direct3D->GetDevice(), L"../Engine/data/waternormal.dds", 50.0f, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}
	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the quad tree object.
	if (m_QuadTree)
	{
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

	// Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	// Release the vehicle object.
	if (m_Vehicle)
	{
		m_Vehicle->Shutdown();
		delete m_Vehicle;
		m_Vehicle = 0;
	}

	if (m_MainMenuObject)
	{
		m_MainMenuObject->Shutdown();
		delete m_MainMenuObject;
		m_MainMenuObject = 0;
	}

	if (m_vehicleBack)
	{
		delete m_vehicleBack;
		m_vehicleBack = 0;
	}

	if (m_vehicleFoward)
	{
		delete m_vehicleFoward;
		m_vehicleFoward = 0;
	}

	if (m_vehicleLeft)
	{
		delete m_vehicleLeft;
		m_vehicleLeft = 0;
	}

	if (m_vehicleRight)
	{
		delete m_vehicleRight;
		m_vehicleRight = 0;
	}

	// Release the mini map object.
	if (m_MiniMap)
	{
		m_MiniMap->Shutdown();
		delete m_MiniMap;
		m_MiniMap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Mouse)
	{
		m_Mouse->Shutdown();
		delete m_Mouse;
		m_Mouse = 0;
	}

	if (m_skydome)
	{
		m_skydome->Shutdown();
		delete m_skydome;
		m_skydome = 0;
	}

	if (m_skydomeshader)
	{
		m_skydomeshader->Shutdown();
		delete m_skydomeshader;
		m_skydomeshader = 0;
	}
	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the water object.
	if (m_Water)
	{
		m_Water->Shutdown();
		delete m_Water;
		m_Water = 0;
	}

	// Release the reflection shader object.
	if (m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;

	// Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed())
	{
		return false;
	}

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Do the water frame processing.
	m_Water->Frame();


	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}


bool ApplicationClass::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);
	m_vehicleFoward->SetFrameTime(frameTime);
	m_vehicleBack->SetFrameTime(frameTime);
	m_vehicleLeft->SetFrameTime(frameTime);
	m_vehicleRight->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsLeftPressed();
	m_Position->TurnLeft(keyDown, posX, posZ);
	m_vehicleFoward->TurnLeft(keyDown, posX, posZ);
	m_vehicleBack->TurnLeft(keyDown, posX, posZ);
	m_vehicleLeft->TurnLeft(keyDown, posX, posZ);
	m_vehicleRight->TurnLeft(keyDown, posX, posZ);

	keyDown = m_Input->IsRightPressed();
	m_Position->TurnRight(keyDown, posX, posZ);
	m_vehicleFoward->TurnRight(keyDown, posX, posZ);
	m_vehicleBack->TurnRight(keyDown, posX, posZ);
	m_vehicleLeft->TurnRight(keyDown, posX, posZ);
	m_vehicleRight->TurnRight(keyDown, posX, posZ);

	keyDown = m_Input->IsUpPressed();
	m_Position->MoveForward(keyDown);
	m_vehicleFoward->MoveForward(keyDown);
	m_vehicleBack->MoveForward(keyDown);
	m_vehicleLeft->MoveForward(keyDown);
	m_vehicleRight->MoveForward(keyDown);

	keyDown = m_Input->IsDownPressed();
	m_Position->MoveBackward(keyDown);
	m_vehicleFoward->MoveBackward(keyDown);
	m_vehicleBack->MoveBackward(keyDown);
	m_vehicleLeft->MoveBackward(keyDown);
	m_vehicleRight->MoveBackward(keyDown);

	//keyDown = m_Input->IsAPressed();
	//m_Position->MoveUpward(keyDown);

	//keyDown = m_Input->IsZPressed();
	//m_Position->MoveDownward(keyDown);

	//keyDown = m_Input->IsPgUpPressed();
	//m_Position->LookUpward(keyDown);

	//keyDown = m_Input->IsPgDownPressed();
	//m_Position->LookDownward(keyDown);

	if (m_Input->IsIDown()) {
		if (m_renderLine)
			m_renderLine = false;
		else
			m_renderLine = true;
	}

	if (m_Input->IsODown()) {
		if (m_renderMesh)
			m_renderMesh = false;
		else
			m_renderMesh = true;
	}

	if (m_Input->IsUDown()) {
		if (m_renderUI)
			m_renderUI = false;
		else
			m_renderUI = true;
	}

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX - 50 * sin(rotY * 2 * MATH_PI / 360.0f), posY + 20, posZ - 50 * cos(rotY * 2 * MATH_PI / 360.0f));
	//m_Camera->SetRotation(rotX, rotY, rotZ);
	m_Camera->SetLookAt(50 * sin(rotY * 2 * MATH_PI / 360.0f), -20, 50 * cos(rotY * 2 * MATH_PI / 360.0f));

	// Update the location of the camera on the mini map.
	m_MiniMap->PositionUpdate(posX, posZ);

	return true;
}


bool ApplicationClass::RenderGraphics()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, vehicleMatrix, reflectionViewMatrix;
	bool result;
	D3DXVECTOR3 cameraPosition;

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	if (!m_start) {

		result = RenderMainMenu();
		if (!result)
		{
			return false;
		}
	}
	else
	{

		// Render the refraction of the scene to a texture.
		RenderRefractionToTexture();

		// Render the reflection of the scene to a texture.
		RenderReflectionToTexture();


		// Do the frame input processing.
		result = HandleInput(m_Timer->GetTime());

		m_Direct3D->GetWorldMatrix(worldMatrix);
		// Translate the sky dome to be centered around the camera position.
		D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// Turn off back face culling.
		m_Direct3D->TurnOffCulling();

		// Turn off the Z buffer.
		m_Direct3D->TurnZBufferOff();

		// Render the sky dome using the sky dome shader.
		m_skydome->Render(m_Direct3D->GetDeviceContext());
		m_skydomeshader->Render(m_Direct3D->GetDeviceContext(), m_skydome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_skydome->GetApexColor(), m_skydome->GetCenterColor());

		// Turn back face culling back on.
		m_Direct3D->TurnOnCulling();

		// Turn the Z buffer back on.
		m_Direct3D->TurnZBufferOn();
		if (!result)
		{
			return false;
		}

		m_Direct3D->GetWorldMatrix(worldMatrix);
		m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

		D3DXMatrixTranslation(&worldMatrix, 256, m_Water->GetWaterHeight(), 256);
		m_Water->Render(m_Direct3D->GetDeviceContext());
		m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix,
			m_RefractionTexture->GetShaderResourceView(), m_ReflectionTexture->GetShaderResourceView(), m_Water->GetTexture(),
			m_Camera->GetPosition(), m_Water->GetNormalMapTiling(), m_Water->GetWaterTranslation(), m_Water->GetReflectRefractScale(),
			m_Water->GetRefractionTint(), m_Light->GetDirection(), m_Water->GetSpecularShininess());

		m_Direct3D->GetWorldMatrix(worldMatrix);

		if (m_renderMesh)
		{
			m_Direct3D->EnableWireframe();
		}

		// Construct the frustum.
		m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

		// Set the terrain shader parameters that it will use for rendering.
		result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(),
			2.0f);
		if (!result)
		{
			return false;
		}


		// Render the terrain using the quad tree and terrain shader.
		m_QuadTree->Render(m_Frustum, m_Direct3D->GetDeviceContext(), m_TerrainShader, m_ColorShader, m_renderLine);

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Vehicle->Render(m_Direct3D->GetDeviceContext());

		CalculateVehicleWorldMatrix(vehicleMatrix);

		// Render the model using the light shader.
		result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Vehicle->GetIndexCount(), vehicleMatrix, viewMatrix, projectionMatrix,
			m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Vehicle->GetTexture());
		if (!result)
		{
			return false;
		}

		if (m_renderMesh)
		{
			m_Direct3D->DisableWireframe();
		}

		result = m_ColorShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}

		// Set the number of rendered terrain triangles since some were culled.
		result = m_Text->SetRenderCount(m_QuadTree->GetDrawCount(), m_Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

	}

	if (m_renderUI) {

		// Turn off the Z buffer to begin all 2D rendering.
		m_Direct3D->TurnZBufferOff();

		// Turn on the alpha blending before rendering the text.
		m_Direct3D->TurnOnAlphaBlending();

		// Render the text user interface elements.
		result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix, m_start);
		if (!result)
		{
			return false;
		}

		if (m_start) {
			// Render the mini map.
			result = m_MiniMap->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix, m_TextureShader);
			if (!result)
			{
				return false;
			}
		}

		// Turn off alpha blending after rendering the text.
		m_Direct3D->TurnOffAlphaBlending();

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_Direct3D->TurnZBufferOn();
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

void ApplicationClass::CalculateVehicleWorldMatrix(D3DXMATRIX& vehicleMatrix) {
	D3DXVECTOR3 position, rotation;
	D3DXMATRIX vehicleRotation, vehicleTranslation, vehicleScale;
	float centerHeight, frontHeight, backHeight, leftHeight, rightHeight, height;
	float yaw, pitch, roll;
	float posX, posY, posZ, rotX, rotY, rotZ;
	bool foundHeight;
	bool result;

	m_Position->GetPosition(position.x, position.y, position.z);
	m_Position->GetRotation(rotation.x, rotation.y, rotation.z);
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight)
	{
		// If there was a triangle under the position object then position the position object just above it by two units.
		m_Position->SetPosition(position.x, height + 2.0f, position.z);
		centerHeight = height + 2.0f;
	}

	m_vehicleFoward->GetPosition(position.x, position.y, position.z);
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight)
	{
		// If there was a triangle under the position object then position the position object just above it by two units.
		m_vehicleFoward->SetPosition(position.x, height + 2.0f, position.z);
		frontHeight = height + 2;
	}
	else
		frontHeight = 2;

	m_vehicleBack->GetPosition(position.x, position.y, position.z);
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight)
	{
		// If there was a triangle under the position object then position the position object just above it by two units.
		m_vehicleBack->SetPosition(position.x, height + 2.0f, position.z);
		backHeight = height + 2;
	}
	else
		backHeight = 2;

	pitch = atan((frontHeight - backHeight) / 2.0f);

	m_vehicleLeft->GetPosition(position.x, position.y, position.z);
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight)
	{
		// If there was a triangle under the position object then position the position object just above it by two units.
		m_vehicleLeft->SetPosition(position.x, height + 2.0f, position.z);
		leftHeight = height + 2;
	}
	else
		leftHeight = 2;

	m_vehicleRight->GetPosition(position.x, position.y, position.z);
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight)
	{
		// If there was a triangle under the position object then position the position object just above it by two units.
		m_vehicleRight->SetPosition(position.x, height + 2.0f, position.z);
		rightHeight = height + 2;
	}
	else
		rightHeight = 2;

	roll = atan((leftHeight - rightHeight) / 2.0f);

	yaw = (rotation.y - 180.0f) * 2 * MATH_PI / 360.0f;

	D3DXMatrixTranslation(&vehicleTranslation, position.x, position.y, position.z);
	D3DXMatrixScaling(&vehicleScale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationYawPitchRoll(&vehicleRotation, yaw, pitch, roll);
	//D3DXMatrixRotationY(&vehicleRotation, (rotation.y-180.0f)*2*MATH_PI/360.0f);
	vehicleMatrix = vehicleScale*vehicleRotation*vehicleTranslation;
	//vehicleMatrix = vehicleTranslation*vehicleScale;

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());

	result = m_Text->SetCameraRotation(pitch * 180.0f / MATH_PI, rotY, roll * 180.0f / MATH_PI, m_Direct3D->GetDeviceContext());

	return;
}

bool ApplicationClass::RenderMainMenu()
{
	int mouseX, mouseY;
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix, projectionMatrix, vehicleTranslation, vehicleScale, vehicleRotation;
	bool result;

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Check if the left mouse button has been pressed.
	if (m_Input->IsLeftMouseButtonDown())
	{
		// If they have clicked on the screen with the mouse then perform an intersection test.
		if (m_check == false)
		{
			m_check = true;
			m_Input->GetMouseLocation(mouseX, mouseY);
			TestIntersection(mouseX, mouseY);
		}
	}

	// Check if the left mouse button has been released.
	if (!m_Input->IsLeftMouseButtonDown())
	{
		m_check = false;
	}
	m_Direct3D->GetWorldMatrix(worldMatrix);
	static float delta = 0;
	delta += 0.01f;
	if (delta >= 2 * MATH_PI)
	{
		delta = 0;
	}
	D3DXMatrixTranslation(&vehicleTranslation, 0, 0, 0);
	D3DXMatrixScaling(&vehicleScale, 0.003f, 0.003f, 0.003f);
	D3DXMatrixRotationY(&vehicleRotation, delta);
	worldMatrix = vehicleScale*vehicleRotation*vehicleTranslation;
	m_MainMenuObject->Render(m_Direct3D->GetDeviceContext());
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_MainMenuObject->GetIndexCount(), worldMatrix, baseViewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_MainMenuObject->GetTexture());
	if (!result)
	{
		return false;
	}

	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	m_Input->GetMouseLocation(mouseX, mouseY);

	// Render the mouse cursor with the texture shader.
	result = m_Mouse->Render(m_Direct3D->GetDeviceContext(), mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Mouse->GetIndexCount(),
		worldMatrix, baseViewMatrix, orthoMatrix, m_Mouse->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	return true;
}


void ApplicationClass::TestIntersection(int mouseX, int mouseY)
{
	float pointX, pointY;
	D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;
	bool intersect, result;


	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	m_Camera->GetViewMatrix(viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = m_Camera->GetPosition();

	// Get the world matrix and translate to the location of the sphere.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	// Now get the inverse of the translated world matrix.
	D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
	D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDirection, &rayDirection);

	// Now perform the ray-sphere intersection test.
	intersect = RaySphereIntersect(rayOrigin, rayDirection, 1.0f);

	if (intersect == true)
	{
		m_start = true;
		//MessageBox(NULL, L"1123", L"1123", MB_OK);
	}
	else
	{
		m_start = false;
	}

	return;
}


bool ApplicationClass::RaySphereIntersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, float radius)
{
	float a, b, c, discriminant;


	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

void ApplicationClass::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;


	// Setup a clipping plane based on the height of the water to clip everything above it to create a refraction.
	clipPlane = D3DXVECTOR4(0, -1, 0, m_Water->GetWaterHeight() + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Render the terrain using the reflection shader and the refraction clip plane to produce the refraction effect.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());
	m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 6.0f,
		clipPlane);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return;
}

void ApplicationClass::RenderReflectionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	D3DXVECTOR3 cameraPosition;


	// Setup a clipping plane based on the height of the water to clip everything below it.
	clipPlane = D3DXVECTOR4(0, 1, 0, -m_Water->GetWaterHeight());

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_Water->GetWaterHeight());

	// Get the camera reflection view matrix instead of the normal view matrix.
	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	// Get the world and projection matrices from the d3d object.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Invert the Y coordinate of the camera around the water plane height for the reflected camera position.
	cameraPosition.y = -cameraPosition.y + (m_Water->GetWaterHeight() * 2.0f);

	// Translate the sky dome and sky plane to be centered around the reflected camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and the Z buffer.
	m_Direct3D->TurnOffCulling();
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the reflection view matrix.
	m_skydome->Render(m_Direct3D->GetDeviceContext());
	m_skydomeshader->Render(m_Direct3D->GetDeviceContext(), m_skydome->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_skydome->GetApexColor(), m_skydome->GetCenterColor());

	// Enable back face culling.
	m_Direct3D->TurnOnCulling();

	// Enable additive blending so the clouds blend with the sky dome color.
	//m_Direct3D->EnableSecondBlendState();
	//
	// Render the sky plane using the sky plane shader.
	//m_SkyPlane->Render(m_Direct3D->GetDeviceContext());
	//m_SkyPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
	//	m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
	//	m_SkyPlane->GetBrightness());

	// Turn off blending and enable the Z buffer again.
	//m_Direct3D->TurnOffAlphaBlending();
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain using the reflection view matrix and reflection clip plane.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());
	m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 2.0f,
		clipPlane);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return;
}