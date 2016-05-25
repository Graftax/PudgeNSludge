//////////////////////////////////////////////////////////////////////////////////////
// Filename:            RenderEngine.cpp
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             An engine used to render anything in the game to the screen.
//////////////////////////////////////////////////////////////////////////////////////
#include "RenderEngine.h"
#include "RenderSet.h"
#include "RenderSetSorted.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "RenderTarget.h"
#include "PostProcess.h"
#include "PostProcessChain.h"
#include "../ObjectManager.h"

#include "LightSystem\LightManager.h"
#include "LightSystem\Light.h"
#include "LightSystem\PointLight.h"
#include "LightSystem\DirectionalLight.h"
#include "LightSystem\SpotLight.h"


#include "DebugShapes.h"

//Default values for all static object within the class

int								RenderEngine::SCREEN_HEIGHT (0);
int								RenderEngine::SCREEN_WIDTH (0);
HWND							RenderEngine::m_hWnd(NULL);
HINSTANCE						RenderEngine::m_hInstance(NULL);
IDirect3D9*						RenderEngine::m_pD3D(nullptr);
IDirect3DDevice9*				RenderEngine::m_pD3Dev(nullptr);
IDirect3DVertexDeclaration9*	RenderEngine::m_pPosNormTanUVdecl(nullptr);
IDirect3DVertexDeclaration9*	RenderEngine::m_pPosNormTanUVInfdecl(nullptr);
IDirect3DVertexDeclaration9*	RenderEngine::m_pPosColor(nullptr);
LPD3DXEFFECTPOOL				RenderEngine::m_pEffectPool(nullptr);
int								RenderEngine::m_pDissolveTexture(0);

UINT							RenderEngine::m_unFrameCounter(NULL);
D3DPRESENT_PARAMETERS			RenderEngine::m_d3dPresentParams; //no default init?
UINT							RenderEngine::m_unScreenWidth(NULL);
UINT							RenderEngine::m_unScreenHeight(NULL);
D3DXVECTOR3						RenderEngine::m_v3ClearColor;
D3DXMATRIX						RenderEngine::m_mMVP;

RenderSetSorted					RenderEngine::m_ObjectRenderSet;
RenderSetSorted					RenderEngine::m_EnvironmentRenderSet;
RenderSetSorted					RenderEngine::m_ObjForwardRenderSet;
RenderSetSorted					RenderEngine::m_EnviForwardRenderSet;

ID3DXEffect*					RenderEngine::CubeEffect(nullptr);
ID3DXEffect*					RenderEngine::TextureMeshEffect(nullptr);
ID3DXEffect*					RenderEngine::AnimatedMeshEffect(nullptr);
ID3DXEffect*					RenderEngine::GBufferEffect(nullptr);
ID3DXEffect*					RenderEngine::DirectionalLightEffect(nullptr);
ID3DXEffect*					RenderEngine::PointLightEffect(nullptr);
ID3DXEffect*					RenderEngine::SpotLightEffect(nullptr);

TextureManager*					RenderEngine::m_pTextureMan = nullptr;

ID3DXFont*						RenderEngine::dxFont = (nullptr);
unsigned int					RenderEngine::m_unFPS = 0;

RenderTarget					RenderEngine::m_rtGDepth;
RenderTarget					RenderEngine::m_rtGNormal;
RenderTarget					RenderEngine::m_rtGDiffuse;
RenderTarget					RenderEngine::m_rtGSpecular;
RenderTarget					RenderEngine::m_rtGEmissive;

PostProcess						RenderEngine::m_DepthOfField;
PostProcess						RenderEngine::m_BlurHorizontal;
PostProcessChain				RenderEngine::m_PostProcessChain;

//#if DEBUGSHAPES && !DEFERREDRENDER
	DebugShapes					RenderEngine::m_oDebugShapes;
	std::vector<D3DXMATRIX>		RenderEngine::m_vSphereMats;
	std::vector<D3DXMATRIX>		RenderEngine::m_vBoxMats;
//#endif

RenderHandle					RenderEngine::screenMesh;
RenderHandle					RenderEngine::postprocessMesh;

CCamera*						RenderEngine::m_pCamera(nullptr);
IDirect3DVertexBuffer9*			RenderEngine::m_pLineVert(nullptr);
IDirect3DIndexBuffer9*			RenderEngine::m_pLineIndex(nullptr);
VERTEX_POSCOLOR*				RenderEngine::m_pLineVertexs(nullptr);
DWORD*							RenderEngine::m_pDwordIndex(nullptr);
std::vector<Glow_Info>			RenderEngine::m_vGlow;


DirectionalLight*				RenderEngine::m_Light = nullptr;
PointLight*						RenderEngine::m_PointLight = nullptr;
SpotLight*						RenderEngine::m_SpotLight = nullptr;

D3DXVECTOR2						RenderEngine::m_v2fScrollOffset;

// Rueben's stuff
// bool for laser sight
bool							RenderEngine::m_bObjPulled;

float							RenderEngine::m_fGammaFactor(0.0f);
float							RenderEngine::m_fBurnFactor(0.95f);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitRenderEngine(): Starts up the render engine and inits all managers necessary.
//                       
// Returns:            bool - returns true if the engine started up
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RenderEngine::InitRenderEngine(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed, bool bVsync, D3DXVECTOR3 _v3ClearColor )
{
	if( !hWnd ) return false; //Easy out if we did not get a window handle yet...

	m_hWnd				= hWnd;//The current window handle
	m_hInstance			= hInstance;//The HINSTANCE
	m_unScreenWidth		= nScreenWidth;//Screen Width
	m_unScreenHeight	= nScreenHeight;//Screen Height
	m_v3ClearColor		= _v3ClearColor;//Color to clear the buffer to
	m_pD3D				= Direct3DCreate9( D3D_SDK_VERSION );//DirectX object created with the current SDK version installed

	ZeroMemory(&m_d3dPresentParams, sizeof(D3DPRESENT_PARAMETERS));


	//Setup the present params in order to create the DirectX device
	m_d3dPresentParams.BackBufferWidth				= nScreenWidth;
	m_d3dPresentParams.BackBufferHeight				= nScreenHeight;
	m_d3dPresentParams.BackBufferFormat				= (bIsWindowed) ? D3DFMT_UNKNOWN : D3DFMT_X8R8G8B8;
	m_d3dPresentParams.BackBufferCount				= 1;
	m_d3dPresentParams.Windowed						= bIsWindowed;
	m_d3dPresentParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_d3dPresentParams.MultiSampleQuality			= NULL;
	m_d3dPresentParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_d3dPresentParams.EnableAutoDepthStencil		= TRUE;
	m_d3dPresentParams.AutoDepthStencilFormat		= D3DFMT_D16;
	m_d3dPresentParams.hDeviceWindow				= hWnd;
	m_d3dPresentParams.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_d3dPresentParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	m_d3dPresentParams.PresentationInterval			= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr;

	//Create the DirectX device and check for errors after being initialized
	if( FAILED( hr = m_pD3D->CreateDevice( 0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dPresentParams, &m_pD3Dev )))
	{
		//const char* string = DXGetErrorStringA( hr );
		MessageBoxW( hWnd, L"Failed to create DRX", NULL, MB_OK );
		return false;
	}

	//Enable the Z buffer to not cause depth artifacts
	//m_pD3Dev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	
	hr = D3DXCreateEffectPool( &m_pEffectPool );

	//Initialize the font object being used to display the frames per second
	D3DXCreateFontW(m_pD3Dev, 22, 0, FW_NORMAL, NULL, false, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		L"Arial", &dxFont);

	//Initialize the needed vertex declerations
	CreateVertexDeclerations();
	//Load shaders needed
	LoadShaders();

	//Create the screen mesh
	BuildScreenMesh();
	//Init the texture manager
	TextureManager::GetInstance()->InitTextureManager();
	m_pTextureMan = TextureManager::GetInstance();

	LightManager::GetInstance()->InitLightManager();

	//CreateLights();

	m_PostProcessChain.Initialize();
	m_DepthOfField.Initialize( &postprocessMesh, L"./Resources/Shaders/DepthOfField.fx" );
	m_BlurHorizontal.Initialize( &postprocessMesh, L"./Resources/Shaders/DrawEdges.fx" );

	m_PostProcessChain.AddPostProcess( &m_BlurHorizontal );

#if DEBUGSHAPES && !DEFERREDRENDER
	//In debug allow debug shapes to be created
	m_oDebugShapes.InitShapes();
#endif

	//Create buffers for the line to be drawn to the screen, will be changed to a cylinder
	//Currently not supported on ATI cards
	m_pLineVert = InitVertexBuffer( sizeof(VERTEX_POSNORMTANUV) * 2);
	m_pLineIndex = InitIndexBuffer( sizeof(DWORD) * 2 );
	m_pLineVertexs = new VERTEX_POSCOLOR[2];
	m_pDwordIndex = new DWORD[2];

	// Rueben's stuff
	// bool for laser sight
	m_bObjPulled = false;

	CreateRenderTargets();

	m_fGammaFactor = 0.0f;

	m_pDissolveTexture = /*TextureManager::GetInstance()->GetTexture( */TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/Clouds.png" );// );
	

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ShutdownRenderEngine(): Shuts down the render engine and releases all COM objects.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::ShutdownRenderEngine(void)
{
	m_rtGDepth.DeleteTarget();
	m_rtGNormal.DeleteTarget();
	m_rtGDiffuse.DeleteTarget();
	m_rtGSpecular.DeleteTarget();
	m_rtGEmissive.DeleteTarget();

	//Shutdown our mesh manager
	MeshManager::GetInstance()->ShutdownMeshManager();
	//Shutdown our texture manager
	TextureManager::GetInstance()->ShutdownTextureManager();

	LightManager::GetInstance()->ShutdownLightManager();

	ClearData();
	
	//Delete the memory allocated for drawing a line to the screen
	delete[] m_pLineVertexs;
	delete[] m_pDwordIndex;

	//Release all loaded DirectX objects
	SAFE_RELEASE( m_pLineIndex );
	SAFE_RELEASE( m_pLineVert );
	SAFE_RELEASE( AnimatedMeshEffect );
	SAFE_RELEASE( CubeEffect );
	SAFE_RELEASE( GBufferEffect );
	SAFE_RELEASE( TextureMeshEffect );
	SAFE_RELEASE( DirectionalLightEffect );
	SAFE_RELEASE( PointLightEffect );
	SAFE_RELEASE( SpotLightEffect );
	SAFE_RELEASE( m_pEffectPool );
	SAFE_RELEASE( dxFont );
	SAFE_RELEASE( m_pPosColor );
	SAFE_RELEASE( m_pPosNormTanUVInfdecl );
	SAFE_RELEASE( m_pPosNormTanUVdecl );
	SAFE_RELEASE( m_pD3Dev );
	//while( m_pD3Dev->Release() > 0 )
	//{}
	//m_pD3Dev = nullptr;
	SAFE_RELEASE( m_pD3D );
	//while( m_pD3D->Release() > 0 )
	//{}
	//m_pD3D = nullptr;
}

float CalcGaussianBlurWeight(float x)
{
	const float StandardDeviation = 1;
	const float StandardDeviationSquared = 
		StandardDeviation * StandardDeviation;

	return (1.0f / sqrt(2 * D3DX_PI * StandardDeviationSquared)) * 
		pow(float(M_E), float(-((x * x) / (2 * StandardDeviationSquared))));
}

void RenderEngine::SetShaderGlobals(void)
{
	m_BlurHorizontal.GetEffect()->SetFloat( "gCenterBlurWeights", CalcGaussianBlurWeight(0));

	D3DXVECTOR4 blurWeights(CalcGaussianBlurWeight(1), CalcGaussianBlurWeight(2), CalcGaussianBlurWeight(3), CalcGaussianBlurWeight(4));
	m_BlurHorizontal.GetEffect()->SetVector( "gBlurWeights", &blurWeights );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render(): Render all objects currently set to be rendered, loops through both render sets.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::Render(void)
{	
	SetShaderGlobals();
	m_v2fScrollOffset.x += SCROLL_SPEED;
	m_v2fScrollOffset.y += SCROLL_SPEED;

	m_fBurnFactor -= BURN_RATE;
#if DEFERREDRENDER
	RenderDepthBuffer();
	if( m_pCamera )
		RenderGBuffers();
	
	//Clear();
#endif
	//Get the beginning of the render set
#if !DEFERREDRENDER
	RenderNode* environmentSet = m_EnvironmentRenderSet.GetHead();
	//Loop through all node within the render set - STATIC OBJECTS//LIST NEVER CLEARED!
	while( environmentSet )
	{
		//Call the current nodes render function
		environmentSet->RenderProcess();
		//Go to the next render node
		environmentSet = environmentSet->GetNext();
	}
	//Get the beginning of the render set
	RenderNode* objectSet = m_ObjectRenderSet.GetHead();
	//Loop through all objects in the current render set
	while( objectSet )
	{
		//Call the current nodes render function
		objectSet->RenderProcess();
		//Go to the next render node
		objectSet = objectSet->GetNext();
	}
#endif
	// Render Frustum
#if DEBUGSHAPES && !DEFERREDRENDER
	RenderViewFrustum();
#endif

//Render code that is only used when running DEBUG mode

//#if DEBUGSHAPES //&& !DEFERREDRENDER// && !DEFERREDRENDER
////DEBUG SHAPES CODE	
//	m_oDebugShapes.RenderSpheres( m_vSphereMats );
//	m_oDebugShapes.RenderBoxs( m_vBoxMats );
//	m_vSphereMats.clear();
//	m_vBoxMats.clear();
//#endif	

	//The render set filled with non static scene objects
	
	//IBaseObject* pPlayer = CObjectManager::GetInstance()->GetPlayer();
	

#if _DEBUG
//Display the current FPS to the screen
	char text[64];
	wsprintfW(LPWSTR(text), L"FPS: %d", m_unFPS);
	RECT fontRec = {0,0,250,250};
	dxFont->DrawTextW(0, LPWSTR(text), -1, &fontRec, 0, D3DCOLOR_XRGB(255,255,255));
#endif




#if DEFERREDRENDER
	//m_rtGEmissive.ActivateTarget(0);
	m_PostProcessChain.GetActiveRenderTarget()->ActivateTarget( 0 );
	Clear();	
	RenderLights();
#if DEBUGSHAPES //&& !DEFERREDRENDER// && !DEFERREDRENDER
//DEBUG SHAPES CODE	
	m_oDebugShapes.RenderSpheres( m_vSphereMats );
	m_oDebugShapes.RenderBoxs( m_vBoxMats );
	m_vSphereMats.clear();
	m_vBoxMats.clear();
#endif	
	m_PostProcessChain.GetActiveRenderTarget()->RevertTarget();
	//m_rtGEmissive.RevertTarget();

	//kellybooooooboooooooooooo<3

	m_PostProcessChain.GetActiveRenderTarget()->ActivateTarget( 0 );
	m_rtGDepth.ActivateTarget( 1 );
	m_rtGNormal.ActivateTarget( 2 );

	TextureMeshEffect->SetTexture( "finalMap", m_PostProcessChain.GetActiveRenderTarget()->GetTexture() );
	RenderNode* forwardPass = m_ObjForwardRenderSet.GetHead();
	while( forwardPass )
	{
		forwardPass->RenderProcess();

		forwardPass = forwardPass->GetNext();
	}

	forwardPass = m_EnviForwardRenderSet.GetHead();
	while( forwardPass )
	{
		forwardPass->RenderProcess();

		forwardPass = forwardPass->GetNext();
	}

	m_ObjForwardRenderSet.ClearRenderSet();
	m_ObjectRenderSet.ClearRenderSet();
#if DEFERREDRENDER
#pragma region RENDER_GLOWEFFECTS
	for( unsigned int i = 0u; i < m_vGlow.size(); ++i )
		DrawTexturedPlane( m_vGlow[i]._transform, TextureManager::GetInstance()->m_umTextureMap[ m_vGlow[i]._nTextHandle ].m_lpTexture );

	m_vGlow.clear();
#pragma endregion
#endif



	m_rtGNormal.RevertTarget();
	m_rtGDepth.RevertTarget();
	m_PostProcessChain.GetActiveRenderTarget()->RevertTarget();
	
	//m_DepthOfField.GetEffect()->SetFloat( "gPlaneX", float( -10000.0f / (10000.0f - 0.1f )));
	//m_DepthOfField.GetEffect()->SetFloat( "gPlaneY", float( -10000.0f * 0.1f / (10000.0f - 0.1f )));

	//m_DepthOfField.Process( m_rtGEmissive, m_rtGEmissive.GetTexture() );
	//m_BlurHorizontal.Process( *m_PostProcessChain.GetActiveRenderTarget(), m_PostProcessChain.GetActiveRenderTarget()->GetTexture() );
	m_PostProcessChain.Run();

	DrawFinalRender( m_PostProcessChain.GetActiveRenderTarget()->GetTexture() );

#endif

#if (SHOWRENDERTARGETS && DEFERREDRENDER)
	RasterTexture( m_rtGDepth.GetTexture(), 0.0f, 0.0f, 0.2f, 0.2f );
	RasterTexture( m_rtGNormal.GetTexture(), 0.2f, 0.0f, 0.4f, 0.2f );
	RasterTexture( m_rtGDiffuse.GetTexture(), 0.4f, 0.0f, 0.6f, 0.2f );
	RasterTexture( m_rtGSpecular.GetTexture(), 0.6f, 0.0f, 0.8f, 0.2f );
#endif
}

void RenderEngine::RenderGBuffers(void)
{
	//m_pD3Dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	
	m_rtGDiffuse.ActivateTarget( 0 );
	m_rtGNormal.ActivateTarget( 1 );
	m_rtGSpecular.ActivateTarget( 2 );
	//m_rtGEmissive.ActivateTarget( 3 );

	m_pD3Dev->Clear(0,0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1, 0);
	//Get the beginning of the render set

	RenderNode* environmentSet = m_EnvironmentRenderSet.GetHead();
	//Loop through all node within the render set - STATIC OBJECTS//LIST NEVER CLEARED!
	while( environmentSet )
	{
		if( environmentSet->m_pOwner->TestFlag( RE_FORWARD ) )
		{
			AddToObjectRenderSet( environmentSet, environmentSet->renderData.m_vTransform[0] );
		}
		else
		//Call the current nodes render function
		DrawGBuffers(*environmentSet);
		//Go to the next render node
		environmentSet = environmentSet->GetNext();
	}
	//Get the beginning of the render set
	RenderNode* objectSet = m_ObjectRenderSet.GetHead();
	//Loop through all objects in the current render set
	while( objectSet )
	{
		//Call the current nodes render function
		DrawGBuffers(*objectSet);
		//Go to the next render node
		objectSet = objectSet->GetNext();
	}

	//m_rtGEmissive.RevertTarget();
	m_rtGSpecular.RevertTarget();
	m_rtGNormal.RevertTarget();
	m_rtGDiffuse.RevertTarget();

	//m_pD3Dev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
}

void RenderEngine::RenderDepthBuffer(void)
{
	m_rtGDepth.ActivateTarget( 0 );
	m_pD3Dev->Clear(0,0, D3DCLEAR_TARGET , D3DCOLOR_XRGB(255,255,255), 1.0f, 0);	

	//Get the beginning of the render set
	RenderNode* environmentSet = m_EnvironmentRenderSet.GetHead();
	//Loop through all node within the render set - STATIC OBJECTS//LIST NEVER CLEARED!
	while( environmentSet )
	{
		if( environmentSet->m_pOwner->TestFlag( RE_FORWARD ) )
		{
			AddToObjectRenderSet( environmentSet, environmentSet->renderData.m_vTransform[0] );
		}
		else
		//Call the current nodes render function
		DrawDepth(*environmentSet);
		//Go to the next render node
		environmentSet = environmentSet->GetNext();
	}
	//Get the beginning of the render set
	RenderNode* objectSet = m_ObjectRenderSet.GetHead();
	//Loop through all objects in the current render set
	while( objectSet )
	{
		//Call the current nodes render function
		DrawDepth(*objectSet);
		//Go to the next render node
		objectSet = objectSet->GetNext();
	}

	m_rtGDepth.RevertTarget();
}

void RenderEngine::ResetDevice(void)
{
	m_rtGDepth.DeleteTarget();
	m_rtGNormal.DeleteTarget();
	m_rtGDiffuse.DeleteTarget();
	m_rtGSpecular.DeleteTarget();
	m_rtGEmissive.DeleteTarget();

	m_PostProcessChain.Clear();

	TextureManager::GetInstance()->UnloadAllTextures();

	//ClearData();

	OnLostDevice();	

	if( m_pD3Dev->Reset(&m_d3dPresentParams) != D3D_OK )
		return;

	OnResetDevice();

	m_pD3Dev->TestCooperativeLevel();

	TextureManager::GetInstance()->ReloadAllTextures();

	CreateRenderTargets();

	m_PostProcessChain.Initialize();

	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Clear(): Clears all the buffers being used by DirectX.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::Clear(void)
{
	//Cleat the current render target and the Z-Buffer
	m_pD3Dev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	// Check for lost device (could happen from an ALT+TAB or ALT+ENTER).
	if (m_pD3Dev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ResetDevice();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BeginScene(): Begins the scene with DirectX
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::BeginScene(void)
{
	//Begin the current scene
	m_pD3Dev->BeginScene();

		// Check for lost device (could happen from an ALT+TAB or ALT+ENTER).
	if (m_pD3Dev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ResetDevice();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EndScene(): Ends the scene with DirectX
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::EndScene(void)
{
	//End the current scene
	m_pD3Dev->EndScene();

		// Check for lost device (could happen from an ALT+TAB or ALT+ENTER).
	if (m_pD3Dev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ResetDevice();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Present(): Presents the currently drawn buffer to the screen based on the current render target.
//                       
// Returns:  void          bool - returns true if the engine started up
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::Present(void)
{
	//Prevent a present call if the renderEngine is currently in the process of reseting the present parameters
	//if(IsWindow(m_hWnd))
		m_pD3Dev->Present( NULL, NULL, NULL, NULL );

		// Check for lost device (could happen from an ALT+TAB or ALT+ENTER).
	if (m_pD3Dev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ResetDevice();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChangeDisplayParam(): Changes the present pararmeters being used by the current DRX COM object.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::ChangeDisplayParam( int nScreenWidth, int nScreenHeight, bool bWindowed, bool bVsync )
{
	m_d3dPresentParams.BackBufferWidth				= nScreenWidth;
	m_d3dPresentParams.BackBufferHeight				= nScreenHeight;
	m_d3dPresentParams.BackBufferFormat				= (bWindowed) ? D3DFMT_UNKNOWN : D3DFMT_X8R8G8B8;

	m_d3dPresentParams.Windowed						= bWindowed;

	m_d3dPresentParams.FullScreen_RefreshRateInHz	= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dPresentParams.PresentationInterval			= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;



	//TextureManager::GetInstance()->UnloadAllTextures();

	//OnLostDevice();

	//HRESULT hr = m_pD3Dev->Reset(&m_d3dPresentParams);

	//LOG(LOG_INFO, "ChangeDisplayParam", "Reset result", hr );

	//OnResetDevice();

	

	HRESULT hr = m_pD3Dev->TestCooperativeLevel();
	//while( m_pD3Dev->TestCooperativeLevel() != D3D_OK ) {}
	//Enable the Z buffer to not cause depth artifacts
	//m_pD3Dev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	//CreateRenderTargets();
	// Setup window style flags

	//TextureManager::GetInstance()->ReloadAllTextures();

	if (bWindowed)
	{
		//dwWindowStyleFlags = NULL;
		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, NULL );
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_SYSMENU);
		//ChangeDisplaySettings(0,0);
		UpdateWindow(m_hWnd);
		//ShowWindow(m_hWnd, SW_SHOW | SW_NORMAL);
		//ShowCursor(TRUE); // show the mouse cursor
	}
	//else
	//{
	//	//DEVMODE dmScreenSettings;
	//	//memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

	//	//dmScreenSettings.dmSize         = sizeof(dmScreenSettings);     
	//	//dmScreenSettings.dmPelsWidth    = SCREEN_WIDTH;
	//	//dmScreenSettings.dmPelsHeight   = SCREEN_HEIGHT;
	//	//dmScreenSettings.dmBitsPerPel   = 16;
	//	//dmScreenSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	//	//ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	//	/*SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
	//	SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP);*/
	//	UpdateWindow(m_hWnd);
	//	ShowCursor(FALSE); // hide the mouse cursor
	//}

	//SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);
	ResetDevice();
	//	Set the window to the middle of the screen.
	if (bWindowed)
	{
		// Setup the desired client area size
		RECT rWindow;
		rWindow.left	= 0;
		rWindow.top		= 0;
		rWindow.right	= nScreenWidth;
		rWindow.bottom	= nScreenHeight;

		// Get the dimensions of a window that will have a client rect that
		// will really be the resolution we're looking for.
		//AdjustWindowRectEx(&rWindow, 
		//	dwWindowStyleFlags,
		//	FALSE, 
		//	WS_EX_APPWINDOW);

		// Calculate the width/height of that window's dimensions
		int windowWidth		= rWindow.right - rWindow.left;
		int windowHeight	= rWindow.bottom - rWindow.top;

		SetWindowPos(m_hWnd, HWND_TOP,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (windowWidth>>1),
			(GetSystemMetrics(SM_CYSCREEN)>>1) - (windowHeight>>1),
			windowWidth, windowHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	//else
	//{
	//	// Let windows know the window has changed.
	//	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );
	//}

	


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OnLostDevice(): Called on a device reset.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::OnLostDevice(void)
{
	//Called when changing the display parameters, DirectX required
	dxFont->OnLostDevice();
	SpotLightEffect->OnLostDevice();
	PointLightEffect->OnLostDevice();
	DirectionalLightEffect->OnLostDevice();
	GBufferEffect->OnLostDevice();
	CubeEffect->OnLostDevice();
	TextureMeshEffect->OnLostDevice();
	AnimatedMeshEffect->OnLostDevice();	
	//m_pParticleManager->GetEfect()->OnLostDevice();
	m_DepthOfField.GetEffect()->OnLostDevice();
	m_BlurHorizontal.GetEffect()->OnLostDevice();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OnResetDevice(): Called on a device reset.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::OnResetDevice(void)
{
	//Called when changing the display parameters, DirectX required
	dxFont->OnResetDevice();
	AnimatedMeshEffect->OnResetDevice();
	TextureMeshEffect->OnResetDevice();
	CubeEffect->OnResetDevice();	
	GBufferEffect->OnResetDevice();
	DirectionalLightEffect->OnResetDevice();
	PointLightEffect->OnResetDevice();
	SpotLightEffect->OnResetDevice();
	//m_pParticleManager->GetEfect()->OnResetDevice();
	m_DepthOfField.GetEffect()->OnResetDevice();
	m_BlurHorizontal.GetEffect()->OnResetDevice();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitVertexBuffer(): Creates a new vertex buffer based on the passed in size, created to protect the DRX COM.
//                       
// Returns: IDirect3DVertexBuffer* - The newly created buffer on the GPU.
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IDirect3DVertexBuffer9* RenderEngine::InitVertexBuffer( unsigned int _size)
{
	//Create a vertex buffer then return a pointer to the new buffer
	IDirect3DVertexBuffer9* _buffer;
	m_pD3Dev->CreateVertexBuffer( _size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &_buffer, 0 );
	return _buffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitIndexBuffer(): Creates a new index buffer based on the passed in size, protects the DRX COM object.
//                       
// Returns: IDirect3DIndexBuffer9* - The newly created index buffer.
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IDirect3DIndexBuffer9* RenderEngine::InitIndexBuffer( unsigned int _size)
{
	//Create an index buffer then return a pointer to the new buffer
	IDirect3DIndexBuffer9* _buffer;
	m_pD3Dev->CreateIndexBuffer( _size, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_buffer, 0 );
	return _buffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddToObjectRenderSet(): Adds the passed in render node to the object render set, also adding the matrix to that
//                       render nodoes vector. This render set is cleared every frame.
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::AddToObjectRenderSet( RenderNode* _node, D3DXMATRIX _mat )
{
	//_node->renderData.m_vTransform.clear();
	

	if( _node->m_pOwner->TestFlag( RE_BLEND ) || _node->m_pOwner->TestFlag( RE_TRANSPARENT ) || _node->m_pOwner->TestFlag( RE_FORWARD ) )
	{
		if(_node->renderData.m_vTransform.size() == 0 )
			_node->renderData.m_vTransform.push_back( _mat );
		m_ObjForwardRenderSet.AddRenderNode( _node );
		return;
	}
	//Take a passed in renderNode and add it to the current render set
	_node->renderData.m_vTransform.push_back( _mat );
	m_ObjectRenderSet.AddRenderNode( _node );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddToEnvironmentRenderSet(): Adds the passed in render node to the environment render set.
//                       This render set is never cleared unless changing levels. 
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::AddToEnvironmentRenderSet( RenderNode* _node )
{
	if( _node->m_pOwner->TestFlag( RE_BLEND ) || _node->m_pOwner->TestFlag( RE_TRANSPARENT ) || _node->m_pOwner->TestFlag( RE_FORWARD ) )
	{
		m_EnviForwardRenderSet.AddRenderNode( _node );
		return;
	}
	m_EnvironmentRenderSet.AddRenderNode( _node );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddGlowToDraw(): Adds a glow effect to the vector to be rendered. 
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::AddGlowToDraw( D3DXMATRIX _transform, int _texture )
{
	Glow_Info test;
	test._transform = _transform;
	test._nTextHandle = _texture;
	m_vGlow.push_back( test );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadShaders(): Loads all shaders needed for rendering
//                       
// Returns: voids
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::LoadShaders(void)
{
	ID3DXBuffer	*errors(nullptr);

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/Cube.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &CubeEffect, &errors);
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/Cube.fx", 0, 0, 0, m_pEffectPool, &CubeEffect, &errors);
#endif
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );	

	errors = nullptr;

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/texture.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &TextureMeshEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/texture.fx", 0, 0, 0, m_pEffectPool, &TextureMeshEffect, &errors );
#endif
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

	errors = nullptr;

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/AnimatedTexture.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &AnimatedMeshEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/AnimatedTexture.fx", 0, 0, 0, m_pEffectPool, &AnimatedMeshEffect, &errors );
#endif

	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/GBuffer.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &GBufferEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/GBuffer.fx", 0, 0, 0, m_pEffectPool, &GBufferEffect, &errors );
#endif

	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/DirectionalLight.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &DirectionalLightEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/DirectionalLight.fx", 0, 0, 0, m_pEffectPool, &DirectionalLightEffect, &errors );
#endif

	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/PointLight.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &PointLightEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/PointLight.fx", 0, 0, 0, m_pEffectPool, &PointLightEffect, &errors );
#endif

	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

#if _DEBUG
	HRESULT hr = D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/SpotLight.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &SpotLightEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/SpotLight.fx", 0, 0, 0, m_pEffectPool, &SpotLightEffect, &errors );
#endif

	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

	SAFE_RELEASE( errors );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BuildScreenMesh(): Creates the mesh required for rastering texture.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::BuildScreenMesh(void)
{
	VERTEX_POSNORMTANUV* verts = new VERTEX_POSNORMTANUV[4];
	verts[0].m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f );
	verts[1].m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f );
	verts[2].m_Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f );
	verts[3].m_Position = D3DXVECTOR3(1.0f, 0.0f, 0.0f );
	verts[0].m_TextureCoord = D3DXVECTOR2(0.0f, 1.0f);
	verts[1].m_TextureCoord = D3DXVECTOR2(0.0f, 0.0f);
	verts[2].m_TextureCoord = D3DXVECTOR2(1.0f, 0.0f);
	verts[3].m_TextureCoord = D3DXVECTOR2(1.0f, 1.0f);

	DWORD* cubeIB = new DWORD[6];
	//Front
	cubeIB[0] = 0;
	cubeIB[1] = 1;
	cubeIB[2] = 2;
	cubeIB[3] = 0;
	cubeIB[4] = 2;
	cubeIB[5] = 3;

	screenMesh = MeshManager::GetInstance()->AddVerts( verts, 4, sizeof(VERTEX_POSNORMTANUV), cubeIB, 6 );

	verts[0].m_Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f );
	verts[1].m_Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f );
	verts[2].m_Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f );
	verts[3].m_Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f );

	postprocessMesh = MeshManager::GetInstance()->AddVerts( verts, 4, sizeof(VERTEX_POSNORMTANUV), cubeIB, 6 );

	delete[] verts;
	delete[] cubeIB;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateVertexDeclerations(): Creates and initializes all needed vertex declerations
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::CreateVertexDeclerations(void)
{
	//The vertex decleration for position, normal, tangent, and 1 UV cord
	D3DVERTEXELEMENT9 POSNORMTANUVdecl[] = 
	{ 
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0,24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,0},
		{0,36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		//{0,44, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		D3DDECL_END()
	};

	//Create the vertex decleration for position, normal, tangent, and 1 UV cord
	m_pD3Dev->CreateVertexDeclaration(POSNORMTANUVdecl, &m_pPosNormTanUVdecl);

	D3DVERTEXELEMENT9 POSCOLORdecl[] =
	{
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	m_pD3Dev->CreateVertexDeclaration(POSCOLORdecl, &m_pPosColor );

	//The vertex decleration for position, normal, tangent, weights, indices, and 1 UV cord
	D3DVERTEXELEMENT9 POSNORMTANUVINFdecl[] = 
	{ 
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0,24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,0},
		{0,36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0,44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0,60, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};

	//Create the vertex decleration for position, normal, tangent, and 1 UV cord
	m_pD3Dev->CreateVertexDeclaration(POSNORMTANUVINFdecl, &m_pPosNormTanUVInfdecl);
}

void RenderEngine::CreateRenderTargets(void)
{
	m_rtGDepth.Create( SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_R32F );
	m_rtGNormal.Create( SCREEN_WIDTH, SCREEN_HEIGHT );
	m_rtGDiffuse.Create( SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_A8R8G8B8 );
	m_rtGSpecular.Create( SCREEN_WIDTH, SCREEN_HEIGHT );
	m_rtGEmissive.Create( SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_A8R8G8B8 );
}



void RenderEngine::AddLight( RenderNode* pNode )
{
	//m_LightRenderSet.AddRenderNode( pNode );
}

void RenderEngine::CreateLights(void)
{
	//LightManager::GetInstance()->LoadLights( "./Resources/LevelData/TestLights.bslights" );
	return;
}

void RenderEngine::RenderLights(void)
{
	DirectionalLightEffect->SetTexture( "diffuseGBuffer", m_rtGDiffuse.GetTexture() );
	DirectionalLightEffect->SetTexture( "normalGBuffer", m_rtGNormal.GetTexture() );
	DirectionalLightEffect->SetTexture( "specularGBuffer", m_rtGSpecular.GetTexture() );
	DirectionalLightEffect->SetTexture( "depthGBuffer", m_rtGDepth.GetTexture() );

	//PointLightEffect->SetTexture( "diffuseGBuffer", m_rtGDiffuse.GetTexture() );
	//PointLightEffect->SetTexture( "normalGBuffer", m_rtGNormal.GetTexture() );
	//PointLightEffect->SetTexture( "specularGBuffer", m_rtGSpecular.GetTexture() );
	//PointLightEffect->SetTexture( "depthGBuffer", m_rtGDepth.GetTexture() );

	//SpotLightEffect->SetTexture( "diffuseGBuffer", m_rtGDiffuse.GetTexture() );
	//SpotLightEffect->SetTexture( "normalGBuffer", m_rtGNormal.GetTexture() );
	//SpotLightEffect->SetTexture( "specularGBuffer", m_rtGSpecular.GetTexture() );
	//SpotLightEffect->SetTexture( "depthGBuffer", m_rtGDepth.GetTexture() );

	////m_rtGEmissive.ActivateTarget(0);
	if( m_pCamera )
		LightManager::GetInstance()->ApplyLights();
	////m_rtGEmissive.RevertTarget();
	return;
	//m_rtGEmissive.ActivateTarget( 0 );
	//if( m_pCamera )
	//	DrawDirectionalLight( m_Light );
	//
	//if( m_pCamera )
	//{
	//	DrawPointLight( m_PointLight );
	//	DrawSpotLight( m_SpotLight );
	//}
	//m_rtGEmissive.RevertTarget();
	
}

////////////////////////////////////////////////////////////////////////////////////
//	SetGamma():		Used to set the new gamma if it changes.
//
//	Returns:		void
//
//	Mod Date:		6/11/12
//	Mod Initials:	JM
////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::SetGamma(float gammaFactor)
{
	//D3DGAMMARAMP newRamp;
	////D3DGAMMARAMP oldRamp; m_pD3Dev->GetGammaRamp(0, &oldRamp);
	//D3DCAPS9 caps;
	//
	////fill out the caps9 we created
	//m_pD3Dev->GetDeviceCaps(&caps);
	//
	////if the D3DCAPS9::Caps2 has its "FULLSCREENGAMMA" on then we are changing the gamma
	//if(CGame::GetInstance()->GetFullScreen())
	//{
	//	//clamp the gammaFactor to a -1 to 1 range
	//	if(gammaFactor < -0.5f)
	//	{
	//		gammaFactor = -0.5f;
	//	}
	//	if(gammaFactor > 0.5f)
	//	{
	//		gammaFactor = 0.5f;
	//	}
	//
	//	//for each position in the new ramp calculate the new gamma
	//	for(unsigned int i = 0; i <= 255; ++i)
	//	{
	//		newRamp.red[i] = (unsigned short)(min(255, i * (gammaFactor + 1.0f))) << 8;
	//		newRamp.green[i] = (unsigned short)(min(255, i * (gammaFactor + 1.0f))) << 8;
	//		newRamp.blue[i] = (unsigned short)(min(255, i * (gammaFactor + 1.0f))) << 8;
	//	}
	//
	//	//if the gamma can be calibrated, set it with calibration, otherwise set it without it
	//	if(caps.Caps2 & D3DCAPS2_CANCALIBRATEGAMMA/* && caps.Caps2 & D3DCAPS2_FULLSCREENGAMMA*/)
	//	{
	//		m_fGammaFactor = gammaFactor;
	//		m_pD3Dev->SetGammaRamp(0, D3DSGR_CALIBRATE, &newRamp);
	//	}
	//	else
	//	{
	//		m_fGammaFactor = gammaFactor;
	//		m_pD3Dev->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &newRamp);
	//	}
	//}
}

void RenderEngine::ClearData(void)
{
	m_ObjectRenderSet.ClearRenderSet();
	m_EnvironmentRenderSet.ClearRenderSet();
	m_ObjForwardRenderSet.ClearRenderSet();
	m_EnviForwardRenderSet.ClearRenderSet();
	m_ObjForwardRenderSet.DeleteCopyMemoryPool();
	m_ObjectRenderSet.DeleteCopyMemoryPool();
	m_EnvironmentRenderSet.DeleteCopyMemoryPool();
	m_EnviForwardRenderSet.DeleteCopyMemoryPool();
}

void RenderEngine::ResetBurn(void)
{
	m_fBurnFactor = 0.95f;
}