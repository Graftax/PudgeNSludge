#include "RenderTarget.h"

RenderTarget::RenderTarget(void)
{
	m_dwTargetIndex = -1;
	m_pOldRenderSurface = nullptr;
	m_pRenderSurface = nullptr;
}

RenderTarget::~RenderTarget(void)
{
	if( m_pRenderSurface )
	{
		m_pRenderSurface->Release();
		m_pRenderSurface = nullptr;
	}

	if( m_pOldRenderSurface )
	{
		m_pOldRenderSurface->Release();
		m_pOldRenderSurface = nullptr;
	}

	if( m_pTargetTexture )
	{
		m_pTargetTexture->Release();
		m_pTargetTexture = nullptr;
	}
}

void RenderTarget::Create( UINT _width, UINT _height, D3DFORMAT _format )
{
	D3DXCreateTexture( RenderEngine::m_pD3Dev, _width, _height, 1, D3DUSAGE_RENDERTARGET, 
		_format, D3DPOOL_DEFAULT, &m_pTargetTexture );
	m_pTargetTexture->GetSurfaceLevel( 0, &m_pRenderSurface );
}

void RenderTarget::ActivateTarget( DWORD _targetIndex )
{
	m_dwTargetIndex = _targetIndex;
	RenderEngine::m_pD3Dev->GetRenderTarget( _targetIndex, &m_pOldRenderSurface );
	RenderEngine::m_pD3Dev->SetRenderTarget( m_dwTargetIndex, m_pRenderSurface );
}

void RenderTarget::RevertTarget(void)
{
	RenderEngine::m_pD3Dev->SetRenderTarget( m_dwTargetIndex, m_pOldRenderSurface );
	if( m_pOldRenderSurface )
	{
		m_pOldRenderSurface->Release();
		m_pOldRenderSurface = nullptr;
	}
	m_dwTargetIndex = -1;
}

void RenderTarget::DeleteTarget(void)
{
	if( m_pRenderSurface )
	{
		m_pRenderSurface->Release();
		m_pRenderSurface = nullptr;
	}

	if( m_pOldRenderSurface )
	{
		m_pOldRenderSurface->Release();
		m_pOldRenderSurface = nullptr;
	}

	if( m_pTargetTexture )
	{
		m_pTargetTexture->Release();
		m_pTargetTexture = nullptr;
	}
}