#ifndef CMS_PAGE_H
#define CMS_PAGE_H


#include <list>
#include "../EffectsLib/TinyXML/tinyxml.h"
#include "CMS_Node.h"

#define MENU_PATH "Resources/Menus/"

enum CMS_Direction{ CMSD_NULL, CMSD_UP, CMSD_LEFT, CMSD_RIGHT, CMSD_DOWN };

class CMenuSystem;
class CMS_Page
{
private:

	CMenuSystem* m_pOwner;

	CMS_Node* m_nodeRoot;
	float m_fOffsetX;
	float m_fOffsetY;
	float m_fOffsetRateX;
	float m_fOffsetRateY;
	float m_fBGM;

	std::list< CMS_Node* > m_listLeafs;
	std::list< CMS_Node* > m_listObjects;

	std::map< std::string, float > m_mapValueBuffer;

	CMS_Node* m_pCurrentSelection;
	CMS_Node* m_pCursorSelection;

	void ProcessNode( CMS_Node* _newNode, CMS_Node* _parent, TiXmlElement* _element );
	void SaveVariablesToBuffer( CMS_Node* _traverNode );

public:

	CMS_Page(){}
	CMS_Page( const char* _toLoad, CMenuSystem* _master );
	CMS_Page( const CMS_Page& );
	CMS_Page& operator = ( const CMS_Page& );
	~CMS_Page();

	void Initialize();
	void Shutdown();

	void Update( float fDT, int _posX, int _posY );
	void Render();
	void Move( CMS_Direction _toMove );
	void RecalcValues();

	void Activate();
	void Activate( CMS_Node* _target );

	void SetValue( bool _state, char* _value );

	CMS_Node* InsertNode( CMS_Node* _parent, CMS_Node* _toInsert );

	void RefreshBuffers();

	friend class CMenuSystem;
	friend class CMS_Node;
};

#endif