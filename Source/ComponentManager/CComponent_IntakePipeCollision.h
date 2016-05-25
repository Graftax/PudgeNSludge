#ifndef CCOMPONENT_INTAKEPIPECOLLISIONH
#define CCOMPONENT_INTAKEPIPECOLLISIONH

#include "CComponent_Collision.h"
#include "../EffectsLib/ParticleManager.h"
#include "../HUD/CIcon.h"

class CComponent_SteamPipeCollision;
__declspec(align(32))
class CComponent_IntakePipeCollision : public CComponent_Collision
{

private:

	AABB m_abTarget;
	bool m_bCorked;
	CObjectManager* m_pObjectMngr;
	IBaseObject* m_pPartner;

	//Pipes to stop flow on when corked
	string m_szFlowPipeName;
	RenderNode* m_pFlowPipe;

	// Pointer to the collision volume
	AABB* m_pCollisionObject;

	bool m_bIsGettingCorked;
	CComponent_Collision* m_pCork;

	CPM_ParEmitter* m_pIntakeParticleEffect;

	// Attribs
	bool m_bCorkable;
	int m_nChannel;
	int m_nDirection;

	// Icon
	CIcon* m_pIcon;
	int m_nIconTexture;
	int m_nIconTexture2;
	short m_sCount;

public:

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	AABB GetTargetAABB() const { return m_abTarget; }
	bool GetCorkable() const { return m_bCorkable; }
	int GetChannel() const { return m_nChannel; }
	string GetFlowPipeNameString(void) { return m_szFlowPipeName; }
	const char* GetFlowPipeNameChar(void) { return m_szFlowPipeName.c_str(); }
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetTargetAABB(AABB val) { m_abTarget = val; }
	void SetPartner( IBaseObject* _partner ) { m_pPartner = _partner; } 
	void SetCorkable(bool val) { m_bCorkable = val; }
	void SetChannel(int val) { m_nChannel = val; }
	void SetDirection( int val ) { m_nDirection = val; }
	void SetFlowMeshName( string _flow );


	CComponent_IntakePipeCollision();
	~CComponent_IntakePipeCollision();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialize():	Guess
	//
	// Returns:			bool
	//
	// Mod. Name:		Ethan Pendergraft
	// Mod. Date:		6/2/2012
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Initialize();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():		Guess
	//
	// Returns:			Void
	//
	// Mod. Name:		Ethan Pendergraft
	// Mod. Date:		6/2/2012
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shutdown():		Guess
	//
	// Returns:			Void
	//
	// Mod. Name:		Ethan Pendergraft
	// Mod. Date:		6/3/2012
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: BuildCorkableAABB()
	// Purpose: Create an AABB for the cork to collide with
	// Original Author: Rueben Massey
	// Creation Date: 6/5/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildCorkableAABB();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckCollisions()
	// Purpose: Check for collision	and react appropriately
	// Original Author: Rueben Massey
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckCorkableAABB( float fDT );

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
	// Creation Date: 6/11/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: MoveCork()
	// Purpose: move the cork to the correct position
	// Original Author: Rueben Massey
	// Creation Date: 6/16/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void MoveCork( float fDT );
};

#endif //CCOMPONENT_INTAKEPIPECOLLISIONH