#ifndef MULTITHREADRENDER_H_
#define MULTITHREADRENDER_H_

#include "../PS/config.h"

#include "VertexDeclerations.h"
#include "MeshManager.h"


class MultiThreadRender
{
	static MultiThreadRender* m_pInstance;

	MultiThreadRender(void);								// Private so that it can  not be called
	MultiThreadRender(MultiThreadRender const&){}				// copy constructor is private
	MultiThreadRender& operator=(MultiThreadRender const&){}  // assignment operator is private
	~MultiThreadRender(void);

	
	static HINSTANCE m_hInstance;
	static IDirect3D9* m_pD3D;
		   
	static IDirect3DVertexDeclaration9* m_pPosNormTanUVdecl;
	static D3DPRESENT_PARAMETERS m_d3dPresentParams;
	static ID3DXEffect* TextureMeshEffect;
	static IDirect3DVertexBuffer9* m_pScreenVert;
	static IDirect3DIndexBuffer9* m_pScreenIndex;	
	static HWND m_hWnd;
	static IDirect3DDevice9* m_pD3Dev;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BuildScreenMesh(): Creates the mesh required for rastering texture.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildScreenMesh(void);

public:
	static MultiThreadRender* GetInstance(void);
	IDirect3DDevice9* GetDevice(void) { return m_pD3Dev; }
	bool InitRenderEngine(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed, bool bVsync );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ChangeDisplayParam(): Changes the present pararmeters being used by the current DRX COM object.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void ChangeDisplayParam( int nScreenWidth, int nScreenHeight, bool bWindowed, bool bVsync );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Clear(): Clears all the buffers being used by DirectX.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BeginScene(): Begins the scene with DirectX
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BeginScene(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// EndScene(): Ends the scene with DirectX
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void EndScene(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Present(): Presents the currently drawn buffer to the screen based on the current render target.
	//                       
	// Returns:  void          bool - returns true if the engine started up
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Present(void);

	void RasterTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom, float _fadeFactor = 1.0f );
	void Shutdown(void);
};

#endif