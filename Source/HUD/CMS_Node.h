#ifndef CMS_NODE_H
#define CMS_NODE_H

#include <list>
#include <map>
#include <string>

enum CMS_AlignMode{ CMSAM_NONE, CMSAM_LEFT, CMSAM_CENTER, CMSAM_BOTTOM, CMSAM_RIGHT };
enum CMS_NodeType{ CMSNT_NULL, CMSNT_BODY, CMSNT_DIV, CMSNT_IMG };

class CMS_Page;
class CMS_Node
{

private:

	CMS_Node* m_pParent;
	CMS_Page* m_pMaster;

	std::list< CMS_Node* > m_nodeChildren;
	std::map< std::string, int > m_mapImages;
	int m_nCurrentImage;
	int m_nCheckImage;
	std::string m_szOrgImage;
	std::string m_szAltImage;
	std::string m_szPageString;
	std::string m_szActionString;
	std::string m_szVariableString;

	bool m_bCurrCursorTarget;
	bool m_bPrevCursorTarget;
	bool m_bCursorBuffer;
	
	bool m_bTargetable; 
	bool m_bChecked;
	bool m_bActive;

	CMS_AlignMode m_amAlignment;
	CMS_NodeType m_enType;

	float m_fTop;
	float m_fLeft;
	float m_fRight;
	float m_fBottom;

	float m_fHorizontalOffset;

	float m_fWidth;
	float m_fHeight;

	float m_fUsedWidth;
	float m_fUsedHeight;

	float m_fBottomExtents;

	
	void CalcExtents();
	void UpdateExtents( float _update );

	float GetLargestChildHeight();

public:

	CMS_Node();

	void CursorHover();

	void Update();
	void Render();

	void AddImage( const char* _address );

	float GetInsideRight();
	float GetInsideBottom();
	float GetInsideLeft();

	CMS_Node* CursorCast( int x, int y );

	void OnCursorHit();
	void OnCursorLeave();

	friend class CMS_Node;
	friend class CMS_Page;



};
#endif