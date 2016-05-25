#ifndef POSTPROCESS_H_
#define POSTPROCESS_H_

#include "RenderEngine.h"
class RenderTarget;

__declspec(align(32))
class PostProcess
{
private:

	ID3DXEffect* m_pEffect;
	D3DXHANDLE   m_hTexture;
	//D3DXHANDLE   m_hWorld;
	//D3DXMATRIX   m_World;

	RenderHandle m_hMesh;

public:
	PostProcess(void) : m_pEffect(nullptr) {}
	~PostProcess(void);

	void Initialize( RenderHandle* _mesh, LPCWSTR _effectFile );
	void Process( RenderTarget &_destination, LPDIRECT3DBASETEXTURE9 _sourceTex );
	inline ID3DXEffect* GetEffect(void) { return m_pEffect; }
};

#endif //POSTPROCESS_H_

