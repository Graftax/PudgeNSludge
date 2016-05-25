// Title:		CComponent_BoxPipeCollision.h
// Purpose:		This component will handle collisions between objects
//				in the game and itself. It will also handle the spawning
//				of objects whenever the corresponding button is hit.
// Author:		Josh Morgan
// Date:		6/28/12
////////////////////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_BOXPIPECOLLISION_H_
#define CCOMPONENT_BOXPIPECOLLISION_H_

#include "CComponent_BasePipe.h"

class CComponent_BoxPipeCollision : public CComponent_BasePipe
{
private:
	bool m_bButtonPressed;		//set by the button to tell us to spawn a box
	bool m_bPrevButtonPressed;	//set to true if we were down last frame
	float m_fSpawnTimer;

public:
	CComponent_BoxPipeCollision(void);
	~CComponent_BoxPipeCollision(void);

	/////////////////////////////////////////////////////////////////////////
	//	Initialize():	Initializes variables, mainly m_bButtonPressed
	//	Returns:		void
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	bool Initialize(void);

	/////////////////////////////////////////////////////////////////////////
	//	Update():		Updates, checks collisions, and spawns boxes
	//	Returns:		void
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void Update(float fDT);

	/////////////////////////////////////////////////////////////////////////
	//	Render():		Just hear because of my parents
	//	Returns:		void
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void Render(void);

	/////////////////////////////////////////////////////////////////////////
	//	Shutdown():		Cleans up and allocated memory
	//	Returns:		void
	//	Mod Date:		7/8/12
	//	Mod Name:		Josh Morgan
	/////////////////////////////////////////////////////////////////////////
	void Shutdown(void);

	///////////////
	// Accessors //
	///////////////
	
	//////////////
	// Mutators //
	//////////////
	void SetPressed(bool _bPressed) { m_bButtonPressed = _bPressed; }
};

#endif //CCOMPONENT_BOXPIPECOLLISION_H_