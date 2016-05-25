#ifndef CCOMPONENT_SPAWNPIPECOLLISION_H
#define	CCOMPONENT_SPAWNPIPECOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_SpawnPipeCollision.h
// Author:              Bryan Sauter
// Date:                6/2/12
// Purpose:             Defines the collisions and physics for the player.
//////////////////////////////////////////////////////////////////////////////////////



#include "CComponent_Collision.h"
#include "../Utilities/Timer/UTimer.h"
class CComponent_Sound;
class CComponent_Emitter;

// TODO
// these values need to be tested
// and adjusted
#define SPAWNCORKABLE_AABB_HEIGHT	20.0f
#define SPAWNCORKABLE_AABB_WIDTH	100.0f
#define	SPAWNCORKABLE_AABB_OFFSET	20.0f

class CComponent_SpawnPipeCollision : public CComponent_Collision
{
private:
	AABB m_abTarget;
	bool m_bCurrentlyActive;
	bool m_bCorked;
	int  m_nActiveEnemies;

	// Attribs
	bool m_bCorkable;
	unsigned int m_unMaxEnemies;
	float m_fFrequency;	
	float m_fFreqTimer;

public:

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	bool GetCorkable() const { return m_bCorkable; }
	unsigned int GetMaxEnemies() const { return m_unMaxEnemies; }
	double GetFrequency() const { return m_fFrequency; }
	int GetActiveEnemies () { return m_nActiveEnemies; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetCorkable(bool val) { m_bCorkable = val; }
	void SetMaxEnemies(unsigned int val) { m_unMaxEnemies = val; }
	void SetFrequency(float val) { m_fFrequency = val; }
	void SetActiveEnemies ( int _nActiveEnemies ) { m_nActiveEnemies = _nActiveEnemies; }

	CComponent_SpawnPipeCollision(void);
	~CComponent_SpawnPipeCollision(void);

	bool Initialize(void);
	void Update( float fDT );
	void Render(void);
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: BuildCorkableAABB()
	// Purpose: Create an AABB for the cork to collide with
	// Original Author: Rueben Massey
	// Creation Date: 6/5/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildCorkableAABB( int nDirection );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckDynamicCollisions()
	// Purpose: Check collisions with all dynamic objects
	// Original Author: Rueben Massey
	// Creation Date: 6/11/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

};












#endif