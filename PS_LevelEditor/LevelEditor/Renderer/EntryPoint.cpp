#include "EntryPoint.h"
#include "PS RenderEngine\MeshManager.h"
#include "ObjectManager.h"
#include <string>
#include "PS RenderEngine\DebugShapes.h"
#include "PS RenderEngine\LightSystem\LightManager.h"

LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL; // Vertex Buffer Containing our triangle

RenderEngine m_reRenderer;
RenderEngine* GetRenderer () { return &m_reRenderer; }

CCamera m_pCamera;
CCamera* GetCam() { return &m_pCamera; }

std::vector<int> HazardTypes;
std::vector<int>& GetHazardTypes () { return HazardTypes; }

std::vector <D3DXVECTOR3> m_Hazards;
std::vector <D3DXVECTOR3>& GetHazards () { return m_Hazards; }

std::vector<int> ObjectTypes;
std::vector<int>& GetObjectTypes () { return ObjectTypes; }

std::vector <IBaseObject*> m_Objects;
std::vector <IBaseObject*>& GetObjects () { return m_Objects; }

void LoadLevel (char* _pFileName)
{
	std::string LevelPath = "Resources/Models/";
	LevelPath += _pFileName;
	LevelPath += ".InfiniteHPScene";

	std::string TrapPath = "Resources/LevelData/TrapData/";
	TrapPath += _pFileName;
	TrapPath += "_Trap.InfiniteHPHazards";

	std::string ObjectsPath = "Resources/LevelData/ObjectData/";
	ObjectsPath += _pFileName;
	ObjectsPath += "_MoveableObject.InfiniteHPMoveableObjects";

	std::string LightPath = "Resources/LevelData/";
	LightPath += _pFileName;
	LightPath += ".InfiniteHPLightsData";

	LightManager::GetInstance()->LoadLights( LightPath.c_str() );

	bool result = MeshManager::GetInstance()->LoadEnvironment( LevelPath.c_str() );
	result = MeshManager::GetInstance()->LoadEnvironmentTraps( TrapPath.c_str() );
	result = MeshManager::GetInstance()->LoadEnvironmentMoveableObjects( ObjectsPath.c_str() );
}

void SwapLevel (char* _pFileName)
{
	ReloadRenderer ();

	std::string LevelPath = "Resources/Models/";
	LevelPath += _pFileName;
	LevelPath += ".InfiniteHPScene";

	std::string TrapPath = "Resources/LevelData/TrapData/";
	TrapPath += _pFileName;
	TrapPath += "_Trap.InfiniteHPHazards";

	std::string ObjectsPath = "Resources/LevelData/ObjectData/";
	ObjectsPath += _pFileName;
	ObjectsPath += "_MoveableObject.InfiniteHPMoveableObjects";

	std::string LightPath = "Resources/LevelData/";
	LightPath += _pFileName;
	LightPath += ".InfiniteHPLightsData";

	LightManager::GetInstance()->LoadLights( LightPath.c_str() );

	bool result = MeshManager::GetInstance()->LoadEnvironment( LevelPath.c_str() );
	result = MeshManager::GetInstance()->LoadEnvironmentTraps( TrapPath.c_str() );
	result = MeshManager::GetInstance()->LoadEnvironmentMoveableObjects( ObjectsPath.c_str() );
}

void ReloadRenderer ()
{
	m_Hazards.clear();
	HazardTypes.clear();
	m_Objects.clear ();
	ObjectTypes.clear();

	MeshManager::GetInstance()->ShutdownMeshManager();
	CObjectManager::GetInstance()->ClearAll();
	m_reRenderer.ClearData();

	MeshManager::GetInstance()->InitMeshManager();

	m_reRenderer.BuildScreenMesh();

	LightManager::GetInstance()->ShutdownLightManager();

	LightManager::GetInstance()->InitLightManager();


	D3DXMATRIX i;
	D3DXMatrixIdentity( &i );

	MeshManager::GetInstance()->LoadAnimatedMesh("Pudge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("LilGoo", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("SlimeMonster", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("Button", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("Bridge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "Door", i );

	MeshManager::GetInstance()->LoadMesh("Crate", i);
	MeshManager::GetInstance()->LoadMesh("Goopit", i );
	MeshManager::GetInstance()->LoadMesh("Cork", i );
	MeshManager::GetInstance()->LoadMesh("SteamPipe", i );
	MeshManager::GetInstance()->LoadMesh("BreakableWall", i );
	MeshManager::GetInstance()->LoadMesh("Intake", i );
	MeshManager::GetInstance()->LoadMesh("Plug", i );
	MeshManager::GetInstance()->LoadMesh("SpawnPipe", i );
	MeshManager::GetInstance()->LoadMesh("Crusher", i );
	MeshManager::GetInstance()->LoadMesh("CrusherBase", i );
	MeshManager::GetInstance()->LoadMesh("Outlet", i);
	MeshManager::GetInstance()->LoadMesh("WaterPool", i);
	MeshManager::GetInstance()->LoadMesh("BoxSpawnPipe", i);
	
}

HRESULT InitD3D( HWND hWnd, int width, int height )
{

	m_reRenderer.SCREEN_WIDTH = width;
	m_reRenderer.SCREEN_HEIGHT = height;
	m_reRenderer.InitRenderEngine (hWnd, NULL, width, height, true, false,  D3DXVECTOR3(0.0f,0.0f,1.0f));

	DebugShapes::CreateSphere (50.0f, 5, 5);

	D3DXMATRIX i;
	D3DXMatrixIdentity( &i );

	MeshManager::GetInstance()->LoadAnimatedMesh("Pudge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("LilGoo", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("SlimeMonster", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("Bridge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "Door", i );
	MeshManager::GetInstance()->LoadAnimatedMesh("Button", i);

	MeshManager::GetInstance()->LoadMesh("Crate", i);
	MeshManager::GetInstance()->LoadMesh("Goopit", i );
	MeshManager::GetInstance()->LoadMesh("Cork", i );
	MeshManager::GetInstance()->LoadMesh("SteamPipe", i );
	MeshManager::GetInstance()->LoadMesh("BreakableWall", i );
	MeshManager::GetInstance()->LoadMesh("Intake", i );
	MeshManager::GetInstance()->LoadMesh("Plug", i );
	MeshManager::GetInstance()->LoadMesh("SpawnPipe", i );
	MeshManager::GetInstance()->LoadMesh("Crusher", i );
	MeshManager::GetInstance()->LoadMesh("CrusherBase", i );
	MeshManager::GetInstance()->LoadAnimatedMesh("Bridge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "Door", i );
	MeshManager::GetInstance()->LoadMesh("Outlet", i);
	MeshManager::GetInstance()->LoadMesh("WaterPool", i);
	MeshManager::GetInstance()->LoadMesh("BoxSpawnPipe", i);

	/*MeshManager::GetInstance()->LoadEnvironment("../../../Renderer/Resources/Models/Sector1.InfiniteHPScene");*/
	/*MeshManager::GetInstance()->LoadEnvironmentTraps( "../../../Renderer/Resources/LevelData/TrapData/Sector1_Trap.InfiniteHPHazards");
	MeshManager::GetInstance()->LoadEnvironmentMoveableObjects( "../../../Renderer/Resources/LevelData/ObjectData/Sector1_MoveableObject.InfiniteHPMoveableObjects");*/

	m_pCamera.Initialize (m_reRenderer.m_pD3Dev);

	float fov = D3DX_PI/3.0f;
	m_pCamera.BuildPerspective( fov, width/(float)(height), 0.1f, 10000.0f);
	m_pCamera.SetViewPosition(0.0f, 0.0f, -1000.0f);

    return S_OK;
}

void Update (float fElapsedTime)
{
	if (GetAsyncKeyState ('W'))
	{
		GetCam()->TranslateGlobalY (500.0f * fElapsedTime);
	}

	if (GetAsyncKeyState ('S'))
	{
		GetCam()->TranslateGlobalY (-500.0f * fElapsedTime);
	}

	if (GetAsyncKeyState ('A'))
	{
		GetCam()->TranslateGlobalX (-500.0f * fElapsedTime);
	}

	if (GetAsyncKeyState ('D'))
	{
		GetCam()->TranslateGlobalX (500.0f * fElapsedTime);
	}
	if (GetAsyncKeyState (VK_UP))
	{
		GetCam()->TranslateGlobalZ (500.0f * fElapsedTime);
	}
	if (GetAsyncKeyState (VK_DOWN))
	{
		GetCam()->TranslateGlobalZ (-500.0f * fElapsedTime);
	}

	m_reRenderer.SetModelViewProjection( m_pCamera.GetViewMatrix() * m_pCamera.GetProjectionMatrix() );
	m_reRenderer.SetCamera( &m_pCamera );
}


VOID Render()
{
	m_reRenderer.Clear();
	m_reRenderer.BeginScene();

	m_reRenderer.Render();
	CObjectManager::GetInstance()->Render ();

	m_reRenderer.EndScene();
	m_reRenderer.Present();
}