////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "terrainshaderclass.h"
#include "lightclass.h"
#include "frustumclass.h"
#include "quadtreeclass.h"
#include "Colorshaderclass.h"
#include "VehicleClass.h"
#include "lightshaderclass.h"
#include "textureshaderclass.h"
#include "minimapclass.h"
#include "bitmapclass.h"
#include "skydomeclass.h"
#include "skydomeshaderclass.h"
#include "rendertextureclass.h"
#include "reflectionshaderclass.h"
#include "waterclass.h"
#include "watershaderclass.h"

#ifndef MATH_PI
#define MATH_PI 3.1415926535897932384626f
#endif
////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();

	bool RenderMainMenu();
	void CalculateVehicleWorldMatrix(D3DXMATRIX&);

	void TestIntersection(int, int);
	bool RaySphereIntersect(D3DXVECTOR3, D3DXVECTOR3, float);

	void RenderRefractionToTexture();
	void RenderReflectionToTexture();
private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	TerrainShaderClass* m_TerrainShader;
	ColorShaderClass* m_ColorShader;
	LightClass* m_Light;
	FrustumClass* m_Frustum;
	QuadTreeClass* m_QuadTree;
	VehicleClass* m_Vehicle, *m_MainMenuObject;
	LightShaderClass* m_LightShader;
	
	PositionClass* m_vehicleFoward, *m_vehicleBack, *m_vehicleLeft, *m_vehicleRight;
	TextureShaderClass* m_TextureShader;
	MiniMapClass* m_MiniMap;
	BitmapClass* m_Mouse;
	SkyDomeClass* m_skydome;
	SkyDomeShaderClass* m_skydomeshader;

	bool m_renderLine, m_renderMesh, m_renderUI, m_check, m_start;
	D3DXMATRIX baseViewMatrix;
	int m_screenWidth, m_screenHeight;

	RenderTextureClass *m_RefractionTexture, *m_ReflectionTexture;
	ReflectionShaderClass* m_ReflectionShader;
	WaterClass* m_Water;
	WaterShaderClass* m_WaterShader;
};

#endif