//////////////////////////////////////////////////////////////////////////////////////
// Filename:            RenderHandle.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             Structures used by the render engine to render things
//////////////////////////////////////////////////////////////////////////////////////
#ifndef RENDERHANDLE_H_
#define RENDERHANDLE_H_

#define MAX_TEXTURES 5

#include "RenderEngine.h"
#include <vector>

#include <d3dx9.h>
#include <d3d9.h>

enum SCROLLING_UV_STYLE { SCROLL_X = 1, SCROLL_Y, SCROLL_BOTH, SCROLL_NONE = 0 };

//class CCollisionAABB;
__declspec(align(16))
struct RenderHandle //SmartHandle
{
	std::vector<D3DXMATRIX>			m_vTransform;
	UINT							m_uiBufferOffset;
	UINT							m_uiStride;
	UINT							m_uiVertices;
	UINT							m_uiStartIndex;
	UINT							m_uiPrimCount;
	INT								m_uiTextureHandles[MAX_TEXTURES];
	IDirect3DVertexDeclaration9*	m_d3dVertDecl;

	RenderHandle(void)
	{
		m_vTransform.clear();
		m_uiBufferOffset = 0u;
		m_uiStride = 0u;
		m_uiVertices = 0u;
		m_uiStartIndex = 0u;
		m_uiPrimCount = 0u;
		for( int i = 0; i < MAX_TEXTURES; ++i )
			m_uiTextureHandles[MAX_TEXTURES] = 0;
		m_d3dVertDecl = nullptr;
	}
};

#endif //RENDERHANDLE_H_