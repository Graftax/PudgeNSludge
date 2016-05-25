#ifndef POSTPROCESSCHAIN_H_
#define POSTPROCESSCHAIN_H_

class RenderTarget;
class PostProcess;

#include "RenderEngine.h"
#include <list>

__declspec(align(32))
class PostProcessChain
{
private:
	std::list< PostProcess* > m_vSwapChain;

	RenderTarget m_RenderTarget1;
	RenderTarget m_RenderTarget2;

	RenderTarget* m_pSource;
	RenderTarget* m_pDestination;

public:
	PostProcessChain(void);

	void Initialize(void);

	void Clear(void);

	void Run(void);

	inline RenderTarget* GetActiveRenderTarget(void) { return m_pSource; }

	inline void AddPostProcess( PostProcess* _process )
	{
		m_vSwapChain.push_back( _process );
	}

	inline void ClearChain(void) { m_vSwapChain.clear(); }
};








#endif //POSTPROCESSCHAIN_H_