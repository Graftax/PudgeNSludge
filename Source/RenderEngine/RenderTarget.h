#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_

#include "RenderEngine.h"

class RenderTarget
{
private:

	LPDIRECT3DTEXTURE9 m_pTargetTexture;
	LPDIRECT3DSURFACE9 m_pRenderSurface;
	LPDIRECT3DSURFACE9 m_pOldRenderSurface;
	DWORD m_dwTargetIndex;

public:
	RenderTarget(void);
	~RenderTarget(void);

	void Create( UINT _width, UINT _height, D3DFORMAT _format = D3DFMT_A2R10G10B10 );

	void ActivateTarget( DWORD _targetIndex );

	void RevertTarget(void);

	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTargetTexture; }

	void DeleteTarget(void);

};









#endif //RENDERTARGET_H_