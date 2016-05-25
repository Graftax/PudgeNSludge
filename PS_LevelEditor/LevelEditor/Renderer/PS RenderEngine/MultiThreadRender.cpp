#include "MultiThreadRender.h"

MultiThreadRender* MultiThreadRender::m_pInstance = nullptr;

 HINSTANCE						MultiThreadRender::m_hInstance;
 IDirect3D9*					MultiThreadRender::m_pD3D;
 
 IDirect3DVertexDeclaration9*	MultiThreadRender::m_pPosNormTanUVdecl;
 D3DPRESENT_PARAMETERS			MultiThreadRender::m_d3dPresentParams;
 ID3DXEffect*					MultiThreadRender::TextureMeshEffect;
 IDirect3DVertexBuffer9*		MultiThreadRender::m_pScreenVert;
 IDirect3DIndexBuffer9*			MultiThreadRender::m_pScreenIndex;	
 HWND							MultiThreadRender::m_hWnd;
 IDirect3DDevice9*				MultiThreadRender::m_pD3Dev;

MultiThreadRender::MultiThreadRender(void)
{
}


MultiThreadRender::~MultiThreadRender(void)
{
}

MultiThreadRender* MultiThreadRender::GetInstance(void)
{
	if( m_pInstance )
		return m_pInstance;
	else 
	{
		m_pInstance = new MultiThreadRender;
	}
	return m_pInstance;
}

bool MultiThreadRender::InitRenderEngine(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed, bool bVsync )
{
	if( !hWnd ) return false; //Easy out if we did not get a window handle yet...

	m_hWnd				= hWnd;//The current window handle
	m_hInstance			= hInstance;//The HINSTANCE
	m_pD3D				= Direct3DCreate9( D3D_SDK_VERSION );//DirectX object created with the current SDK version installed

	ZeroMemory(&m_d3dPresentParams, sizeof(D3DPRESENT_PARAMETERS));


	//Setup the present params in order to create the DirectX device
	m_d3dPresentParams.BackBufferWidth				= nScreenWidth;
	m_d3dPresentParams.BackBufferHeight				= nScreenHeight;
	m_d3dPresentParams.BackBufferFormat				= (bIsWindowed) ? D3DFMT_UNKNOWN : D3DFMT_R5G6B5;
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
	if( FAILED( hr = m_pD3D->CreateDevice( 0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dPresentParams, &m_pD3Dev ) ))
	{
		MessageBoxW( hWnd, L"Failed to create DRX", NULL, MB_OK );
		return false;
	}

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

	ID3DXBuffer	*errors(nullptr);

#if _DEBUG
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/texture.fx", 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 0, &TextureMeshEffect, &errors );
#else
	D3DXCreateEffectFromFileW( m_pD3Dev, L"./Resources/Shaders/texture.fx", 0, 0, 0, 0, &TextureMeshEffect, &errors );
#endif
	if( errors )
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );

	SAFE_RELEASE( errors );

	BuildScreenMesh();

	//LOG(LOG_INFO, "MultiThreadRender::InitRenderEngine()", "MultiThreadRender was initialized!! :)");
	return true;
}
void MultiThreadRender::ChangeDisplayParam( int nScreenWidth, int nScreenHeight, bool bWindowed, bool bVsync )
{
	m_d3dPresentParams.BackBufferWidth				= nScreenWidth;
	m_d3dPresentParams.BackBufferHeight				= nScreenHeight;
	m_d3dPresentParams.BackBufferFormat				= (bWindowed) ? D3DFMT_UNKNOWN : D3DFMT_R5G6B5;
	m_d3dPresentParams.BackBufferCount				= 1;
	m_d3dPresentParams.Windowed						= bWindowed;
	m_d3dPresentParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_d3dPresentParams.MultiSampleQuality			= NULL;
	m_d3dPresentParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_d3dPresentParams.EnableAutoDepthStencil		= TRUE;
	m_d3dPresentParams.AutoDepthStencilFormat		= D3DFMT_D16;
	m_d3dPresentParams.hDeviceWindow				= m_hWnd;
	m_d3dPresentParams.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_d3dPresentParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	m_d3dPresentParams.PresentationInterval			= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;


	//TextureManager::GetInstance()->UnloadAllTextures();

	TextureMeshEffect->OnLostDevice();

	HRESULT hr = m_pD3Dev->Reset(&m_d3dPresentParams);


	TextureMeshEffect->OnResetDevice();

	while( m_pD3Dev->TestCooperativeLevel() != D3D_OK ) {}
	//Enable the Z buffer to not cause depth artifacts
	//m_pD3Dev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	// Setup window style flags
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	//TextureManager::GetInstance()->ReloadAllTextures();

	if (bWindowed)
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
		ShowCursor(TRUE); // show the mouse cursor
	}
	else
	{
		dwWindowStyleFlags |= WS_POPUP;
		ShowCursor(FALSE); // hide the mouse cursor
	}

	SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);

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
			windowWidth, windowHeight, SWP_SHOWWINDOW);
	}
	else
	{
		// Let windows know the window has changed.
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void MultiThreadRender::Shutdown(void)
{
	//Release all loaded DirectX objects
	SAFE_RELEASE( m_pScreenVert );
	SAFE_RELEASE( m_pScreenIndex );
	SAFE_RELEASE( TextureMeshEffect );
	SAFE_RELEASE( m_pPosNormTanUVdecl );
	SAFE_RELEASE( m_pD3Dev );
	SAFE_RELEASE( m_pD3D );
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
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
void MultiThreadRender::Clear(void)
{
	m_pD3Dev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);
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
void MultiThreadRender::BeginScene(void)
{
	m_pD3Dev->BeginScene();
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
void MultiThreadRender::EndScene(void)
{
	m_pD3Dev->EndScene();
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
void MultiThreadRender::Present(void)
{
	if( m_pD3Dev->TestCooperativeLevel() == D3D_OK )
		m_pD3Dev->Present(0,0,0,0);
}

void MultiThreadRender::BuildScreenMesh(void)
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

	//screenMesh = MeshManager::GetInstance()->AddVerts( verts, 4, sizeof(VERTEX_POSNORMTANUV), cubeIB, 6 );
	m_pD3Dev->CreateVertexBuffer( ( 4 * sizeof( VERTEX_POSNORMTANUV)), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pScreenVert, 0 );
	void* buffer;
	m_pScreenVert->Lock(0,0,&buffer,0);
	memcpy( buffer, verts, sizeof( VERTEX_POSNORMTANUV ) * 4 );
	m_pScreenVert->Unlock();

	m_pD3Dev->CreateIndexBuffer( 6 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pScreenIndex, 0 );
	m_pScreenIndex->Lock( 0, 0, &buffer, 0 );
	memcpy( buffer, cubeIB, sizeof(DWORD) * 6 );
	m_pScreenIndex->Unlock();
	
	delete[] verts;
	delete[] cubeIB;
}

void MultiThreadRender::RasterTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom, float _fadeFactor )
{
	//set rendered texture
	m_pD3Dev->SetTexture(0, _raster );

	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );

	D3DXMATRIX scale;
	D3DXMatrixScaling( &scale, (_right - _left) * 2.0f, (_bottom - _top) * 2.0f, 1.0f );
	_matProj *= scale;

	D3DXMATRIX translate;
	D3DXMatrixTranslation( &translate, (_left * 2) - 1.0f, ( (_top * 2) - 1.0f ), 0.0f );
	_matProj *= translate;

	TextureMeshEffect->SetTechnique("Splash");
	unsigned passes(0);
	TextureMeshEffect->Begin(&passes, 0);

	TextureMeshEffect->SetMatrix("gWVP", &(_matProj));
	TextureMeshEffect->SetTexture("tex1", _raster);
	TextureMeshEffect->SetFloat( "fadeFactor", _fadeFactor );
	for(unsigned j(0); j<passes;++j)
	{
		TextureMeshEffect->BeginPass(j);
		{	
			m_pD3Dev->SetStreamSource( 0, m_pScreenVert, 0, sizeof(VERTEX_POSNORMTANUV) );
			m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
			m_pD3Dev->SetIndices( m_pScreenIndex );
			m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );
		}
		TextureMeshEffect->EndPass();
	}
	TextureMeshEffect->End();
}
