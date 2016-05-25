#include "CBitmapFont.h"
#include "..//RenderEngine/TextureManager.h"

#define CHAR_SPACEX 0.2f
#define CHAR_SPACEY 0.085f
#define BITMAP_SPACE 0.02f
#define PRINTRECT_LEFT 0.5f

CBitmapFont::CBitmapFont(void)
{

	m_BitmapFontTextureHandle = 0;
}

CBitmapFont::~CBitmapFont(void)
{

}

void CBitmapFont::Initialize(  /*LPDIRECT3DTEXTURE9* pID, */float fCharWidth, float fCharHeight, float fNumCols, float fTexWidth, float fTexHeight, char cStartChar )
{
	//m_pBitmapFontTextureHandle = pID;
	m_fCharWidth	= fCharWidth;
	m_fCharHeight	= fCharHeight;
	m_fNumCols		= fNumCols;
	m_fTextureWidth = fTexWidth;
	m_fTextureHeight = fTexHeight;
	m_cStartChar	= cStartChar;

	m_BitmapFontTextureHandle = /*TextureManager::GetInstance()->GetTexture(*/ TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/PS_HUD_Font.png" );// );
}

void CBitmapFont::GetCharRect(int nID)
{
	// [0] - LEFT
	// [1] - TOP
	// [2] - RIGHT
	// [3] - BOTTOM
	m_fTextureRect[0] 	= ((nID % (int)m_fNumCols) * m_fCharWidth) / m_fTextureWidth;
	m_fTextureRect[1]	= ((nID / (int)m_fNumCols) * m_fCharHeight) / m_fTextureHeight;
	m_fTextureRect[2]	= (float)(m_fTextureRect[0] + m_fCharWidth / m_fTextureWidth) ;
	m_fTextureRect[3] 	= (float)(m_fTextureRect[1] + m_fCharHeight / m_fTextureHeight);
}

void CBitmapFont::Print( const char* szTextToPrint, float nPosX, float nPosY, float	fScale, DWORD dwColor)
{
	// cache start position
	float nOrigin = nPosX;

	// cache length of szTextToPrint
	int len = strlen(szTextToPrint);

	// loop through szTextToPrint
	for(int i=0; i < len; ++i)
	{
		char ch = szTextToPrint[i];

		if( ch == ' ')
		{
			nPosX += BITMAP_SPACE * fScale;
			continue; 
		}

		else if(ch == '\n')
		{
			nPosX = nOrigin;
			nPosY += CHAR_SPACEY * fScale;
			continue;
		}

		int id = int(ch - m_cStartChar);

		GetCharRect(id);

		// Draw it
		//CSGD_TextureManager::GetInstance()->Draw(m_nImageID, nPosX, nPosY, fScale, fScale, &rLetter, 0, 0, 0, dwColor);
		// [0] - LEFT
		// [1] - TOP
		// [2] - RIGHT
		// [3] - BOTTOM
		RenderEngine::RasterSourceRectTexture( TextureManager::GetInstance()->GetTexture(m_BitmapFontTextureHandle), nPosX, nPosY, nPosX + CHAR_SPACEX, nPosY + CHAR_SPACEY, 
												m_fTextureRect[0], m_fTextureRect[2], m_fTextureRect[1], m_fTextureRect[3] );

		nPosX += BITMAP_SPACE * fScale;
	}
}