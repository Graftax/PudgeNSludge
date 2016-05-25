#include "CMS_Page.h"

#include "CMenuSystem.h"
#include "../RenderEngine/TextureManager.h"
#include "../AudioLib/AudioManager.h"

void CMS_Page::ProcessNode( CMS_Node* _newNode, CMS_Node* _parent, TiXmlElement* _element )
{
	_newNode->m_pMaster = this;

	double fValBuffer = 0.0f;

	char type[8];
	strcpy_s( type, 8, _element->Value() );

	// FIND THE TYPE
	const char* szType = _element->Value();

	const char* src = _element->Attribute( "src" );
	if( src )
	{
		std::string extendedAddress = std::string( MENU_PATH );
		extendedAddress += src;
		_newNode->AddImage( extendedAddress.c_str() );
	}

	if( strcmp( szType, "div" ) == 0 )
	{
		_newNode->m_enType = CMSNT_DIV;
	}
	else if( strcmp( szType, "img" ) == 0 )
	{
		_newNode->m_enType = CMSNT_IMG;
	}
	else if( strcmp( szType, "body" ) == 0 )
	{
		_newNode->m_enType = CMSNT_BODY;
	}

	const char* alt = _element->Attribute( "alt" );
	if( alt )
	{
		if( strlen( alt ) > 0 )
		{
			std::string extendedAddress = std::string( MENU_PATH );
			extendedAddress += alt;
			_newNode->AddImage( extendedAddress.c_str() );
		}
	}

	const char* check = _element->Attribute( "check" );
	if( check )
	{
		if( strlen( check ) > 0 )
		{
			std::string extendedAddress = std::string( MENU_PATH );
			extendedAddress += check;
			_newNode->m_nCheckImage = TextureManager::GetInstance()->LoadTexture( extendedAddress.c_str() );
		}
	}

	_element->Attribute( "width", &fValBuffer );
	_newNode->m_fWidth = (float)fValBuffer;
	_element->Attribute( "height", &fValBuffer );
	_newNode->m_fHeight = (float)fValBuffer;
	_element->Attribute( "shift", &fValBuffer );
	_newNode->m_fHorizontalOffset = (float)fValBuffer;

	const char* page = _element->Attribute( "page" );
	if( page )
	{
		if( strlen( page ) > 0 )
		{
			m_pOwner->LoadPage( page );
			_newNode->m_szPageString = page;
			m_listObjects.push_back( _newNode );
			_newNode->m_bTargetable = true;
		}
	}

	const char* szAction = _element->Attribute( "action" );
	if( szAction )
	{
		if( strlen( szAction ) > 0 )
		{
			_newNode->m_szActionString = szAction;
			m_listObjects.push_back( _newNode );

			if( strcmp( szAction, "slider" ) == 0 )
				_parent->m_bTargetable = true;

			_newNode->m_bTargetable = true;
		}
	}

	const char* szAlign = _element->Attribute( "align" );
	if( szAlign )
	{
		if( strcmp( szAlign, "left" ) == 0 )
			_newNode->m_amAlignment = CMSAM_LEFT;
		else if( strcmp( szAlign, "right" ) == 0 )
			_newNode->m_amAlignment = CMSAM_RIGHT;
		else if( strcmp( szAlign, "center" ) )
			_newNode->m_amAlignment = CMSAM_CENTER;
	}

	const char* szVar = _element->Attribute( "variable" );
	if( szVar )
	{
		if( strlen( szVar ) > 0 )
		{
			_newNode->m_szVariableString = szVar;
		}
	}

	InsertNode( _parent, _newNode ); 

	TiXmlElement* xmlElements = _element->FirstChildElement();

	while( xmlElements )
	{
		ProcessNode( new CMS_Node(), _newNode, xmlElements );
		xmlElements = xmlElements->NextSiblingElement();
	}

}

void CMS_Page::SaveVariablesToBuffer( CMS_Node* _traverseNode )
{
	if( _traverseNode->m_szVariableString.length() > 0 )
	{
		if( m_pOwner->m_mapVariables.count( _traverseNode->m_szVariableString ) > 0 )
			m_mapValueBuffer[ _traverseNode->m_szVariableString ] = m_pOwner->m_mapVariables[ _traverseNode->m_szVariableString ];
	}

	std::list< CMS_Node* >::iterator itrNode = _traverseNode->m_nodeChildren.begin();
	while( itrNode != _traverseNode->m_nodeChildren.end() )
	{
		SaveVariablesToBuffer( (*itrNode) );
		itrNode++;
	}
}

CMS_Page::CMS_Page( const char* _toLoad, CMenuSystem* _master )
{
	m_pOwner = _master;
	m_pCurrentSelection = nullptr;
	m_pCursorSelection = nullptr;
	m_nodeRoot = nullptr;

	m_fOffsetX = 0.0f;
	m_fOffsetY = 0.0f;
	m_fBGM = 0.0;

	TiXmlDocument xmlDoc;

	std::string szFilename = std::string( MENU_PATH );
	szFilename += std::string( _toLoad );

	xmlDoc.LoadFile( szFilename.c_str() );

	TiXmlElement* xmlRoot = xmlDoc.RootElement();

	if( !xmlRoot )
		return;

	TiXmlElement* xmlElement;

	// At this point, the element is the head.
	xmlElement = xmlRoot->FirstChildElement("body");

	double fValBuffer = 0.0f;
	xmlElement->Attribute( "scrollx", &fValBuffer );
	//m_fOffsetRateX = (float)fValBuffer;
	m_fOffsetRateX = 0.0f;
	xmlElement->Attribute( "scrolly", &fValBuffer );
	m_fOffsetRateY = (float)fValBuffer;

	fValBuffer = 0.0f;
	xmlElement->Attribute( "bgm", &fValBuffer );
	m_fBGM = (float)fValBuffer;

	ProcessNode( new CMS_Node(), nullptr, xmlElement );

	m_nodeRoot->CalcExtents();

	//_toInsert->CalcExtents();
}

CMS_Page::CMS_Page( const CMS_Page& )
{
	m_nodeRoot = nullptr;
	m_pCurrentSelection = nullptr;
}

CMS_Page& CMS_Page::operator = ( const CMS_Page& )
{
	m_nodeRoot = nullptr;
	return *this;
}

CMS_Page::~CMS_Page()
{
	// Need to overload the destructor for memory cleanup.
	// OH GOD THE MEMORY CLEANUP!

	std::list< CMS_Node* > toDelete;
	toDelete.push_front( m_nodeRoot );
	m_nodeRoot = nullptr;

	CMS_Node* pBuffer = nullptr;
	while( toDelete.size() )
	{
		pBuffer = (toDelete.front());
		if( pBuffer )
		{
			std::list< CMS_Node*>::iterator itrNode = pBuffer->m_nodeChildren.begin();
			while( itrNode != pBuffer->m_nodeChildren.end() )
			{
				toDelete.push_back( (*itrNode) );
				itrNode++;
			}
		}

		delete pBuffer;

		toDelete.pop_front();
	}

}

void CMS_Page::Initialize()
{
	SaveVariablesToBuffer( m_nodeRoot );

	if( m_fBGM == 1.0f )
		CGame::GetInstance()->GetAudioManager()->PlayMusic( AK::SWITCHES::MUSIC::SWITCH::PS_MENU );

	if( m_fBGM == 2.0f )
		CGame::GetInstance()->GetAudioManager()->PlayMusic( AK::SWITCHES::MUSIC::SWITCH::PS_CREDITS );
}

void CMS_Page::Shutdown()
{
	if( m_fBGM == 2.0f )
		CGame::GetInstance()->GetAudioManager()->PlayMusic( AK::SWITCHES::MUSIC::SWITCH::PS_MENU );
}

void CMS_Page::Update( float fDT, int _posX, int _posY )
{
	m_fOffsetX += m_fOffsetRateX * fDT;
	m_fOffsetY += m_fOffsetRateY * fDT;

	if( !m_pOwner->m_bPrevClick && m_pOwner->m_bCurrClick )
	{
		CMS_Node* pTarget = m_nodeRoot->CursorCast( _posX, _posY );
		if( pTarget )
		{
			m_pCursorSelection = pTarget;
			Activate( m_pCursorSelection );
		}
	}
	else
	{
		if( !m_pOwner->m_bCurrClick )
			m_pCursorSelection = false;
	}

	// Loop throught the objects
	std::list< CMS_Node* >::iterator itrObj = m_listObjects.begin();
	while( itrObj != m_listObjects.end() )
	{
		CMS_Node* pCurrNode = (*itrObj);

		if( strcmp( pCurrNode->m_szActionString.c_str(), "slider" ) == 0 )
		{
			float percentage = m_pOwner->m_mapVariables[pCurrNode->m_szVariableString] / 100.0f;

			float min = ( pCurrNode->m_pParent->m_fLeft + pCurrNode->m_fWidth / 2 );
			float max = ( pCurrNode->m_pParent->m_fRight - pCurrNode->m_fWidth / 2 );

			float offset = min + percentage * ( max - min );

			pCurrNode->m_fLeft = ( offset - pCurrNode->m_fWidth / 2.0f );
			pCurrNode->m_fRight = ( offset + pCurrNode->m_fWidth / 2.0f );
		}
		if( strcmp( pCurrNode->m_szActionString.c_str(), "button" ) == 0 )
		{
			if( m_pOwner->m_mapVariables[ pCurrNode->m_szVariableString ] > 0.0f )
				pCurrNode->m_bChecked = true;
			else
				pCurrNode->m_bChecked = false;
		}

		itrObj++;
	}

	if( m_pCursorSelection )
	{
		if( m_pCursorSelection->m_szActionString.length() > 0 )
		{
			if( strcmp( m_pCursorSelection->m_szActionString.c_str(), "slider" ) == 0 )
			{
				m_pCursorSelection->m_fLeft = _posX - m_pCursorSelection->m_fWidth/2;
				m_pCursorSelection->m_fRight = _posX + m_pCursorSelection->m_fWidth/2;

				float offset = m_pCursorSelection->m_fLeft - m_pCursorSelection->m_pParent->m_fLeft;
				if( offset < 0 )
				{
					 m_pCursorSelection->m_fLeft -= offset;
					 m_pCursorSelection->m_fRight -= offset;
				}
				offset = m_pCursorSelection->m_fRight - m_pCursorSelection->m_pParent->m_fRight;
				if( offset > 0 )
				{
					 m_pCursorSelection->m_fLeft -= offset;
					 m_pCursorSelection->m_fRight -= offset;
				}
			}
		}
	}
	else
	{
		if( m_pOwner->m_bMouseMoved )
		{
			CMS_Node* pTarget = m_nodeRoot->CursorCast( _posX, _posY );
			if( pTarget )
			{
				if( m_pCurrentSelection != pTarget )
					CGame::GetInstance()->GetAudioManager()->PlayEffect( AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT );

				m_pCurrentSelection = pTarget;
			}
		}
	}

	if( m_pCurrentSelection )
		m_pCurrentSelection->m_bCursorBuffer= true;

	if( m_nodeRoot )
		m_nodeRoot->Update();
}

void CMS_Page::Render()
{
	// Setup for breadth-first traversal.
	std::list< CMS_Node* > listRenderables;
	listRenderables.push_front( m_nodeRoot );

	CMS_Node* pBuffer = nullptr;
	while( listRenderables.size() )
	{
		pBuffer = (listRenderables.front());
		if( pBuffer )
		{
			pBuffer->Render();
			
			std::list< CMS_Node*>::iterator itrNode = pBuffer->m_nodeChildren.begin();
			while( itrNode != pBuffer->m_nodeChildren.end() )
			{
				listRenderables.push_back( (*itrNode) );
				itrNode++;
			}
		}

		listRenderables.pop_front();
	}

}

void CMS_Page::Move( CMS_Direction _toMove )
{
	if( _toMove == CMSD_UP || _toMove == CMSD_DOWN )
	{
		int currIS = 0;
		std::list< CMS_Node* >::iterator itrNode = m_listObjects.begin();
		for( unsigned int i = 0; i < m_listObjects.size(); ++i )
		{
			if( (*itrNode) == m_pCurrentSelection )
			{
				currIS = i;
			}

			itrNode++;
		}

		switch( _toMove )
		{
		case CMSD_UP:
			currIS--;
			break;
		case CMSD_DOWN:
			currIS++;
			break;
		}

		if( currIS < 0 )
			currIS = 0;
		if( currIS > (int)m_listObjects.size() - 1 )
			currIS = m_listObjects.size() - 1;

		itrNode = m_listObjects.begin();
		for( unsigned int i = 0; i < m_listObjects.size(); ++i )
		{
			if( i == currIS )
			{
				m_pCurrentSelection = (*itrNode);
			}

			itrNode++;
		}

		if( m_pCurrentSelection )
			if( !m_pCurrentSelection->m_bActive )
				Move( _toMove );
	}
	else
	{
		switch( _toMove )
		{
		case CMSD_LEFT:
			{
				if( m_pCurrentSelection )
				{
					if( m_pCurrentSelection->m_szActionString.length() )
					{
						if( strcmp( m_pCurrentSelection->m_szActionString.c_str(), "slider" ) == 0 )
						{
							m_pCurrentSelection->m_fLeft -= 64;
							m_pCurrentSelection->m_fRight -= 64;

							float offset = m_pCurrentSelection->m_fLeft - m_pCurrentSelection->m_pParent->m_fLeft;
							if( offset < 0 )
							{
								m_pCurrentSelection->m_fLeft -= offset;
								m_pCurrentSelection->m_fRight -= offset;
							}
						}
					}
				}

				break;
			}
		case CMSD_RIGHT:
			{
				if( m_pCurrentSelection )
				{
					if( m_pCurrentSelection->m_szActionString.length() )
					{
						if( strcmp( m_pCurrentSelection->m_szActionString.c_str(), "slider" ) == 0 )
						{
							m_pCurrentSelection->m_fLeft += 64;
							m_pCurrentSelection->m_fRight += 64;

							float offset = m_pCurrentSelection->m_fRight - m_pCurrentSelection->m_pParent->m_fRight;
							if( offset > 0 )
							{
								m_pCurrentSelection->m_fLeft -= offset;
								m_pCurrentSelection->m_fRight -= offset;
							}
						}
					}
				}
				break;
			}
		}
	}

}

void CMS_Page::RecalcValues()
{
	std::list< CMS_Node* >::iterator itrObj = m_listObjects.begin();
	while( itrObj != m_listObjects.end() )
	{
		CMS_Node* pCurrNode = (*itrObj);

		if( strcmp( pCurrNode->m_szActionString.c_str(), "slider" ) == 0 )
		{
			float extent = (pCurrNode->m_pParent->m_fRight - pCurrNode->m_fWidth/2.0f) - (pCurrNode->m_pParent->m_fLeft + pCurrNode->m_fWidth/2.0f);
			float offset = ( pCurrNode->m_fLeft + pCurrNode->m_fWidth / 2.0f ) - ( pCurrNode->m_pParent->m_fLeft + pCurrNode->m_fWidth / 2.0f );
			float percentage = offset / extent;
			
			if( pCurrNode->m_szVariableString.length() > 0 ) 
			{
				if( m_pOwner->m_mapVariables.count( pCurrNode->m_szVariableString ) )
					m_pOwner->m_mapVariables[ pCurrNode->m_szVariableString ] = (percentage * 100.0f);
			}
		}
		if( strcmp( pCurrNode->m_szActionString.c_str(), "button" ) == 0 )
		{
			if( pCurrNode->m_bChecked )
				m_pOwner->m_mapVariables[ pCurrNode->m_szVariableString ] = 1.0f;
			else
				m_pOwner->m_mapVariables[ pCurrNode->m_szVariableString ] = 0.0f;
		}

		itrObj++;
	}
}

void CMS_Page::Activate()
{
	if( m_pCurrentSelection )
		Activate(m_pCurrentSelection);
}

void CMS_Page::Activate( CMS_Node* _target )
{
	if( _target )
	{
		CGame::GetInstance()->GetAudioManager()->PlayEffect( AK::EVENTS::PLAY_SFX_MENU_ACCEPT );

		if( _target->m_szActionString.length() > 0 )
		{
			if( strcmp( _target->m_szActionString.c_str(), "play" ) == 0 )
			{
				m_pOwner->Play();
			}
			if( strcmp( _target->m_szActionString.c_str(), "back" ) == 0 )
			{
				RefreshBuffers();
				m_pOwner->Back();
			}
			if( strcmp( _target->m_szActionString.c_str(), "accept" ) == 0 )
			{
				m_pOwner->Back();
			}
			if( strcmp( _target->m_szActionString.c_str(), "reset" ) == 0 )
			{
				m_pOwner->m_mapVariables[ "RESET" ] = 1.0f;
			}
			if( strcmp( _target->m_szActionString.c_str(), "button" ) == 0 )
			{
				if( _target->m_bChecked )
				{
					_target->m_bChecked = false;
					m_pOwner->m_mapVariables[ _target->m_szVariableString ] = 0.0f;
				}
				else
				{
					_target->m_bChecked = true;
					m_pOwner->m_mapVariables[ _target->m_szVariableString ] = 1.0f;
				}
			}
		}
		if( _target->m_szPageString.length() > 0 )
		{
			m_pOwner->StackPage( _target->m_szPageString.c_str() );
		}
		
		std::list< CMS_Node* >::iterator itrNode = _target->m_nodeChildren.begin();
		while( itrNode != _target->m_nodeChildren.end() )
		{
			if( strcmp( (*itrNode)->m_szActionString.c_str(), "slider" ) == 0 )
			{
				if( (*itrNode)->m_bActive )
				{
					m_pCursorSelection = (*itrNode);
					m_pCurrentSelection = (*itrNode);
				}
			}

			itrNode++;
		}
	}
}

void CMS_Page::SetValue( bool _state, char* _value )
{
	std::list< CMS_Node* >::iterator itrNode = m_listObjects.begin();

	while( itrNode != m_listObjects.end() )
	{
		if( strcmp( (*itrNode)->m_szVariableString.c_str(), _value ) == 0 )
			(*itrNode)->m_bActive = _state;

		itrNode++;
	}
}

CMS_Node* CMS_Page::InsertNode( CMS_Node* _parent, CMS_Node* _toInsert )
{

	if( !_toInsert )
		return _toInsert;

	_toInsert->m_pParent = _parent;

	if( !_toInsert->m_pParent )
		m_nodeRoot = _toInsert;
	else
	{
		// Check to remove the parent from the list of leafs.
		std::list< CMS_Node* >::iterator itrNode = m_listLeafs.begin();
		while( itrNode != m_listLeafs.end() )
		{
			if( (*itrNode) == _parent )
			{
				itrNode = m_listLeafs.erase( itrNode );
				break;
			}

			itrNode++;
		}

		switch( _parent->m_amAlignment )
		{
		case CMSAM_LEFT:
			_parent->m_nodeChildren.push_back( _toInsert );
			break;
		case CMSAM_RIGHT:
			_parent->m_nodeChildren.push_front( _toInsert );
			break;
		}
	}

	// Add it to list of leaves.
	m_listLeafs.push_front( _toInsert );

	return _toInsert;
}

void CMS_Page::RefreshBuffers()
{
	std::map< std::string, float >::iterator valItr = m_mapValueBuffer.begin();
	while( valItr != m_mapValueBuffer.end() )
	{
		m_pOwner->m_mapVariables[ (*valItr).first ] = (*valItr).second;
		valItr++;
	}
}