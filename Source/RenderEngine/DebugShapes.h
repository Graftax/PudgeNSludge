//////////////////////////////////////////////////////////////////////////////////////
// Filename:            MeshManager.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all loaded meshes
//////////////////////////////////////////////////////////////////////////////////////
#ifndef DEBUGSHAPES_H_
#define DEBUGSHAPES_H_


#include "RenderEngine.h"


struct DEBUGVERTEX
{
	D3DXVECTOR3		position;
	D3DXVECTOR3		normal;
};

__declspec(align(32))
class DebugShapes
{
private:
	static ID3DXMesh* m_pSphereMesh;
	static ID3DXMesh* m_pBoxMesh;
	static ID3DXEffect* m_pDebugShapes;
	static IDirect3DVertexDeclaration9* m_pPosNormDecl;

	static IDirect3DVertexBuffer9* m_CurrentBuffer;
	static IDirect3DIndexBuffer9* m_CurrentIndex;

	//HANDLES
	static D3DXHANDLE m_TechniqueHandle;			// Current technique
	static D3DXHANDLE m_WorldViewProjectionHandle;	// Camera matrix
	static D3DXHANDLE m_WorldHandle;				// Current object world matrix
	static D3DXHANDLE m_ColorHandle;				// Current object color

public:
	DebugShapes(void){}
	~DebugShapes(void);

	static void InitShapes(void);
	static void CreateSphere( float _radius, UINT _slices, UINT _stacks );


	static void CreateBox( float _width, float _height, float _depth );
	static void RenderSphere( D3DXMATRIX _mat ) { RenderEngine::m_vSphereMats.push_back(_mat); }
	static void RenderBox( D3DXMATRIX _mat ) { RenderEngine::m_vBoxMats.push_back(_mat); }

	static void RenderSpheres( std::vector<D3DXMATRIX> _mats );	
	static void RenderBoxs( std::vector<D3DXMATRIX> _mats );	
};
#endif //DEBUGSHAPES_H_