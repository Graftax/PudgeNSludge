/////////////////////////////////////////////////////////////
//	Title:		CComponenet_ButtonCollision
//
//	Purpose:	This class handles the collision with the button.
//				It will tell the box spawn to spawn a box, and
//				the doors to open, when stepped on.
//
//	Name:		Josh Morgan
//	Date:		7/8/12
//////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_BUTTONCOLLISION_H_
#define CCOMPONENT_BUTTONCOLLISION_H_

#include "CComponent_Collision.h"
#include "CComponent_Animation.h"
#include "../HUD/CIcon.h"

class CComponent_Sound;
class CComponent_ButtonCollision : public CComponent_Collision
{
private:
	bool m_bPressed, m_bPrevPressed;//tells us if the button was pressed and if it was pressed last frame
	IBaseObject* m_pPartner;		//this partner is the object that the button is connected to
	int m_nChannel;					//this tells use the channel to use to find our partner
	int m_nDirection;
	AABB* m_pCollisionObj;
	AABB  m_pOriginalCollision;		
	bool m_bPrevDone;				//this is set to show that the animation was done last frame


	std::string m_szWireMeshName;
	RenderNode* m_pWireMesh;
	int m_nWireTexture;
	int m_nScrollingWireTexture;

	// Icon
	//CIcon m_cButtonIcon;
	CIcon* m_pIcon;
	int m_nIconTexture1;
	int m_nIconTexture2;
	short m_sCount;

	CComponent_Animation* m_pAnimComponent;
	CComponent_Sound* m_pSoundComponent;
	bool ButtonUp;

public:
	CComponent_ButtonCollision(void);
	~CComponent_ButtonCollision(void);

	/////////////////////////////////////////////////////////////////////////
	//	Initialize():	Used to initialize the values of member variables.
	//	Returns:		bool - returns true if everything succeeded in
	//					initializing.
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	bool Initialize(void);

	/////////////////////////////////////////////////////////////////////////
	//	Update():		Updates and checks the current collisions of the 
	//					button.
	//	Returns:		void
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void Update(float _fDT);

	/////////////////////////////////////////////////////////////////////////
	//	Update():		Updates the Collision when the Button drops.
	//	Returns:		void
	//	Mod Date:		7/12/12
	//	Mod Name:		Andy Madruga
	/////////////////////////////////////////////////////////////////////////
	void DropButtonAABB( float fDT );

	/////////////////////////////////////////////////////////////////////////
	//	CheckCollisions():	Called by update to check the collisions.
	//	Returns:			void
	//	Mod Date:			7/8/12
	//	Mod Name:			Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void CheckCollisions( float _fDT );

	/////////////////////////////////////////////////////////////////////////
	//	CheckDynamicCollisions():	Checks for collisions with any debris
	//								so it can be pushed out accordingly.
	//	Returns:					void
	//	Mod Date:					7/8/12
	//	Mod Name:					Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions(	CComponent_Collision* _pCurCollision, 
									CCollisionVolume* _pCurVolume, 
									float _fDT );

	/////////////////////////////////////////////////////////////////////////
	//	Shutdown():		Cleans up any memory allocated, and resets variables.
	//	Returns:		void
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void Shutdown(void);

	///////////////
	// Accessors //
	///////////////
	bool GetPressed(void) { return m_bPressed; }
	int GetDirection(void) { return m_nDirection; }

	//////////////
	// Mutators //
	//////////////
	void SetPartner(IBaseObject* _pPartner) { m_pPartner = _pPartner; }
	void SetChannel(int _nChannel) { m_nChannel = _nChannel; }
	void SetDirection(int _nDirection) { m_nDirection = _nDirection; }
	void SetWireMeshName( std::string _szWireName );
};

#endif //CCOMPONENT_BUTTONCOLLISION_H_