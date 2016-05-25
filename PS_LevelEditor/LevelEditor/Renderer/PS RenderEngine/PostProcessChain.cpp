#include "RenderTarget.h"
#include "PostProcess.h"
#include "PostProcessChain.h"

using namespace std;

PostProcessChain::PostProcessChain(void)
{
	m_pSource = m_pDestination = nullptr;
}

void PostProcessChain::Initialize(void)
{
	m_RenderTarget1.Create( RenderEngine::SCREEN_WIDTH, RenderEngine::SCREEN_HEIGHT );
	m_RenderTarget2.Create( RenderEngine::SCREEN_WIDTH, RenderEngine::SCREEN_HEIGHT );

	m_pSource = &m_RenderTarget1;
	m_pDestination = &m_RenderTarget2;
}

void PostProcessChain::Run(void)
{
	std::list< PostProcess* >::iterator iter = m_vSwapChain.begin();

	for( ; iter != m_vSwapChain.end(); ++iter )
	{
		(*iter)->Process( *m_pDestination, m_pSource->GetTexture() );
		swap( m_pDestination, m_pSource );
	}
}

void PostProcessChain::Clear(void)
{
	m_RenderTarget1.DeleteTarget();
	m_RenderTarget2.DeleteTarget();
}