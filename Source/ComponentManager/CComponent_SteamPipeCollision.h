#ifndef CCOMPONENT_STEAMPIPECOLLISION_H
#define	CCOMPONENT_STEAMPIPECOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Josh Fields
// Date:                5/16/12
// Purpose:             Defines the collisions and physics for the player.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
//#include "../PhysicsEnvironment/PhysicsEnvironment.h"
//#include "../Utilities/CollisionLib/CollisionShapes.h"
//#include "../RenderEngine/DebugShapes.h"

// TODO
// these values need to be tested
// and adjusted
#define STEAM_FORCE 50.0f
#define STEAM_PUSHOUT 4500.0f
#define PLAYER_STEAM_FORCE 500.0f
#define STEAM_AABB_OFFSET 0.0f
#define STEAM_AABB_WIDTH 50.0f

// FORWARD DECLARATIONS
class CComponent_Sound;
class CPM_ParEmitter;
class CEffects;

__declspec(align(32))
class CComponent_SteamPipeCollision : public CComponent_Collision
{

private:
	AABB m_cvSteam;
	IBaseObject* m_pPlayer;
	CComponent_Sound* m_pSoundComponent;
	CObjectManager* m_pObjectMngr;
	bool m_bSteamOn;
	bool m_bCurrentlyLooping;
	bool m_bCorked;
	bool m_bPrevOff;
	


	// Pionter to the collision volume
	AABB* m_pCollisionObject;

	// frequency timer
	float m_fFrequencyTimer;
	float m_fDurationTimer;
	
	// Direction of the steam
	// used for push out
	vec3f m_SteamDir;
	// Line for Mid-steam wall
	Line m_SteamLine;
	int index;

	// Attribs
	bool m_bConstant;
	double m_dDuration;
	double m_dFrequency;
	int m_nChannel;
	int m_nLength;
	bool Steam;
	float deltaTime;
	int indexForActivePart;

	float fDuration;
public:
	CPM_ParEmitter* m_pSteamEffect;
	CComponent_SteamPipeCollision();
	~CComponent_SteamPipeCollision(void);

	bool Initialize ();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	bool GetSteamOn() { return m_bSteamOn; }
	//vec3f GetSteamDir() const { return m_SteamDir; }
	AABB GetSteamAABB() const { return m_cvSteam; }
	bool GetConstant() const { return m_bConstant; }
	double GetDuration() const { return m_dDuration; }
	double GetFrequency() const { return m_dFrequency; }
	int GetChannel() const { return m_nChannel; }
	bool GetCorked() const { return m_bCorked; }
	int GetLength() const { return m_nLength; }
	vec3f GetDir() { return m_SteamDir; }
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetSteamOn(bool val) { m_bSteamOn = val; }
	//void SetSteamDir(vec3f val) { m_SteamDir = val; }
	void SetSteamAABB(AABB val) { m_cvSteam = val; }
	void SetConstant(bool val) { m_bConstant = val; }
	void SetDuration(double val) { m_dDuration = val; }
	void SetFrequency(double val) { m_dFrequency = val; }
	void SetChannel(int val) { m_nChannel = val; }
	void SetCorked(bool val) { m_bCorked = val; }
	void SetLength( int val ) { m_nLength = val; }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );
	void Render();
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: BuildSteamAABB()
	// Purpose: Create an AABB for the steam
	// Original Author: Rueben Massey
	// Creation Date: 6/5/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildSteamAABB( int nLength, int nDirection );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckSteamCollisions()
	// Purpose: Check for collision with the steam
	//			and react appropriately
	// Original Author: Rueben Massey
	// Creation Date: 6/6/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckSteamCollisions( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckCollisions()
	// Purpose: Check for collision	and react appropriately
	// Original Author: Rueben Massey
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckCollisions( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckDynamicCollisions()
	// Purpose: Check collisions with all dynamic objects
	// Original Author: Rueben Massey
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

	void UpdateSteamLine (vec3f _v3Pos);
};

#endif