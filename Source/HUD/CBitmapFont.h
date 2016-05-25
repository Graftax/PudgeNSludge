#ifndef CBITMAPFONT_H_
#define CBITMAPFONT_H_

#include "..//RenderEngine/RenderEngine.h"

__declspec(align(32))
class CBitmapFont
{
private:
	float		m_fCharWidth;
	float		m_fCharHeight;
	float		m_fNumCols;
	float		m_fTextureWidth;
	float		m_fTextureHeight;
	float		m_fTextureRect[4];

	int m_BitmapFontTextureHandle;

	// First Character in Bitmap for reference
	char	m_cStartChar;

	LPDIRECT3DTEXTURE9* m_pBitmapFontTextureHandle;
	void	GetCharRect(int nID);
public:
	CBitmapFont(void);
	~CBitmapFont(void);

	void Initialize( float fCharWidth, float fCharHeight, float fNumCols, float fTexWidth, float fTexHeight, char cStartChar );
	void	Print(	const char*	szTextToPrint, float	nPosX, float	nPosY,
		float	fScale=1.0f, DWORD	dwColor= 0xffffffff);
};
#endif