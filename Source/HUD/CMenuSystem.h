#ifndef CMENUSYSTEM_H
#define CMENUSYSTEM_H

#include <map>
#include <list>
#include <string>

#include "CMS_Page.h"

class CMenuSystem
{

private:

	bool m_bRunning;
	bool m_bShouldPlay;

	float m_fInputWaitTimer;

	std::map< std::string, CMS_Page* > m_mapBook;
	std::list< CMS_Page* > m_listPageStack;

	std::map< std::string, float > m_mapVariables;

public:

	int m_nMouseX;
	int m_nMouseY;
	bool m_bMouseMoved;

	bool m_bPrevClick;
	bool m_bCurrClick;

	bool ShouldPlay() { return m_bShouldPlay; }
	void Play() { m_bShouldPlay = true; }
	void StopPlay() { m_bShouldPlay = false; }
	bool GetRunning() { return m_bRunning; }
	void MouseDown(){ m_bCurrClick = true; }

	void Initialize( char* _start );
	void LoadPage( const char* _toLoad );
	void StackPage( const char* _toStack );

	void Update( float fDT,  int _cursorX, int _cursorY  );
	void Render();

	void Move( CMS_Direction _toMove );
	void Activate();
	void Back();

	float GetValue( char* _name );
	void SetValue( char* _name, float _value );
	void RecalcValues();

	void SetActive( bool _bState, char* valName );

	void ClearLibrary();
	void ClearStack();
	void Shutdown();

	friend class CMS_Page;
};

#endif