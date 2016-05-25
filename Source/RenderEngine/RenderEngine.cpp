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
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../PS/States/GamePlayState.h"
#include "../EffectsLib/ParticleManager.h"
#include "../Utilities/CollisionLib/CollisionFrustum.h"
#include "LightSystem\LightManager.h"
#include "LightSystem\Light.h"
#include "LightSystem\PointLight.h"
#include "LightSystem\DirectionalLight.h"
#include "LightSystem\SpotLight.h"

#include "../LevelManager/CLevelManager.h"

#include "DebugShapes.h"

//Default values for all static object within the class

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
RenderTarget					RenderEngine::m_rtSharp;

#if DEPTHOFFIELD	
	PostProcess					RenderEngine::m_DepthOfField;
	PostProcess					RenderEngine::m_BlurH;
	PostProcess					RenderEngine::m_BlurV;
#endif

PostProcess						RenderEngine::m_DownSample;
PostProcess						RenderEngine::m_UpSample;
PostProcess						RenderEngine::m_DrawEdges;
PostProcessChain				RenderEngine::m_PostProcessChain;
PostProcessChain				RenderEngine::m_EmissiveChain;

CParticleManager*				RenderEngine::m_pParticleManager;

//#if DEBUGSHAPES && !DEFERREDRENDER
	DebugShapes					RenderEngine::m_oDebugShapes;
	std::vector<D3DXMATRIX>		RenderEngine::m_vSphereMats;
	std::vector<D3DXMATRIX>		RenderEngine::m_vBoxMats;
//#endif

RenderHandle					RenderEngine::screenMesh;
RenderHandle					RenderEngine::postprocessMesh;

CCamera*						RenderEngine::m_pCamera(nullptr);
VERTEX_POSCOLOR*				RenderEngine::m_pLineVertexs(nullptr);
DWORD*							RenderEngine::m_pDwordIndex(nullptr);
std::vector<Glow_Info>			RenderEngine::m_vGlow;
CCollisionFrustum				RenderEngine::ViewFrustum;

DirectionalLight*				RenderEngine::m_Light = nullptr;
PointLight*						RenderEngine::m_PointLight = nullptr;
SpotLight*						RenderEngine::m_SpotLight = nullptr;

D3DXVECTOR2						RenderEngine::m_v2fScrollOffset;

float							RenderEngine::m_fGammaFactor(0.0f);
float							RenderEngine::m_fPlayerHealth(0.0f);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitRenderEngine(): Starts up the render engine and inits all managers necessary.
//                       
// Returns:            bool - returns true if the engine started up
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RenderEngine::InitRenderEngine(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bFullScreen, bool bVsync, D3DXVECTOR3 _v3ClearColor )
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
	m_d3dPresentParams.BackBufferFormat				= (!bFullScreen) ? D3DFMT_UNKNOWN : D3DFMT_X8R8G8B8;
	m_d3dPresentParams.BackBufferCount				= 1;
	m_d3dPresentParams.Windowed						= !bFullScreen;
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

	//Init all post process effects and setup the swap chains
	m_PostProcessChain.Initialize();
	m_EmissiveChain.Initialize();
	m_DownSample.Initialize(	&postprocessMesh, L"./Resources/Shaders/DownSample.fx" );
	m_UpSample.Initialize(		&postprocessMesh, L"./Resources/Shaders/UpSample.fx" );
	m_DrawEdges.Initialize(		&postprocessMesh, L"./Resources/Shaders/DrawEdges.fx" );

#if DEPTHOFFIELD
	//Depth of field chain setup
	m_DepthOfField.Initialize(	&postprocessMesh, L"./Resources/Shaders/DepthOfField.fx" );
	m_BlurH.Initialize(			&postprocessMesh, L"./Resources/Shaders/PPBlurH.fx" );
	m_BlurV.Initialize(			&postprocessMesh, L"./Resources/Shaders/PPBlurV.fx" );
	m_PostProcessChain.AddPostProcess( &m_BlurH );
	m_PostProcessChain.AddPostProcess( &m_BlurV );
	m_PostProcessChain.AddPostProcess( &m_BlurH );
	m_PostProcessChain.AddPostProcess( &m_BlurV );
	m_PostProcessChain.AddPostProcess( &m_DepthOfField );
#else
	//Regular gameplay setup
	m_PostProcessChain.AddPostProcess( &m_DrawEdges );
#endif

#if DEBUGSHAPES && !DEFERREDRENDER
	//In debug allow debug shapes to be created
	m_oDebugShapes.InitShapes();
#endif
	//Create all the render targets for the deferred render process
	CreateRenderTargets();
	//Init the gama setting
	m_fGammaFactor = 0.0f;
	//Texture handle used for blending between textures
	m_pDissolveTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/Clouds.png" );// );
	//Pointer to the particle manager to render all particles to the screen
	m_pParticleManager = CParticleManager::GetInstance();
	//We were successful tell everyone!
	LOG(LOG_INFO, "RenderEngine::InitRenderEngine()", "RenderEngine was initialized!!");
	//bye bye
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
	//Delete all Render Targets
	m_rtGDepth.DeleteTarget();
	m_rtGNormal.DeleteTarget();
	m_rtGDiffuse.DeleteTarget();
	m_rtGSpecular.DeleteTarget();
	m_rtSharp.DeleteTarget();
	//Release all shaders loaded for post process effects
#if DEPTHOFFIELD
	m_DepthOfField.GetEffect()->Release();
	m_BlurH.GetEffect()->Release();
	m_BlurV.GetEffect()->Release();
#endif
	m_DrawEdges.GetEffect()->Release();
	m_DownSample.GetEffect()->Release();
	m_UpSample.GetEffect()->Release();
	//Shutdown our mesh manager
	MeshManager::GetInstance()->ShutdownMeshManager();
	//Shutdown our texture manager
	TextureManager::GetInstance()->ShutdownTextureManager();
	//Clear all rendersets and their memory pools
	ClearData();
	
	//Delete the memory allocated for drawing a line to the screen
	delete[] m_pLineVertexs;
	delete[] m_pDwordIndex;

	//Release all loaded DirectX objects
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
	SAFE_RELEASE( m_pD3D );
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
	//Create the bulr offsets for down and up sampling
	float width = (float)CGame::GetInstance()->GetScreenWidth();
	float height = (float)CGame::GetInstance()->GetScreenHeight();
	D3DXVECTOR4 PixelCoordsDownFilter[16] =
	{
		D3DXVECTOR4( 1.5f/width,  -1.5f/height, 0, 0),
		D3DXVECTOR4( 1.5f/width,  -0.5f/height, 0, 0),
		D3DXVECTOR4( 1.5f/width,   0.5f/height, 0, 0),
		D3DXVECTOR4( 1.5f/width,   1.5f/height, 0, 0),
		D3DXVECTOR4( 0.5f/width,  -1.5f/height, 0, 0),
		D3DXVECTOR4( 0.5f/width,  -0.5f/height, 0, 0),
		D3DXVECTOR4( 0.5f/width,   0.5f/height, 0, 0),
		D3DXVECTOR4( 0.5f/width,   1.5f/height, 0, 0),
		D3DXVECTOR4(-0.5f/width,  -1.5f/height, 0, 0),
		D3DXVECTOR4(-0.5f/width,  -0.5f/height, 0, 0),
		D3DXVECTOR4(-0.5f/width,   0.5f/height, 0, 0),
		D3DXVECTOR4(-0.5f/width,   1.5f/height, 0, 0),
		D3DXVECTOR4(-1.5f/width,  -1.5f/height, 0, 0),
		D3DXVECTOR4(-1.5f/width,  -0.5f/height, 0, 0),
		D3DXVECTOR4(-1.5f/width,   0.5f/height, 0, 0),
		D3DXVECTOR4(-1.5f/width,   1.5f/height, 0, 0),
	};
	//Send the offsets to the shader
	m_DownSample.GetEffect()->SetVectorArray(
		"TexelCoordsDownFilterX4", PixelCoordsDownFilter, 16);
	//Send the blur weights to the shader
#if DEPTHOFFIELD
	m_BlurH.GetEffect()->SetFloat( "gCenterBlurWeights", CalcGaussianBlurWeight(0));

	D3DXVECTOR4 blurWeights(CalcGaussianBlurWeight(1), CalcGaussianBlurWeight(2), CalcGaussianBlurWeight(3), CalcGaussianBlurWeight(4));
	m_BlurH.GetEffect()->SetVector( "gBlurWeights", &blurWeights );
#endif

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
	//Controls for setting up depth of field
#if DEPTHOFFIELD
	static float focusDistance = 0.0f;

	if( CGame::GetInstance()->GetInputManager()->KeyDown( DIK_W ) )
		focusDistance += 0.01f;
	if( CGame::GetInstance()->GetInputManager()->KeyDown( DIK_S ) )
		focusDistance -= 0.01f;
#endif 
	//Set needed shader globals
	SetShaderGlobals();
	//Increment the UV scrolling
	m_v2fScrollOffset.x += SCROLL_SPEED;
	m_v2fScrollOffset.y += SCROLL_SPEED;

#if DEFERREDRENDER
	//Render the depth values
	RenderDepthBuffer();
	//If playing the game render the G Buffers - nomal/diffuse/spec/emissive
	if( m_pCamera )
		RenderGBuffers();
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

#if DEFERREDRENDER
	//Render to the sharp texture
	m_rtSharp.ActivateTarget( 0 );
	//Clear the sharp texture
	Clear();		
	//Render the lights to the final render target - sharp texture
	RenderLights();	
	//if debug shapes are being used render them now
#if DEBUGSHAPES	
	m_oDebugShapes.RenderSpheres( m_vSphereMats );
	m_oDebugShapes.RenderBoxs( m_vBoxMats );
	m_vSphereMats.clear();
	m_vBoxMats.clear();
#endif	
	m_rtSharp.RevertTarget();
	//kellybooooooboooooooooooobryanboooooboooooooooooooobellaboooboooooooo<3

	//Activate the render targets needed for the forward pass
	m_rtSharp.ActivateTarget( 0 );
	m_rtGDepth.ActivateTarget( 1 );
	m_rtGNormal.ActivateTarget( 2 );
	//In case of failed depth passes on objects use what has already been rendered
	TextureMeshEffect->SetTexture( "finalMap", m_rtSharp.GetTexture() );
	//Get the beginning of the forward pass set
	RenderNode* forwardPass = m_ObjForwardRenderSet.GetHead();
	//While there is something to render
	while( forwardPass )
	{
		//Call its render function
		forwardPass->RenderProcess();
		//Go to the next object
		forwardPass = forwardPass->GetNext();
	}
	//Get the beginning of the forward pass set
	forwardPass = m_EnviForwardRenderSet.GetHead();
	//While there is something to render
	while( forwardPass )
	{
		//Call its render function
		forwardPass->RenderProcess();
		//Go to the next object
		forwardPass = forwardPass->GetNext();
	}
	//Clear the forward sets
	m_ObjForwardRenderSet.ClearRenderSet();
	m_ObjectRenderSet.ClearRenderSet();

	//Draw any tutorial posters or any other object in global space to a plane
#if DEFERREDRENDER
	for( unsigned int i = 0u; i < m_vGlow.size(); ++i )
		DrawTexturedPlane( m_vGlow[i]._transform, TextureManager::GetInstance()->m_umTextureMap[ m_vGlow[i]._nTextHandle ].m_lpTexture );

	m_vGlow.clear();
#endif
	//If in gameplay render all the particles
	if( m_pCamera )
		m_pParticleManager->Render( m_pCamera );
	//Revert targets after completing forward pass
	m_rtGNormal.RevertTarget();
	m_rtGDepth.RevertTarget();
	m_rtSharp.RevertTarget();
	//Place the sharp texture onto the post process chains texture
	m_PostProcessChain.GetActiveRenderTarget()->ActivateTarget( 0 );
	DrawFinalRender( m_rtSharp.GetTexture() );
	m_PostProcessChain.GetActiveRenderTarget()->RevertTarget();
	//Set values needed for depth of field
#if DEPTHOFFIELD
	m_DepthOfField.GetEffect()->SetFloat( "gPlaneX", float( -4000.0f / (4000.0f - 0.1f )));
	m_DepthOfField.GetEffect()->SetFloat( "gPlaneY", float( -4000.0f * 0.1f / (4000.0f - 0.1f )));
	//Only do draw edges on objects in focus
	m_DrawEdges.Process( m_rtSharp, m_rtSharp.GetTexture() );
	//Send the texture to be used for in focus objects
	m_DepthOfField.GetEffect()->SetTexture( "sharpTexture", m_rtSharp.GetTexture() );
	m_DepthOfField.GetEffect()->SetFloat( "gFocusDistance", focusDistance );
#endif
	//Run any post process effects setup in the current swap chain
	m_PostProcessChain.Run();
	//Draw the completed scene
	DrawFinalRender( m_PostProcessChain.GetActiveRenderTarget()->GetTexture() );
	//If begugging show the FPS to the screen
//#if _DEBUG
	//Display the current FPS to the screen
	char text[64];
	wsprintfW(LPWSTR(text), L"FPS: %d", m_unFPS);
	RECT fontRec = {0,0,250,250};
	dxFont->DrawTextW(0, LPWSTR(text), -1, &fontRec, 0, D3DCOLOR_XRGB(255,255,255));
//#endif
#endif
	//Display all the render target buffers to the screen
#if (SHOWRENDERTARGETS && DEFERREDRENDER)
	RasterTexture( m_rtGDepth.GetTexture(), 0.0f, 0.0f, 0.2f, 0.2f );
	RasterTexture( m_rtGNormal.GetTexture(), 0.2f, 0.0f, 0.4f, 0.2f );
	RasterTexture( m_rtGDiffuse.GetTexture(), 0.4f, 0.0f, 0.6f, 0.2f );
	RasterTexture( m_rtGSpecular.GetTexture(), 0.6f, 0.0f, 0.8f, 0.2f );
	RasterTexture( m_rtSharp.GetTexture(), 0.8f, 0.0f, 1.0f, 0.2f );
#endif
}

void RenderEngine::RenderGBuffers(void)
{
	//Activate our targets to store the G buffer data - normal/diffuse/specular/emissive
	m_rtGDiffuse.ActivateTarget( 0 );
	m_rtGNormal.ActivateTarget( 1 );
	m_rtGSpecular.ActivateTarget( 2 );
	m_EmissiveChain.GetActiveRenderTarget()->ActivateTarget( 3 );
	//Clear my targets
	m_pD3Dev->Clear(0,0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1, 0);
	//Get the beginning of the render set
	RenderNode* environmentSet = m_EnvironmentRenderSet.GetHead();
	//Loop through all nodes within the render set - STATIC OBJECTS//LIST NEVER CLEARED!
	while( environmentSet )
	{
		//If I need to be rendered without lights change me
		if( environmentSet->m_pOwner->TestFlag( RE_FORWARD ) )
		{
			//Skip me
			environmentSet = environmentSet->GetNext();
		}
		else
		{
			//Get my collision volume for frustrum culling
			CCollisionAABB aabb = (*environmentSet).renderData.m_vCollisions[0];
			//if in the frustum render it
			if( ViewFrustum.infrustum(  aabb ))
			{
				//Call the current nodes render function
				DrawGBuffers(*environmentSet);
			}
			//Go to the next render node
			environmentSet = environmentSet->GetNext();
		}
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
	//Revert all my render targets
	m_EmissiveChain.GetActiveRenderTarget()->RevertTarget();
	m_rtGSpecular.RevertTarget();
	m_rtGNormal.RevertTarget();
	m_rtGDiffuse.RevertTarget();
}

void RenderEngine::RenderDepthBuffer(void)
{
	//Make my depth buffer the render target
	m_rtGDepth.ActivateTarget( 0 );
	//Clear the depth buffer
	m_pD3Dev->Clear(0,0, D3DCLEAR_TARGET , D3DCOLOR_XRGB(255,255,255), 1.0f, 0);	

	//Get the beginning of the render set
	RenderNode* environmentSet = m_EnvironmentRenderSet.GetHead();
	//Loop through all node within the render set - STATIC OBJECTS//LIST NEVER CLEARED!
	while( environmentSet )
	{
		//If I am not lit dont render me
		if( environmentSet->m_pOwner->TestFlag( RE_FORWARD ) )
		{
			//Skip me please
			environmentSet = environmentSet->GetNext();
		}
		else
		{
			//Get my collision volume
			CCollisionAABB aabb = (*environmentSet).renderData.m_vCollisions[0];
			//If I am in the frustrum render me
			if( ViewFrustum.infrustum(  aabb ))
			{
				//Call the current nodes render function
				DrawDepth(*environmentSet);
			}
			//Go to the next render node
			environmentSet = environmentSet->GetNext();
		}
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
	//Revert my depth buffer target
	m_rtGDepth.RevertTarget();
}

void RenderEngine::ResetDevice(void)
{
	//Delete my render targets
	m_rtGDepth.DeleteTarget();
	m_rtGNormal.DeleteTarget();
	m_rtGDiffuse.DeleteTarget();
	m_rtGSpecular.DeleteTarget();
	m_rtSharp.DeleteTarget();
	//Delete my chain render targets
	m_PostProcessChain.Clear();
	m_EmissiveChain.Clear();
	//Release all textures
	TextureManager::GetInstance()->UnloadAllTextures();
	//Call LostDevice on all COM objects
	OnLostDevice();	
	//Reset the device and if needs to try again do it
	if( m_pD3Dev->Reset(&m_d3dPresentParams) != D3D_OK )
		return;
	//Reset all COM objects
	OnResetDevice();
	//Reload all textures that were loaded before
	TextureManager::GetInstance()->ReloadAllTextures();
	//My render targets must come back to life!
	CreateRenderTargets();
	//Create the render targets for the post process chains
	m_PostProcessChain.Initialize();	
	m_EmissiveChain.Initialize();
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
void RenderEngine::ChangeDisplayParam( int nScreenWidth, int nScreenHeight, bool bFullScreen, bool bVsync )
{
	//Change the display parameters
	m_d3dPresentParams.BackBufferWidth				= nScreenWidth;
	m_d3dPresentParams.BackBufferHeight				= nScreenHeight;
	m_d3dPresentParams.BackBufferFormat				= (!bFullScreen) ? D3DFMT_UNKNOWN : D3DFMT_X8R8G8B8;

	m_d3dPresentParams.Windowed						= !bFullScreen;

	m_d3dPresentParams.FullScreen_RefreshRateInHz	= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dPresentParams.PresentationInterval			= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	//Reset the device
	ResetDevice();
	//	Set the window to the middle of the screen.
	if (!bFullScreen)
	{
		// Setup the desired client area size
		RECT rWindow;
		rWindow.left	= 0;
		rWindow.top		= 0;
		rWindow.right	= nScreenWidth;
		rWindow.bottom	= nScreenHeight;

		// Calculate the width/height of that window's dimensions
		int windowWidth		= rWindow.right - rWindow.left;
		int windowHeight	= rWindow.bottom - rWindow.top;

		SetWindowPos(m_hWnd, HWND_TOP,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (windowWidth>>1),
			(GetSystemMetrics(SM_CYSCREEN)>>1) - (windowHeight>>1),
			windowWidth, windowHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
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
	//WE MUST SAVE EVERYTHING!!
	//Called when changing the display parameters, DirectX required
	dxFont->OnLostDevice();
	SpotLightEffect->OnLostDevice();
	PointLightEffect->OnLostDevice();
	DirectionalLightEffect->OnLostDevice();
	GBufferEffect->OnLostDevice();
	CubeEffect->OnLostDevice();
	TextureMeshEffect->OnLostDevice();
	AnimatedMeshEffect->OnLostDevice();	
	m_pParticleManager->GetEfect()->OnLostDevice();

#if DEPTHOFFIELD
	m_DepthOfField.GetEffect()->OnLostDevice();
	m_BlurH.GetEffect()->OnLostDevice();
	m_BlurV.GetEffect()->OnLostDevice();
#endif

	m_DownSample.GetEffect()->OnLostDevice();
	m_UpSample.GetEffect()->OnLostDevice();
	m_DrawEdges.GetEffect()->OnLostDevice();
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
	//BRING EVERYTHING BACK TO LIFE!!
	//Called when changing the display parameters, DirectX required
	dxFont->OnResetDevice();
	AnimatedMeshEffect->OnResetDevice();
	TextureMeshEffect->OnResetDevice();
	CubeEffect->OnResetDevice();	
	GBufferEffect->OnResetDevice();
	DirectionalLightEffect->OnResetDevice();
	PointLightEffect->OnResetDevice();
	SpotLightEffect->OnResetDevice();
	m_pParticleManager->GetEfect()->OnResetDevice();

#if DEPTHOFFIELD
	m_DepthOfField.GetEffect()->OnResetDevice();
	m_BlurH.GetEffect()->OnResetDevice();
	m_BlurV.GetEffect()->OnResetDevice();
#endif

	m_DownSample.GetEffect()->OnResetDevice();
	m_UpSample.GetEffect()->OnResetDevice();
	m_DrawEdges.GetEffect()->OnResetDevice();
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
	//Check if we are supposed to be forward
	if( _node->m_pOwner->TestFlag( RE_BLEND ) || _node->m_pOwner->TestFlag( RE_TRANSPARENT ) || _node->m_pOwner->TestFlag( RE_FORWARD ) )
	{
		if(_node->renderData.m_vTransform.size() == 0 || _node->TestFlag( RE_ANIMATED ) )
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
	//Are we supposed to be forward? If so place me in the right place please
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
	Glow_Info glowToAdd;
	glowToAdd._transform = _transform;
	glowToAdd._nTextHandle = _texture;
	m_vGlow.push_back( glowToAdd );
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
	//Load the shaders used by the engine!
	ID3DXBuffer	*errors(nullptr);
	//Do we want to be fast or debuggable?
#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/Cube.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &CubeEffect, &errors);
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/Cube.fx", 0, 0, 0, m_pEffectPool, &CubeEffect, &errors);
#endif
	//Post my errors
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );	

	errors = nullptr;
	//Do we want to be fast or debuggable?
#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/texture.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &TextureMeshEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/texture.fx", 0, 0, 0, m_pEffectPool, &TextureMeshEffect, &errors );
#endif
	//Post my errors
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

	errors = nullptr;
	//Do we want to be fast or debuggable?
#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/AnimatedTexture.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &AnimatedMeshEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/AnimatedTexture.fx", 0, 0, 0, m_pEffectPool, &AnimatedMeshEffect, &errors );
#endif
	//Post my errors
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );
	//Do we want to be fast or debuggable?
#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/GBuffer.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &GBufferEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/GBuffer.fx", 0, 0, 0, m_pEffectPool, &GBufferEffect, &errors );
#endif
	//Post my errors
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );
	//Do we want to be fast or debuggable?
#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/DirectionalLight.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &DirectionalLightEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/DirectionalLight.fx", 0, 0, 0, m_pEffectPool, &DirectionalLightEffect, &errors );
#endif
	//Post my errors
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );
	//Do we want to be fast or debuggable?
#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/PointLight.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &PointLightEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/PointLight.fx", 0, 0, 0, m_pEffectPool, &PointLightEffect, &errors );
#endif
	//Post my errors
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );
	//Do we want to be fast or debuggable?
#if _DEBUG
	HRESULT hr = D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/SpotLight.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, m_pEffectPool, &SpotLightEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/SpotLight.fx", 0, 0, 0, m_pEffectPool, &SpotLightEffect, &errors );
#endif
	//Post my errors
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
	m_rtGDepth.Create( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), D3DFMT_R32F );
	m_rtGNormal.Create( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight() );
	m_rtGDiffuse.Create( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), D3DFMT_A8R8G8B8 );
	m_rtGSpecular.Create( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight() );
	m_rtSharp.Create( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), D3DFMT_A8R8G8B8 ); 
}

void RenderEngine::BuildFrustum( float FoV, float nearDist, float farDist, float aspectRatio, matrix4f cameraWorld )
{
	ViewFrustum.Build( FoV, nearDist, farDist, aspectRatio, cameraWorld );
}

CCollisionFrustum* RenderEngine::GetFrustum(void)
{
	return &ViewFrustum;
}

void RenderEngine::RenderViewFrustum(void)
{
#if DEBUGSHAPES && !DEFERREDRENDER
	for( unsigned int cornerIndex = 0; cornerIndex < 8; cornerIndex++)
	{
		// Grab the corner
		CCollisionPoint* corner = ViewFrustum.GetCorner(cornerIndex);

		D3DXMATRIX cornerMatrix;
		D3DXMatrixIdentity(&cornerMatrix);

		cornerMatrix._41 = corner->Get3DCentorid().x;
		cornerMatrix._42 = corner->Get3DCentorid().y;
		cornerMatrix._43 = corner->Get3DCentorid().z;

		DebugShapes::RenderSphere( cornerMatrix );

	}
#endif
}

void RenderEngine::RenderLights(void)
{
	DirectionalLightEffect->SetTexture( "emissiveMap", m_EmissiveChain.GetActiveRenderTarget()->GetTexture() );
	DirectionalLightEffect->SetTexture( "diffuseGBuffer", m_rtGDiffuse.GetTexture() );
	DirectionalLightEffect->SetTexture( "normalGBuffer", m_rtGNormal.GetTexture() );
	DirectionalLightEffect->SetTexture( "specularGBuffer", m_rtGSpecular.GetTexture() );
	DirectionalLightEffect->SetTexture( "depthGBuffer", m_rtGDepth.GetTexture() );

	CParticleManager::GetInstance()->GetEfect()->SetTexture( "depthGBuffer", m_rtGDepth.GetTexture() );

	if( m_pCamera )
	{
		LevelManager* m_LvlMng = LevelManager::GetInstance();

		for( int i = 0; i < m_LvlMng->GetNumLevels(); i++)
		{
			CLevel* currRenderLightLevel = m_LvlMng->GetLevelByIndex(i);
			currRenderLightLevel->GetLightManager().ApplyLights();
		}
	}
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
	D3DGAMMARAMP newRamp;
	//D3DGAMMARAMP oldRamp; m_pD3Dev->GetGammaRamp(0, &oldRamp);
	D3DCAPS9 caps;

	//fill out the caps9 we created
	m_pD3Dev->GetDeviceCaps(&caps);

	//if the D3DCAPS9::Caps2 has its "FULLSCREENGAMMA" on then we are changing the gamma
	if(CGame::GetInstance()->GetFullScreen())
	{
		//clamp the gammaFactor to a -1 to 1 range
		if(gammaFactor < -0.5f)
		{
			gammaFactor = -0.5f;
		}
		if(gammaFactor > 0.5f)
		{
			gammaFactor = 0.5f;
		}

		//for each position in the new ramp calculate the new gamma
		for(unsigned int i = 0; i <= 255; ++i)
		{
			newRamp.red[i] = (unsigned short)(min(255, i * (gammaFactor + 1.0f))) << 8;
			newRamp.green[i] = (unsigned short)(min(255, i * (gammaFactor + 1.0f))) << 8;
			newRamp.blue[i] = (unsigned short)(min(255, i * (gammaFactor + 1.0f))) << 8;
		}

		//if the gamma can be calibrated, set it with calibration, otherwise set it without it
		if(caps.Caps2 & D3DCAPS2_CANCALIBRATEGAMMA/* && caps.Caps2 & D3DCAPS2_FULLSCREENGAMMA*/)
		{
			m_fGammaFactor = gammaFactor;
			m_pD3Dev->SetGammaRamp(0, D3DSGR_CALIBRATE, &newRamp);
		}
		else
		{
			m_fGammaFactor = gammaFactor;
			m_pD3Dev->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &newRamp);
		}
	}
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
