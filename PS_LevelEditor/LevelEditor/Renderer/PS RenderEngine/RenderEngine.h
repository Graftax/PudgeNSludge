//////////////////////////////////////////////////////////////////////////////////////
// Filename:            RenderEngine.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             An engine used to render anything in the game to the screen.
//////////////////////////////////////////////////////////////////////////////////////
#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_

#include "VertexDeclerations.h"
#include "RenderHandle.h"
#include "../Camera.h"

//Forward declare needed classes to prevent circular includes
class MeshManager;
class TextureManager;
class RenderNode;
class RenderSet;
class RenderSetSorted;
class CObjectManager;


class RenderTarget;
class LightManager;

class PostProcess;
class PostProcessChain;


//LIGHTS
class Light;
class PointLight;
class DirectionalLight;
class SpotLight;


class DebugShapes;

#define SCROLL_SPEED 0.03f
#define BURN_RATE 0.009f
#define DEBUGSHAPES 1
#define SHOWRENDERTARGETS 0 //NO LONGER U
#define DEFERREDRENDER 1 // Want lighting? Make this a 1
#define M_E        2.71828182845904523536


//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)			if (p) { p->Release(); p = nullptr; }
#endif
// Macro to make a random float from the min and max
#ifndef RAND_FLOAT
	#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))
#endif

//Static class used for rendering all objects to the screen
__declspec(align(32))
class RenderEngine
{
public:

	friend class MeshManager;
	friend class TextureManager;
	
	friend class RenderTarget;
	friend class Light;
	friend class LightManager;
	
	friend class PostProcess;

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static HWND m_hWnd;
	static HINSTANCE m_hInstance;
	static IDirect3D9* m_pD3D;
	static IDirect3DDevice9* m_pD3Dev;
	static IDirect3DVertexDeclaration9* m_pPosNormTanUVdecl;
	static IDirect3DVertexDeclaration9* m_pPosNormTanUVInfdecl;
	static IDirect3DVertexDeclaration9* m_pPosColor;
	static LPD3DXEFFECTPOOL m_pEffectPool;
	static ID3DXFont *dxFont;
	static int m_pDissolveTexture;

	static UINT m_unFrameCounter;
	static D3DPRESENT_PARAMETERS m_d3dPresentParams;
	static UINT m_unScreenWidth;
	static UINT m_unScreenHeight;
	static D3DXVECTOR3 m_v3ClearColor;
	static D3DXMATRIX m_mMVP;
	static unsigned int m_unFPS;

	static RenderSetSorted m_ObjectRenderSet;
	static RenderSetSorted m_EnvironmentRenderSet;
	static RenderSetSorted m_ObjForwardRenderSet;
	static RenderSetSorted m_EnviForwardRenderSet;

	static ID3DXEffect* CubeEffect;
	static ID3DXEffect* TextureMeshEffect;
	static ID3DXEffect* AnimatedMeshEffect;
	static ID3DXEffect* GBufferEffect;
	static ID3DXEffect* DirectionalLightEffect;
	static ID3DXEffect* PointLightEffect;
	static ID3DXEffect* SpotLightEffect;

	static TextureManager* m_pTextureMan;

	static RenderHandle screenMesh;
	static RenderHandle postprocessMesh;

	static IDirect3DVertexBuffer9* m_pLineVert;
	static IDirect3DIndexBuffer9* m_pLineIndex;	

	static VERTEX_POSCOLOR* m_pLineVertexs;
	static DWORD*			m_pDwordIndex;
	static std::vector<Glow_Info> m_vGlow;

////////////////////////////////////////////////////////////////
	static RenderTarget m_rtGDepth;
	static RenderTarget m_rtGNormal;
	static RenderTarget m_rtGDiffuse;
	static RenderTarget m_rtGSpecular;
	static RenderTarget m_rtGEmissive;

	static PostProcess m_DepthOfField;
	static PostProcess m_BlurHorizontal;
	static PostProcessChain m_PostProcessChain;


	static float m_fGammaFactor;
	static float m_fBurnFactor;

	friend class DebugShapes;
	static DebugShapes m_oDebugShapes;
	static std::vector<D3DXMATRIX> m_vSphereMats;
	static std::vector<D3DXMATRIX> m_vBoxMats;

	static CCamera* m_pCamera;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadShaders(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void LoadShaders(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OnLostDevice(): Called on a device reset.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void OnLostDevice(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OnResetDevice(): Called on a device reset.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void OnResetDevice(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CreateVertexDeclerations(): Creates and initializes all needed vertex declerations
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void CreateVertexDeclerations(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CreateRenderTargets(): Creates and initializes all render targets.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void CreateRenderTargets(void);

	static void SetShaderGlobals(void);

	//Not added by me, used by Rueben for the health component.  
	static bool m_bObjPulled;

	static DirectionalLight* m_Light;
	static PointLight* m_PointLight;
	static SpotLight* m_SpotLight;

	static void CreateLights(void);
	static void RenderLights(void);

	static void DrawDirectionalLight( DirectionalLight& _Light );
	static void DrawPointLight( PointLight& _Light );
	static void DrawSpotLight( SpotLight& _Light );
	static void DrawFinalRender( LPDIRECT3DTEXTURE9 _raster );

	static D3DXVECTOR2 m_v2fScrollOffset;

public:
	RenderEngine(void){}
	~RenderEngine(void){}

	static bool GetObjPulled(void) { return m_bObjPulled; }
	static void SetObjPulled(bool _bObjPulled) { m_bObjPulled = _bObjPulled; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static IDirect3DDevice9* GetDevice(void) { return m_pD3Dev; }

	static RenderHandle GetScreenMesh(void) { return screenMesh; }
	void AddLight( RenderNode* pNode );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BuildScreenMesh(): Creates the mesh required for rastering texture.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void BuildScreenMesh(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InitRenderEngine(): Starts up the render engine and inits all managers necessary.
	//                       
	// Returns:            bool - returns true if the engine started up
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool InitRenderEngine(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed, bool bVsync, D3DXVECTOR3 _v3ClearColor );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ShutdownRenderEngine(): Shuts down the render engine and releases all COM objects.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void ShutdownRenderEngine(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Clear(): Clears all the buffers being used by DirectX.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void Clear(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BeginScene(): Begins the scene with DirectX
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void BeginScene(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// EndScene(): Ends the scene with DirectX
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void EndScene(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Present(): Presents the currently drawn buffer to the screen based on the current render target.
	//                       
	// Returns:  void          bool - returns true if the engine started up
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void Present(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render(): Render all objects currently set to be rendered, loops through both render sets.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void Render(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RenderGBuffer(): Render all objects currently set to be rendered, loops through both render sets.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void RenderDepthBuffer(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RenderGBuffers(): Render all objects currently set to be rendered, loops through both render sets.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void RenderGBuffers(void);
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
	// SetModelViewProjection(): Sets the Model View Projection matrix being used for rendering.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void SetModelViewProjection(D3DXMATRIX mMVP) { m_mMVP = mMVP; }
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SetCamera(): Sets a pointer to the current camera.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void SetCamera(CCamera* _cam) { m_pCamera = _cam; }
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// UpdateFPS(): Used in debug mode for updating the FPS to show to the screen.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void UpdateFPS(int _fps) { m_unFPS = _fps; }
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InitVertexBuffer(): Creates a new vertex buffer based on the passed in size, created to protect the DRX COM.
	//                       
	// Returns: IDirect3DVertexBuffer* - The newly created buffer on the GPU.
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static IDirect3DVertexBuffer9* InitVertexBuffer( unsigned int _size);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InitIndexBuffer(): Creates a new index buffer based on the passed in size, protects the DRX COM object.
	//                       
	// Returns: IDirect3DIndexBuffer9* - The newly created index buffer.
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static IDirect3DIndexBuffer9* InitIndexBuffer( unsigned int _size);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddToObjectRenderSet(): Adds the passed in render node to the object render set, also adding the matrix to that
	//                       render nodoes vector. This render set is cleared every frame.
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void AddToObjectRenderSet( RenderNode* _node, D3DXMATRIX _mat );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddToEnvironmentRenderSet(): Adds the passed in render node to the environment render set.
	//                       This render set is never cleared unless changing levels. 
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void AddToEnvironmentRenderSet( RenderNode* _node );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawCube(): Using a flat shader this function can draw the render node flat shaded, used for testing purposes.
	//             Not deprecated can still be called as the shader is still loaded in INIT.           
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawCube( RenderNode& pNode );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawCube(): Using a flat shader this function can draw the render node flat shaded, used for testing purposes.
	//             Not deprecated can still be called as the shader is still loaded in INIT.           
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawDepth( RenderNode& pNode );
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawCube(): Using a flat shader this function can draw the render node flat shaded, used for testing purposes.
	//             Not deprecated can still be called as the shader is still loaded in INIT.           
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawGBuffers( RenderNode& pNode );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawTexturedMesh(): Uses the rendernode to draw a textured mesh, used for every object within the current game scene.
	//                     The shader can be changed but please ask me first!
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawTexturedMesh( RenderNode& pNode );
	static void DrawScrollingTexturedMesh( RenderNode& pNode );
	static void DrawTexturedEnvironmentMesh( RenderNode& pNode );
	static void DrawScrollingTexturedEnvironmentMesh( RenderNode& pNode );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawLine(): Draws a line to the screen.
	//             Currently does not work on ATI cards, change aim sight to cylinder.
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawLine( D3DXVECTOR3 _start, D3DXVECTOR3 _end, D3DXCOLOR color );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawSmoothSkinnedMesh(): Draws an animated mesh to the screen.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawSmoothSkinnedMesh( RenderNode& pNode );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DrawTexturedPlane(): Draws the texture passed in using the transform matrix. 
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DrawTexturedPlane( D3DXMATRIX _transform, LPDIRECT3DTEXTURE9 _texture );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddGlowToDraw(): Adds a glow effect to the vector to be rendered. 
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void AddGlowToDraw( D3DXMATRIX _transform, int _texture );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GetPosNormTanUVDecl(): Get the Position, Normal, Tangent, UV vertex decleration. 
	//                       
	// Returns: IDirect3DVertexDeclaration9* to the vertex decleration.
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static inline IDirect3DVertexDeclaration9* GetPosNormTanUVDecl(void) { return m_pPosNormTanUVdecl; }
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GetPosNormTanUVInfDecl(): Get the Position, Normal, Tangent, UV, Influences vertex decleration.
	//                       
	// Returns: IDirect3DVertexDeclaration9* to the vertex deleration.
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static inline IDirect3DVertexDeclaration9* GetPosNormTanUVInfDecl(void) { return m_pPosNormTanUVInfdecl; }
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RasterTexture(): Draws a texture in screen space based on the scales passed in, used for menus. 
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void RasterTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom );
	static void RasterTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom, float _fadeFactor );
	static void RasterSourceRectTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom, float _minX, float _maxX, float _minY, float _maxY, float _fadeFactor = 1.0f );



	static void RenderBurningTexture( RenderNode& pNode );

	////////////////////////////////////////////////////////////////////////////////////
	//	SetGamma():		Used to set the new gamma if it changes.
	//
	//	Returns:		void
	//
	//	Mod Date:		6/11/12
	//	Mod Initials:	JM
	////////////////////////////////////////////////////////////////////////////////////
	static void SetGamma(float gammaFactor);

	////////////////////////////////////////////////////////////////////////////////////
	//	GetGamma():		Used to get the current gamma factor
	//
	//	Returns:		float - the current factor at which the gamma is calculated
	//
	//	Mod Date:		6/11/12
	//	Mod Initials:	JM
	////////////////////////////////////////////////////////////////////////////////////
	static float GetGamma(void) { return m_fGammaFactor; }

	static void ClearData(void);
	static void ResetBurn(void);
	
	static void ResetDevice(void);
};











#endif //RENDERENGINE_H_