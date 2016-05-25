//////////////////////////////////////////////////////////////////////////////////////
// Filename:            MeshManager.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all loaded meshes
//////////////////////////////////////////////////////////////////////////////////////
#include "DebugShapes.h"



ID3DXMesh*						DebugShapes::m_pSphereMesh = NULL;
ID3DXMesh*						DebugShapes::m_pBoxMesh = NULL;
ID3DXEffect*					DebugShapes::m_pDebugShapes = NULL;
IDirect3DVertexDeclaration9*	DebugShapes::m_pPosNormDecl = NULL;

IDirect3DVertexBuffer9*			DebugShapes::m_CurrentBuffer = NULL;
IDirect3DIndexBuffer9*			DebugShapes::m_CurrentIndex = NULL;

//HANDLES
D3DXHANDLE						DebugShapes::m_TechniqueHandle = NULL;			// Current technique
D3DXHANDLE						DebugShapes::m_WorldViewProjectionHandle = NULL;	// Camera matrix
D3DXHANDLE						DebugShapes::m_WorldHandle = NULL;				// Current object world matrix
D3DXHANDLE						DebugShapes::m_ColorHandle = NULL;				// Current object color

void DebugShapes::InitShapes(void)
{
	m_pSphereMesh = nullptr;
	m_pBoxMesh = nullptr;
	m_pDebugShapes = nullptr;
	m_pPosNormDecl = nullptr;

	ID3DXBuffer* error = NULL;
	if(FAILED(D3DXCreateEffectFromFileW( RenderEngine::m_pD3Dev, L"./Resources/Shaders/StaticObjects.fx", NULL, NULL, 0, NULL, &m_pDebugShapes, &error )))
			OutputDebugStringA((LPCSTR)error->GetBufferPointer());
	if(error)
		error->Release();

	m_WorldViewProjectionHandle = m_pDebugShapes->GetParameterByName(NULL, "g_mWorldViewProjection");
	m_TechniqueHandle			= m_pDebugShapes->GetTechniqueByName("DebugShape");
	m_WorldHandle				= m_pDebugShapes->GetParameterByName(NULL, "g_mWorld");
	m_ColorHandle				= m_pDebugShapes->GetParameterByName(NULL, "g_mColor");

	D3DVERTEXELEMENT9 _posNorm[] = {
	{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	D3DDECL_END() };

	RenderEngine::m_pD3Dev->CreateVertexDeclaration( _posNorm, &m_pPosNormDecl );
}

DebugShapes::~DebugShapes(void)
{
	SAFE_RELEASE(m_pSphereMesh);
	SAFE_RELEASE(m_pBoxMesh);
	SAFE_RELEASE(m_pPosNormDecl);
	SAFE_RELEASE(m_pDebugShapes);
}

void DebugShapes::CreateSphere( float _radius, UINT _slices, UINT _stacks )
{
	SAFE_RELEASE(m_pSphereMesh);
	D3DXCreateSphere( RenderEngine::m_pD3Dev, _radius, _slices, _stacks, &m_pSphereMesh, NULL );
}

void DebugShapes::RenderSpheres( std::vector<D3DXMATRIX> _mats )
{
	if( m_pSphereMesh == nullptr )
		CreateSphere( 20.0f, 8, 16 );
	if(!m_pDebugShapes) 
		InitShapes();
	RenderEngine::m_pD3Dev->SetVertexDeclaration(m_pPosNormDecl);
	RenderEngine::m_pD3Dev->SetFVF( 0 );
	for( unsigned int i = 0u; i < _mats.size(); ++i )
	{
		m_pDebugShapes->SetMatrix( m_WorldViewProjectionHandle, &RenderEngine::m_mMVP );
		m_pDebugShapes->Begin(0,0);
		m_pDebugShapes->BeginPass(0);
		{
			
			m_pSphereMesh->GetVertexBuffer( &m_CurrentBuffer );
			m_pSphereMesh->GetIndexBuffer( &m_CurrentIndex );
			RenderEngine::m_pD3Dev->SetStreamSource( 0, m_CurrentBuffer, 0, sizeof(DEBUGVERTEX));
			RenderEngine::m_pD3Dev->SetIndices( m_CurrentIndex );
			m_pDebugShapes->SetTechnique( "DebugShape" );
			m_pDebugShapes->SetMatrix( m_WorldHandle, &_mats[i] );
			m_pDebugShapes->SetVector( m_ColorHandle, &D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f) );
			m_pDebugShapes->CommitChanges();
		
			RenderEngine::m_pD3Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_pSphereMesh->GetNumVertices(),0,m_pSphereMesh->GetNumFaces());
		}
		m_pDebugShapes->EndPass();
		m_pDebugShapes->End();
	}
}
	
void DebugShapes::CreateBox( float _width, float _height, float _depth )
{
	SAFE_RELEASE( m_pBoxMesh );
	D3DXCreateBox( RenderEngine::m_pD3Dev, _width, _height, _depth, &m_pBoxMesh, NULL );
}
	
void DebugShapes::RenderBoxs( std::vector<D3DXMATRIX> _mats )
{
	if( m_pBoxMesh == nullptr )
		CreateBox( 1.0f, 1.0f, 1.0f );
	if(!m_pDebugShapes) 
		InitShapes();		
	RenderEngine::m_pD3Dev->SetVertexDeclaration(m_pPosNormDecl);
	RenderEngine::m_pD3Dev->SetFVF( 0 );

	for( unsigned int i = 0u; i < _mats.size(); ++i )
	{
		m_pDebugShapes->SetMatrix( m_WorldViewProjectionHandle, &RenderEngine::m_mMVP );
		m_pDebugShapes->Begin(0,0);
		m_pDebugShapes->BeginPass(0);
		{
			m_pBoxMesh->GetVertexBuffer( &m_CurrentBuffer );
			m_pBoxMesh->GetIndexBuffer( &m_CurrentIndex );
			RenderEngine::m_pD3Dev->SetStreamSource( 0, m_CurrentBuffer, 0, sizeof(DEBUGVERTEX));
			RenderEngine::m_pD3Dev->SetIndices( m_CurrentIndex );
			m_pDebugShapes->SetTechnique( "DebugShape" );
			m_pDebugShapes->SetMatrix( m_WorldHandle, &_mats[i] );
			m_pDebugShapes->SetVector( m_ColorHandle, &D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f) );
			m_pDebugShapes->CommitChanges();
		
			RenderEngine::m_pD3Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_pBoxMesh->GetNumVertices(),0,m_pBoxMesh->GetNumFaces());
		}
		m_pDebugShapes->EndPass();
		m_pDebugShapes->End();
	}
}