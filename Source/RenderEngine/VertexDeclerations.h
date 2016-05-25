//////////////////////////////////////////////////////////////////////////////////////
// Filename:			VertexDeclerations.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             The structures for vertex declerations being used
//////////////////////////////////////////////////////////////////////////////////////
#ifndef VERTEXDECLS_H_
#define VERTEXDECLS_H_

#define RE_UVS_NX 1
#define RE_UVS_NY 2
#define RE_UVS_NB 4
#define RE_UVS_PX 8
#define RE_UVS_PY 16
#define RE_UVS_PB 32
#define RE_UVS_NONE 64
#define RE_ANIMATED 128
#define RE_TRANSPARENT 256
#define RE_BLEND 512
#define RE_BURNTEX 1024
#define RE_FORWARD 2048
#define RE_BURNOWNER 4096
#define RE_PLAYER 8192

#define DIFFUSE_TEX 0 
#define NORMAL_TEX  1
#define SPECULAR_TEX 2
#define EMISSIVE_TEX 3
#define BURNDOWN_TEX 4


struct VERTEX_POSNORMTANUV
{
	D3DXVECTOR3					m_Position;
	D3DXVECTOR3					m_Normal;
	D3DXVECTOR3					m_Tangents;
	D3DXVECTOR2					m_TextureCoord;
};

struct VERTEX_POSNORMTANUVINF
{
	D3DXVECTOR3					m_Position;
	D3DXVECTOR3					m_Normal;
	D3DXVECTOR3					m_Tangents;
	D3DXVECTOR2					m_TextureCoord;
	D3DXVECTOR4					m_JointWeights;
	D3DXVECTOR4					m_JointIndices;
};

//////////////////////////////////
//Structure used for drawing a line to the screen.
struct VERTEX_POSCOLOR
{
	D3DXVECTOR3		m_position;
	D3DCOLOR		m_color;
};
//Structure used for rendering object glows.
struct Glow_Info
{
	D3DXMATRIX _transform;
	int _nTextHandle;
};
//////////////////////////////////





#endif //VERTEXDECLS_H_