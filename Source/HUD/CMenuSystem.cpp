#include "CMenuSystem.h"

	void CMenuSystem::LoadPage( const char* _toLoad )
	{
		std::string name = std::string( _toLoad );

		if( !m_mapBook.count( name ) )
		{
			m_mapBook[ name ] = new CMS_Page( name.c_str(), this );
		}
	}

	void CMenuSystem::StackPage( const char* _toStack )
	{
		if( m_mapBook.count( std::string( _toStack ) ) > 0 )
		{
			CMS_Page* pToPush = m_mapBook[ std::string( _toStack ) ];
			if( pToPush )
			{
				m_fInputWaitTimer = 0.0f;

				pToPush->m_fOffsetX = 0.0f;
				pToPush->m_fOffsetY = 0.0f;

				if( pToPush->m_listObjects.size() > 0 )
					pToPush->m_pCurrentSelection = pToPush->m_listObjects.front();

				m_listPageStack.push_back( pToPush );
				pToPush->Initialize();
			}
		}
	}

	void CMenuSystem::Back()
	{
		if( m_listPageStack.size() > 1 )
		{
			m_listPageStack.back()->Shutdown();
			m_listPageStack.pop_back();
			m_listPageStack.back()->m_fOffsetX = 0.0f;
			m_listPageStack.back()->m_fOffsetY = 0.0f;
		}
		else
			m_bRunning = false;
	}

	void CMenuSystem::Initialize( char* _start )
	{
		m_bShouldPlay = false;
		m_bRunning = true;

		m_bPrevClick = false;
		m_bCurrClick = false;

		m_bMouseMoved = false;
		m_fInputWaitTimer = 0.0f;

		LoadPage( _start );
		StackPage( _start );
	}

	void CMenuSystem::Update( float fDT, int _cursorX, int _cursorY  )
	{
		m_fInputWaitTimer += fDT;

		if( m_nMouseX != _cursorX || m_nMouseY != _cursorY )
			m_bMouseMoved = true;
		else
			m_bMouseMoved = false;

		m_nMouseX = _cursorX;
		m_nMouseY = _cursorY;
		
		if( m_listPageStack.size() )
		{
			(m_listPageStack.back())->Update( fDT, _cursorX, _cursorY );
		}

		m_bPrevClick = m_bCurrClick;
		m_bCurrClick = false;
	}

	void CMenuSystem::Render()
	{
		if( m_fInputWaitTimer < 0.1f )
			return;

		if( m_listPageStack.size() )
		{
			(m_listPageStack.back())->Render();
		}
	}

	void CMenuSystem::Move( CMS_Direction _toMove )
	{
		if( m_listPageStack.size() )
		{
			(m_listPageStack.back())->Move( _toMove );
		}
	}

	void CMenuSystem::Activate()
	{
		if( m_listPageStack.size() )
		{
			(m_listPageStack.back())->Activate();
		}
	}

	float CMenuSystem::GetValue( char* _name )
	{
		if( m_mapVariables.count( std::string( _name ) ) > 0 )
		{
			return m_mapVariables[ _name ];
		}
		else
		{
			return 0;
		}
	}

	void CMenuSystem::SetValue( char* _name, float _value )
	{
		m_mapVariables[ std::string( _name ) ] = _value;
	}

	void CMenuSystem::RecalcValues()
	{
		if( m_fInputWaitTimer < 0.1f )
			return;

		if( m_listPageStack.size() )
		{
			(m_listPageStack.back())->RecalcValues();
		}
	}

	void CMenuSystem::SetActive( bool _bState, char* valName )
	{
		if( m_listPageStack.size() )
		{
			(m_listPageStack.back())->SetValue( _bState, valName );
		}
	}

	void CMenuSystem::ClearLibrary()
	{
		m_mapBook.clear();
	}
	void CMenuSystem::ClearStack()
	{
		m_listPageStack.clear();
	}

	void CMenuSystem::Shutdown()
	{
		m_bRunning = false;
		ClearStack();

		std::map< std::string, CMS_Page* >::iterator itrPage = m_mapBook.begin();
		while( itrPage != m_mapBook.end() )
		{
			delete (*itrPage).second;
			itrPage++;
		}

		ClearLibrary();
	}