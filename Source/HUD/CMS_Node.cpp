#include "CMS_Node.h"
#include "../RenderEngine/TextureManager.h"
#include "CMS_Page.h"

CMS_Node::CMS_Node()
{
	m_bCurrCursorTarget = false;
	m_bPrevCursorTarget = false;
	m_bCursorBuffer = false;
	m_bTargetable = false;
	m_bChecked = false;
	m_pMaster = nullptr;
	m_bActive = true;

	m_amAlignment = CMSAM_LEFT;
	m_nCurrentImage = -1;

	m_fTop = 0.0f;
	m_fLeft = 0.0f;
	m_fRight = 0.0f;
	m_fBottom = 0.0f;

	m_fHorizontalOffset = 0.0f;

	m_fWidth = -1.0f;
	m_fHeight = -1.0f;
	m_fBottomExtents = 0.0f;

	m_fUsedHeight = 0.0f;
	m_fUsedWidth = 0.0f;
}

void CMS_Node::CalcExtents()
{
	switch( m_enType )
	{
	case CMSNT_BODY:
		{
			m_fLeft = 0.0f;
			m_fTop = 0.0f;
			m_fBottom = (float)(CGame::GetInstance()->GetScreenHeight());
			m_fRight = (float)(CGame::GetInstance()->GetScreenWidth());

			std::list< CMS_Node* >::iterator itrNode = m_nodeChildren.begin();
			while( itrNode != m_nodeChildren.end() )
			{
				(*itrNode)->CalcExtents();
				itrNode++;
			}

			break;
		}
	case CMSNT_DIV:
		{
			m_fLeft = m_pParent->m_fLeft + m_fHorizontalOffset;
			if( m_fWidth > 0.0f )
			{
				m_fRight = m_fLeft + m_fWidth;
			}
			else
			{
				m_fRight = m_pParent->m_fRight;
			}

			m_pParent->m_fUsedHeight = m_pParent->m_fBottomExtents - m_pParent->m_fTop;
			m_fTop = m_pParent->m_fTop + m_pParent->m_fUsedHeight;
			m_fBottomExtents = m_fTop;
			if( m_fHeight > 0.0f )
			{
				m_fBottom = m_fTop + m_fHeight;
			}
			else
			{
				m_fBottom = m_fTop + GetLargestChildHeight();
			}

			if( m_fBottom > m_pParent->m_fBottomExtents )
			{
				m_pParent->m_fBottomExtents = m_fBottom;
			}

			float fOldBotExtents = m_fBottomExtents;

			std::list< CMS_Node* >::iterator itrNode = m_nodeChildren.begin();
			while( itrNode != m_nodeChildren.end() )
			{
				(*itrNode)->CalcExtents();
				itrNode++;
			}

			m_pParent->m_fUsedHeight += m_fBottomExtents - fOldBotExtents;
			m_pParent->m_fUsedWidth = 0.0f;

			if( m_fWidth > 0.0f )
				m_fRight = m_fLeft + m_fWidth;
			else
				m_fRight = m_fLeft + m_fUsedWidth;
			if( m_fHeight > 0.0f )
				m_fBottom = m_fTop + m_fHeight;
			else
				m_fBottom = m_fBottomExtents;

			break;
		}
	case CMSNT_IMG:
		{
			m_fTop = m_pParent->GetInsideBottom();

			switch( m_pParent->m_amAlignment )
			{
			case CMSAM_LEFT:
				{
					m_fLeft = m_pParent->GetInsideRight() + 1.0f;
					if( m_fWidth > 0.0f )
					{
						m_fRight = m_fLeft + m_fWidth;

						if( m_fRight > m_pParent->m_fRight - 1.0f )
						{
							m_pParent->m_fUsedWidth = 0.0f;
							m_fLeft = m_pParent->GetInsideRight() + 1.0f;
							m_pParent->m_fUsedHeight = m_pParent->m_fBottomExtents - m_pParent->m_fTop;
							m_fTop = m_pParent->GetInsideBottom() + 1.0f;
						}

						m_fRight = m_fLeft + m_fWidth;
						m_pParent->m_fUsedWidth += m_fRight - m_fLeft + 2.0f;
					}
					break;
				}
			case CMSAM_RIGHT:
				{
					m_fRight = m_pParent->GetInsideLeft() - 1.0f;
					if( m_fWidth > 0.0f )
					{
						m_fLeft = m_fRight - m_fWidth;

						if( m_fLeft < m_pParent->m_fLeft + 1.0f )
						{
							m_pParent->m_fUsedWidth = 0.0f;
							m_fRight = m_pParent->GetInsideLeft() - 1.0f;
							m_pParent->m_fUsedHeight = m_pParent->m_fBottomExtents - m_pParent->m_fTop;
							m_fTop = m_pParent->GetInsideBottom() + 1.0f;
						}

						m_fLeft = m_fRight - m_fWidth;
						m_pParent->m_fUsedWidth += m_fRight - m_fLeft + 2.0f;
					}
					break;
				}
			}
			if( m_fHeight > 0.0f )
			{
				m_fBottom = m_fTop + m_fHeight;
			}

			if( m_fBottom > m_pParent->m_fBottomExtents )
			{
				//m_pParent->UpdateExtents( m_fBottom - m_pParent->m_fBottomExtents );
				m_pParent->m_fBottomExtents = m_fBottom;
			}
			break;
		}
	}
}

void CMS_Node::UpdateExtents( float _update )
{
	switch( m_enType )
	{
	case CMSNT_BODY:
		{
			m_fUsedHeight += _update;
			break;
		}
	case CMSNT_DIV:
		{
			m_fUsedHeight += _update;
			m_pParent->UpdateExtents( _update );
			break;
		}
	}
}

float CMS_Node::GetLargestChildHeight()
{
	float toReturn = m_fHeight;

	std::list< CMS_Node* >::iterator itrNode = m_nodeChildren.begin();
	while( itrNode != m_nodeChildren.end() )
	{
		float temp = (*itrNode)->GetLargestChildHeight();
		if( toReturn < temp )
			toReturn = temp;

		itrNode++;
	}

	return toReturn;
}

void CMS_Node::Update()
{
	m_bPrevCursorTarget = m_bCurrCursorTarget;
	m_bCurrCursorTarget = m_bCursorBuffer;;
	m_bCursorBuffer = false;

	if( m_bPrevCursorTarget && !m_bCurrCursorTarget )
		OnCursorLeave();
	if( !m_bPrevCursorTarget && m_bCurrCursorTarget )
		OnCursorHit();



	std::list< CMS_Node* >::iterator itrNodes = m_nodeChildren.begin();

	while( itrNodes != m_nodeChildren.end() )
	{
		(*itrNodes)->Update();
		itrNodes++;
	}
}

void  CMS_Node::Render()
{
	//std::map< std::string, int >::iterator itrImages = m_mapImages.begin();

	//if( itrImages != m_mapImages.end() )
	{
		//RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture( itrImages->second ), m_fLeft/CGame::GetInstance()->GetScreenWidth(), 1.0f - (m_fBottom/CGame::GetInstance()->GetScreenHeight()), m_fRight/CGame::GetInstance()->GetScreenWidth(), 1.0f - (m_fTop/CGame::GetInstance()->GetScreenHeight()) );
	}

	if( !m_bActive )
		return;

	if( m_nCurrentImage > -1 )
		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture( m_nCurrentImage ), (m_fLeft + m_pMaster->m_fOffsetX)/CGame::GetInstance()->GetScreenWidth(), 1.0f - ((m_fBottom + m_pMaster->m_fOffsetY)/CGame::GetInstance()->GetScreenHeight()), (m_fRight + m_pMaster->m_fOffsetX)/CGame::GetInstance()->GetScreenWidth(), 1.0f - ((m_fTop + m_pMaster->m_fOffsetY)/CGame::GetInstance()->GetScreenHeight()) );

	if( strcmp( m_szActionString.c_str(), "button" ) == 0 )
	{
		if( m_bChecked )
			RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture( m_nCheckImage ), (m_fLeft + m_pMaster->m_fOffsetX)/CGame::GetInstance()->GetScreenWidth(), 1.0f - ((m_fBottom + m_pMaster->m_fOffsetY)/CGame::GetInstance()->GetScreenHeight()), (m_fRight + m_pMaster->m_fOffsetX)/CGame::GetInstance()->GetScreenWidth(), 1.0f - ((m_fTop + m_pMaster->m_fOffsetY)/CGame::GetInstance()->GetScreenHeight()) );
	}
}

void CMS_Node::AddImage( const char* _address )
{
	if( m_mapImages.size() < 1 )
	{
		m_szOrgImage = _address;
		m_nCurrentImage = m_mapImages[ std::string( _address ) ] = TextureManager::GetInstance()->LoadTexture( _address );
	}
	else
	{
		m_szAltImage = _address;
		m_mapImages[ std::string( _address ) ] = TextureManager::GetInstance()->LoadTexture( _address ); 
	}
}

float CMS_Node::GetInsideRight()
{
	return m_fLeft + m_fUsedWidth;
}

float CMS_Node::GetInsideBottom()
{
	return m_fTop + m_fUsedHeight;
}

float CMS_Node::GetInsideLeft()
{
	return m_fRight - m_fUsedWidth;
}

CMS_Node* CMS_Node::CursorCast( int x, int y )
{
	std::list< CMS_Node* >::iterator itrNode = m_nodeChildren.begin();

	while( itrNode != m_nodeChildren.end() )
	{
		CMS_Node* pReturn = (*itrNode)->CursorCast( x, y );
		if( pReturn )
			return pReturn;

		itrNode++;
	}

	if( m_bTargetable )
	{
		if( x > m_fLeft && x < m_fRight )
		{
			if( y > m_fTop && y < m_fBottom )
			{
				if( m_bActive )
					return this;
			}
		}
	}

	return nullptr;
}

void CMS_Node::OnCursorHit()
{
	if( m_szAltImage.length() > 0 )
	{
		m_nCurrentImage = m_mapImages[ m_szAltImage ];
	}
}

void CMS_Node::OnCursorLeave()
{
	if( m_szOrgImage.length() > 0 )
	{
		m_nCurrentImage = m_mapImages[ m_szOrgImage ];
	}
}