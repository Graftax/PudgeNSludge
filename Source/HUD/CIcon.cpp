/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CIcon
//
// Purpose: Manage the trajectory line created when the player
//			has an object
//
// Original Author: Rueben Massey
//
// Creation Date: 8/15/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////

// includes
#include "CIcon.h"
#include "..//RenderEngine/TextureManager.h"

CIcon* CIcon::m_pInstance = NULL;

CIcon::CIcon()
{
	m_bIsOn = true;
	m_usRefCount = 0;
	m_fSwapTimer = 0;
}

CIcon::~CIcon()
{

}

CIcon* CIcon::GetInstance()
{
	if( !m_pInstance )
	{
		m_pInstance = new CIcon();
	}
	m_pInstance->m_usRefCount += 1;
	return m_pInstance;
}

void CIcon::DeleteInstance()
{
	--m_usRefCount;

	if( m_pInstance && 0 == m_usRefCount )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: Update the swap counter/ be awesome and junk
// Original Author: Heidi Hernandez
// Creation Date: 7/9/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcon::Update( float fElapsedTime )
{
	m_fSwapTimer += fElapsedTime;
	if( m_fSwapTimer > 2.0f )
	{
		m_fSwapTimer = 0.0f;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render()
// Purpose: Render the Icon line
// Original Author: Heidi Hernandez
// Creation Date: 8/15/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcon::Render( int nTexture1, int nTexture2, vec3f posVec, D3DXMATRIX scalemat )
{
	if( !m_bIsOn )
		return;

	D3DXMATRIX posmat;
	D3DXMatrixIdentity( &posmat );
	posmat._41 = posVec.x;// - 50.0f;
	posmat._42 = posVec.y;// + 100.0f;
	posmat._43 = posVec.z;

	//D3DXMatrixScaling( &scalemat, 100.0f, 100.0f, 0.0f );
	D3DXMatrixMultiply( &posmat, &scalemat, &posmat );

	if( m_fSwapTimer < 1.0f )
	{
		RenderEngine::AddGlowToDraw( posmat, nTexture1 );
	}
	else
	{
		RenderEngine::AddGlowToDraw( posmat, nTexture2 );
	}
}