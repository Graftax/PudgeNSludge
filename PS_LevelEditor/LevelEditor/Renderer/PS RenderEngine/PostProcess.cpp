#include "PostProcess.h"
#include "RenderHandle.h"
#include "MeshManager.h"
#include "RenderTarget.h"

PostProcess::~PostProcess(void)
{
	SAFE_RELEASE(m_pEffect);
}

void PostProcess::Initialize( RenderHandle* _mesh, LPCWSTR _effectFile )
{
	m_hMesh = *_mesh;
	ID3DXBuffer	*errors(nullptr);

#if _DEBUG
	HRESULT HR = D3DXCreateEffectFromFileW( RenderEngine::m_pD3Dev, _effectFile, 0, 0, D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, RenderEngine::m_pEffectPool, &m_pEffect, &errors);
#else
	D3DXCreateEffectFromFileW( RenderEngine::m_pD3Dev, _effectFile, 0, 0, 0, RenderEngine::m_pEffectPool, &m_pEffect, &errors);
#endif
	if( errors )
	{
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0 );	
		SAFE_RELEASE( errors );
	}
	else
	{
 		m_hTexture = m_pEffect->GetParameterByName( 0, "sourceTexture" );
		m_pEffect->SetTechnique( "PostProcess" );
	}
}

void PostProcess::Process( RenderTarget &_destination, LPDIRECT3DBASETEXTURE9 _sourceTex )
{
	_destination.ActivateTarget(0);

	LPDIRECT3DSURFACE9 pOldRT;
	D3DSURFACE_DESC desc;
	RenderEngine::m_pD3Dev->GetRenderTarget(0, &pOldRT );
	pOldRT->GetDesc(&desc);

	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );
	RenderEngine::m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), RenderEngine::postprocessMesh.m_uiBufferOffset, RenderEngine::postprocessMesh.m_uiStride );
	RenderEngine::m_pD3Dev->SetVertexDeclaration( RenderEngine::GetPosNormTanUVDecl() );
	RenderEngine::m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
	m_pEffect->SetMatrix( "gWVP", &(_matProj) );
	m_pEffect->SetTexture( m_hTexture, _sourceTex );
	m_pEffect->SetVector("gHalfPixel", 
			&D3DXVECTOR4(0.5f / desc.Width, 0.5f / desc.Height, 0, 1));
	m_pEffect->CommitChanges();
	UINT passes = 0;
	m_pEffect->Begin( &passes, 0 );
	for( UINT i = 0u; i < passes; ++i )
	{
		m_pEffect->BeginPass(i);

		RenderEngine::m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, RenderEngine::postprocessMesh.m_uiVertices, RenderEngine::postprocessMesh.m_uiStartIndex, RenderEngine::postprocessMesh.m_uiPrimCount );

		m_pEffect->EndPass();
	}
	m_pEffect->End();
	_destination.RevertTarget();

	pOldRT->Release();
}

