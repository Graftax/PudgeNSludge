//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CBossAttackAI.cpp
// Author:              Josh Fields
// Date:                8/22/12
// Purpose:             Defines the boss attack functions
//////////////////////////////////////////////////////////////////////////////////////
#include "CBossAttackAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "CBossMoveAI.h"
#include "../RenderEngine/TextureManager.h"

// Include All Animation Files.
#include "../AnimationLib/AnimationIncludes.h"

#define GOO_DROP_WAIT 0.5f
#define CORNER_WAIT 1.0f
#define MAX_GOOS_BEFORE_ALT 3

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BossAttackAI( IBaseObject* _pParent ): Constructor for the boss taking in the parent object	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BossAttackAI::BossAttackAI( IBaseObject* _pParent ) : IBaseAI( _pParent )
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~BossAttackAI(): Destructor for the boss.	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BossAttackAI::~BossAttackAI()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():	Inits all the member vairables.
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::Init()
{
	vec3f ParentPos = m_pParentObject->GetLocalMat()->axis_pos;

	IBaseAI::Init();
	m_FloatSphere.SetRadius( 500.0f );
	m_FloatSphere.SetCenter( ParentPos );
	m_v3MoveDir = vec3f( 1.0f, 0.0f, 0.0f );

	m_LineToCeling.SetStartPoint( ParentPos );
	m_LineToCeling.SetEndPoint( vec2f (ParentPos.x, ParentPos.y + m_FloatSphere.GetRadius() * 0.60f ) );

	m_LineToRight.SetStartPoint( m_pParentObject->GetLocalMat()->axis_pos );
	m_LineToRight.SetEndPoint( vec2f (ParentPos.x +  m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_LineToLeft.SetStartPoint( ParentPos );
	m_LineToLeft.SetEndPoint( vec2f (ParentPos.x - m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_DropGooLine.SetStartPoint( ParentPos );
	m_DropGooLine.SetEndPoint( vec2f (ParentPos.x, ParentPos.y - 2000.0f) );

	m_AltLineToLeft.SetStartPoint( ParentPos );
	m_AltLineToLeft.SetEndPoint( vec2f (ParentPos.x - 1500.0f, ParentPos.y ) );

	m_AltLineToRight.SetStartPoint( ParentPos );
	m_AltLineToRight.SetEndPoint( vec2f (ParentPos.x + 1500.0f, ParentPos.y ) );

	m_fElapsedTime = 0.0f;

	m_bMaxHeightReached = false;

	m_fDropGoosTime = 0.0f;
	m_fCornerTimer = 0.0f;
	m_fPrevDistance = 0.0f;
	m_nNumGoosDroped = 0;

	pSludge = NULL;
	pSludgesShip = NULL;
	pAnimComp = (CComponent_Animation*)m_pParentObject->GetComponent( ECOMP_ANIMATION );

	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent( ECOMP_SOUND );

	m_nCurStage = 0;

	(m_Stages[0]) = &BossAttackAI::StageOne;
	(m_Stages[1]) = &BossAttackAI::StageTwo;
	(m_Stages[2]) = &BossAttackAI::StageThree;
	m_CurrentAttack = &BossAttackAI::PrepareGooAttack;

	m_SteamCol.SetParent( m_pParentObject );
	m_SteamCol.Initialize();
	m_SteamCol.BuildSteamAABB( 4, 1 );
	m_SteamCol.SetConstant( true );
	m_SteamCol.m_pSteamEffect->m_uColorRampHandle = TextureManager::GetInstance()->LoadTexture( "./Resources/Particles/BossSteam_ColorRamp.png" );
	m_SteamCol.m_pSteamEffect->m_fRampScaleMax *= 1.2f;

	m_pCrusherSteamEffect = CParticleManager::GetInstance()->CreateEffect( "SteamPipe", m_pParentObject->GetWorldPos() + vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pCrusherSteamEffect->m_fScale /= 2.0f;
	m_pCrusherSteamEffect->m_fRampScaleMax /= 2.0f;
	m_pCrusherSteamEffect->m_fRampScaleMin /= 2.0f;

	m_fAltTimer = 0.0f;
	m_bCollideWithWall = false;
	m_nCrusherDrops = 0;
	m_vSpawnedGoos.clear();

	m_bPlayed = false;
	
	AltInfomation[0].NumAltData = 3;
	AltInfomation[0].CurAltData = 0;
	AltInfomation[0].AltData = new AltAttackData[3];

	AltInfomation[1].NumAltData = 2;
	AltInfomation[1].CurAltData = 0;
	AltInfomation[1].AltData = new AltAttackData[2];

	AltInfomation[2].NumAltData = 3;
	AltInfomation[2].CurAltData = 0;
	AltInfomation[2].AltData = new AltAttackData[3];

	// Setup the alt data for stage one
	AltInfomation[0].AltData[0].Timing.x = 1.39f;
	AltInfomation[0].AltData[0].Timing.y = 1.41f;
	AltInfomation[0].AltData[0].GooDropVels.push_back( vec2f( 150.0f, -1000.0f ) );
	AltInfomation[0].AltData[0].GooDropVels.push_back( vec2f( -150.0f, -1000.0f ) );

	AltInfomation[0].AltData[1].Timing.x = 1.89f;
	AltInfomation[0].AltData[1].Timing.y = 1.91f;
	AltInfomation[0].AltData[1].GooDropVels.push_back( vec2f( 1600.0f, -1000.0f) );

	AltInfomation[0].AltData[2].Timing.x = 2.59f;
	AltInfomation[0].AltData[2].Timing.y = 2.61f;
	AltInfomation[0].AltData[2].GooDropVels.push_back( vec2f( -1600.0f, -1000.0f) );

	// Setup the alt data for stage two
	AltInfomation[1].AltData[0].Timing.x = 1.89f;
	AltInfomation[1].AltData[0].Timing.y = 1.91f;
	AltInfomation[1].AltData[0].GooDropVels.push_back( vec2f( 600.0f, -800.0f) );
	AltInfomation[1].AltData[0].GooDropVels.push_back( vec2f( 500.0f, -800.0f) );
				  
	AltInfomation[1].AltData[1].Timing.x = 2.59f;
	AltInfomation[1].AltData[1].Timing.y = 2.61f;
	AltInfomation[1].AltData[1].GooDropVels.push_back( vec2f( -600.0f, -800.0f) );
	AltInfomation[1].AltData[1].GooDropVels.push_back( vec2f( -500.0f, -800.0f) );

	// Setup the alt data for stage three
	AltInfomation[2].AltData[0].Timing.x = 1.39f;
	AltInfomation[2].AltData[0].Timing.y = 1.41f;
	AltInfomation[2].AltData[0].GooDropVels.push_back( vec2f( 150.0f, -1000.0f ) );
	AltInfomation[2].AltData[0].GooDropVels.push_back( vec2f( -150.0f, -1000.0f ) );
				  
	AltInfomation[2].AltData[1].Timing.x = 1.89f;
	AltInfomation[2].AltData[1].Timing.y = 1.91f;
	AltInfomation[2].AltData[1].GooDropVels.push_back( vec2f( 1200.0f, -1000.0f) );
	AltInfomation[2].AltData[1].GooDropVels.push_back( vec2f( 1000.0f, -1000.0f) );
				  
	AltInfomation[2].AltData[2].Timing.x = 2.59f;
	AltInfomation[2].AltData[2].Timing.y = 2.61f;
	AltInfomation[2].AltData[2].GooDropVels.push_back( vec2f( -1200.0f, -1000.0f) );
	AltInfomation[2].AltData[2].GooDropVels.push_back( vec2f( -1000.0f, -1000.0f) );

	SteamAttackDis[0] = 200.0f;
	SteamAttackDis[1] = 140.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown(): Deletes all alocated memory	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::Shutdown()
{
	CParticleManager::GetInstance()->DestroyEffect( m_pCrusherSteamEffect );

	delete[] AltInfomation[0].AltData;
	AltInfomation[0].AltData = NULL;

	delete[] AltInfomation[1].AltData;
	AltInfomation[1].AltData = NULL;

	delete[] AltInfomation[2].AltData;
	AltInfomation[2].AltData = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update( float _fElapsedTime ): Updates the boss calling the correct attack functions	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::Update( float _fElapsedTime )
{
	if( !pSludge )
		pSludge = CObjectManager::GetInstance()->GetSludge();
	if( !pSludgesShip )
		pSludgesShip = CObjectManager::GetInstance()->GetSludgesShip();

	m_fElapsedTime = _fElapsedTime;

	if( m_fDropGoosTime > 0.0f )
	{
		m_fDropGoosTime -= m_fElapsedTime;
	}

	CleanSpawnedVector();

	m_bMaxHeightReached = false;
	m_bCollideWithWall = false;

	vec3f ParentPos = m_pParentCollcmp->GetPos();

	m_FloatSphere.SetCenter( ParentPos );

	m_LineToCeling.SetStartPoint( ParentPos );
	m_LineToCeling.SetEndPoint( vec2f( ParentPos.x, ParentPos.y + m_FloatSphere.GetRadius() * 0.60f ) );

	m_LineToRight.SetStartPoint( m_pParentObject->GetLocalMat()->axis_pos );
	m_LineToRight.SetEndPoint( vec2f( ParentPos.x +  m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_LineToLeft.SetStartPoint( ParentPos );
	m_LineToLeft.SetEndPoint( vec2f( ParentPos.x - m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_DropGooLine.SetStartPoint( ParentPos );
	m_DropGooLine.SetEndPoint( vec2f( ParentPos.x, ParentPos.y - 2000.0f ) );

	m_AltLineToLeft.SetStartPoint( ParentPos );
	m_AltLineToLeft.SetEndPoint( vec2f( ParentPos.x - 1500.0f, ParentPos.y ) );

	m_AltLineToRight.SetStartPoint( ParentPos );
	m_AltLineToRight.SetEndPoint( vec2f( ParentPos.x + 1500.0f, ParentPos.y ) );

	CheckCollisions();

	(this->*m_Stages[m_nCurStage])();

	if (m_CurrentAttack)
	{
		(this->*m_CurrentAttack)();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckCollisions(): Checks collisions with the ceiling and walls.	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::CheckCollisions()
{
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_WALL_BREAKABLE) | (1<<OBJ_DOOR);

	// call the kd tree and get the near objects
	CSceneObject _FloatSceneObj;
	_FloatSceneObj.SetCollidableObject (&m_FloatSphere);

	// call the kd tree to get the nearest objects
	CKdTree::GetNearObjects ( &_FloatSceneObj, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_FloatSceneObj.SetCollidableObject (NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		CheckCeilingCollision ( objAABB );
		CheckWallCollision ( objAABB );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrepareGooAttack(): 	Move the boss over the player and drop a goo if the player is close enough. if the player
//						is close to the wall then drop a goo every so often.
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::PrepareGooAttack()
{
	// check the goo line to see if it is ok to drop a goo.
	bool bDropGoo = CheckDropGooLine ();

	// make sure the player exists and the boss is at the max height
	IBaseObject* _pPlayer = CObjectManager::GetInstance()->GetPlayer();
	if( _pPlayer && m_bMaxHeightReached )
	{
		// follow the player and get the distance from him
		float CurDistance = FollowObject( _pPlayer->GetWorldPos () );
	
		// if the distance is less then 30 and the boss can drop a goo then drop and goo when the player passes the goo line
		if( CurDistance <= 30.0f && bDropGoo)
		{
			vec3f CPA, CPB;
			if( m_DropGooLine.LineToAABB( *((AABB*)_pPlayer->GetCollidableObject ()), CPA, CPB) )
			{
				m_pParentCollcmp->CleanRemoveVelocity();
				if( m_fDropGoosTime <= 0.0f )
				{
					m_fDropGoosTime = GOO_DROP_WAIT;
					// switch the current attack to the goo attack to drop the goo
					m_CurrentAttack = &BossAttackAI::GooAttack;
				}
			}
		}
		else
		{
			// if the distance is less then 120 and the boss is colliding a wall then try and drop a goo
			if( CurDistance <= 120.0f && m_bCollideWithWall )
			{
				// check if the player is within an aabb below the boss
				AABB Corner;
				Corner.SetMaxPoint( vec3f (m_pParentObject->GetWorldPos().x + 100.0f, m_pParentObject->GetWorldPos().y + 10.0f, 0.0f) );
				Corner.SetMinPoint( vec3f (m_pParentObject->GetWorldPos().x - 100.0f, m_DropGooLine.GetEndPoint2D().y, 0.0f) );

				vec3f CP;
				vec2f CPA;
				if( Corner.AABBToAABB( *((AABB*)_pPlayer->GetCollidableObject ()), CP, CPA))
				{
					// if the player is in the aabb drop a goo
					m_pParentCollcmp->CleanRemoveVelocity();
					if( m_fDropGoosTime <= 0.0f )
					{
						m_fDropGoosTime = GOO_DROP_WAIT;
						// switch the current attack to the goo attack to drop the goo
						m_CurrentAttack = &BossAttackAI::GooAttack;
					}
				}
			}
		}
	
		m_fPrevDistance = CurDistance;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GooAttack(): Switch animation and drop a goo. check if the number of goos has reached the max number the boss can drop
//				then switch to the alt goo attack.
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::GooAttack()
{
	// remove the velocity of the boss and change the animation.
	m_pParentCollcmp->CleanRemoveVelocity();
	if( pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipPullLeverDownState::GetState() )
	{
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgePullLeverDownState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipPullLeverDownState::GetState() );
	}

	// when the animation is done playing drop a goo down
	if( pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipPullLeverDownState::GetState() && pAnimComp->GetInterpolator().IsAnimationDone()/* && m_bDropGoo*/)
	{
		// reset the timer and set the current attack to the prepare attack
		m_fDropGoosTime = GOO_DROP_WAIT;
		m_CurrentAttack = &BossAttackAI::PrepareGooAttack;

		// increase the number of goos droped and drop a goo
		m_nNumGoosDroped++;
		DropGooAttack( vec2f( 0.0f, -1200.0f ) * m_fElapsedTime);
		m_bPlayed = false;

		// switch the animations back to the idle
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );
	}

	// if the number of goos droped is at the max that the boss can drop switch the current attack to the prepare alt
	if( m_nNumGoosDroped >= MAX_GOOS_BEFORE_ALT )
	{
		m_nNumGoosDroped = 0;
		(m_CurrentAttack) = &BossAttackAI::PrepareGooAlt;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckCollisions(): Checks collisions with the ceiling and walls.	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::PrepareGooAlt()
{
	std::vector <CSceneObject*> NearestObjectsToLeft;
	std::vector <CSceneObject*> NearestObjectsToRight;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_WALL_BREAKABLE) | (1<<OBJ_DOOR);

	// call the kd tree and get the near objects
	CSceneObject _ALtLinesSceneObj;
	_ALtLinesSceneObj.SetCollidableObject (&m_AltLineToLeft);

	// get the nearest objects from the left
	CKdTree::GetNearObjects ( &_ALtLinesSceneObj, PSFLAG_LINE, ReturnParams, NearestObjectsToLeft, ReturnBody, ReturnObject);

	// get the nearest objects from the right
	_ALtLinesSceneObj.SetCollidableObject (&m_AltLineToRight);
	CKdTree::GetNearObjects ( &_ALtLinesSceneObj, PSFLAG_LINE, ReturnParams, NearestObjectsToRight, ReturnBody, ReturnObject);

	_ALtLinesSceneObj.SetCollidableObject (NULL);
				 
	// shorten both the left and the right alt lines
	for( unsigned int objindex = 0; objindex < NearestObjectsToLeft.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjectsToLeft[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		vec3f CPA, CPB;
		if ( m_AltLineToLeft.LineToAABB (objAABB, CPA, CPB) )
		{
			m_AltLineToLeft.SetEndPoint (CPA);
		}
	}

	for( unsigned int objindex = 0; objindex < NearestObjectsToRight.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjectsToRight[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		vec3f CPA, CPB;
		if ( m_AltLineToRight.LineToAABB (objAABB, CPA, CPB) )
		{
			m_AltLineToRight.SetEndPoint (CPA);
		}
	}

	// get a vector of both the right and left lines.
	vec2f LeftAlt = vec2f (m_AltLineToLeft.GetEndPoint2D().x - m_AltLineToLeft.GetStartPoint2D().x, 0.0f);
	vec2f RightAlt = vec2f (m_AltLineToRight.GetEndPoint2D().x - m_AltLineToRight.GetStartPoint2D().x, 0.0f);

	// get the length of both lines
	float Left = LeftAlt.magnitude();
	float Right = RightAlt.magnitude();

	// if the left length is greater then the right length move the boss to the left
	// else move the boss to the right
	if (Left > Right)
	{
		m_pParentCollcmp->CleanForce ( vec2f (-1.0f, 0.0f) * 5.0f * m_fElapsedTime );
	}
	else
	{
		m_pParentCollcmp->CleanForce ( vec2f (1.0f, 0.0f) * 5.0f * m_fElapsedTime );
	}

	// if the the left length minus the right length is less then epsilon plus 10 switch to the alt attack and change the animation.
	if ( abs ( Left - Right ) <= FLT_EPSILON + 10.0f)
	{
		m_CurrentAttack = &BossAttackAI::AltGooAttack;

		if(m_pSoundCmp)
		{
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_LAUGH);
		}
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeSpinAttackState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipSpinAttackState::GetState() );
		m_pParentCollcmp->CleanRemoveVelocity ();
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AltGooAttack(): Shoot goos to each platom based on the information created in the init	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::AltGooAttack()
{
	// make sure the boss is in the spin attack
	if( pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipSpinAttackState::GetState() )
	{
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeSpinAttackState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipSpinAttackState::GetState() );
		return;
	}

	m_fAltTimer += m_fElapsedTime;

	// get the current alt data
	// if the alt timer is less then the min alt data timing and the alt timer is greater then the max alt data timing 
	int nCur = AltInfomation[m_nCurStage].CurAltData;
	if( m_fAltTimer >= (AltInfomation[m_nCurStage].AltData[nCur].Timing.x - FLT_EPSILON) && m_fAltTimer <= (AltInfomation[m_nCurStage].AltData[nCur].Timing.y + FLT_EPSILON) )
	{
		// get the size of how many goos to drop
		// and loop through all the goo velocities and drop a goo with that velocity
		unsigned int size = AltInfomation[m_nCurStage].AltData[nCur].GooDropVels.size();
		for (unsigned int DropVelIndex = 0; DropVelIndex < size; ++DropVelIndex)
		{
			DropGooAttack( AltInfomation[m_nCurStage].AltData[nCur].GooDropVels[ DropVelIndex ] * m_fElapsedTime );
	}

		// make sure the cur alt data is less then the max number of data
		AltInfomation[m_nCurStage].CurAltData++;
		if( AltInfomation[m_nCurStage].CurAltData > AltInfomation[m_nCurStage].NumAltData )
	{
			AltInfomation[m_nCurStage].CurAltData = AltInfomation[m_nCurStage].NumAltData - 1;
	}

		m_bPlayed = false;
	}

	// if the animation is done reset the infomation for the alt, change the current attack to either the prepare goo or the prepare steam
	// depending on the current stage.
	if( pAnimComp->GetInterpolator().IsAnimationDone() )
	{
		AltInfomation[m_nCurStage].CurAltData = 0;
		m_fAltTimer = 0.0f;

		if( m_nCurStage < 1 )
	{
			m_CurrentAttack = &BossAttackAI::PrepareGooAttack;
	}
		else
	{
			m_CurrentAttack = &BossAttackAI::PrepareSteamAttack;
	}

		// change the animation back to the idle
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StageOne(): checks the bosses health to see if the boss needs to switch stages.	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::StageOne()
	{
	// check the boss health
	CComponent_Health* _pParentHealth = (CComponent_Health*)m_pParentObject->GetComponent(ECOMP_HEALTH);
	if (_pParentHealth && _pParentHealth->GetHealth () <= 70.0f)
	{
		// change the animation to the idle state.
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );

		// switch to the move ai for the boss and set the fade screen to true.
		BossMoveAI* _MoveAI = (BossMoveAI*)m_pParentAIcmp->GetLoadedAI(AI_MOVE);
		_MoveAI->SetFadeScreen (true);
		m_pParentAIcmp->SwitchAI (AI_MOVE);

		// reset attack data.
		m_fAltTimer = 0.0f;
		m_nNumGoosDroped = 0;
		m_nCurStage = 1;
		RemoveGoos ();
		m_CurrentAttack = &BossAttackAI::PrepareGooAttack;
		m_bPlayed = false;
		return;
	}

	m_fDropGoosTime -= m_fElapsedTime;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrepareSteamAttack(): Move the boss to the middle of the level
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::PrepareSteamAttack ()
{
	std::vector <CSceneObject*> NearestObjectsToLeft;
	std::vector <CSceneObject*> NearestObjectsToRight;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_WALL_BREAKABLE) | (1<<OBJ_DOOR);
	
	// call the kd tree and get the near objects
	CSceneObject _ALtLinesSceneObj;
	_ALtLinesSceneObj.SetCollidableObject (&m_AltLineToLeft);
	
	// get the nearest objects for the left line
	CKdTree::GetNearObjects ( &_ALtLinesSceneObj, PSFLAG_LINE, ReturnParams, NearestObjectsToLeft, ReturnBody, ReturnObject);
	
	// get the nearest objects for the right line
	_ALtLinesSceneObj.SetCollidableObject (&m_AltLineToRight);
	CKdTree::GetNearObjects ( &_ALtLinesSceneObj, PSFLAG_LINE, ReturnParams, NearestObjectsToRight, ReturnBody, ReturnObject);
	
	_ALtLinesSceneObj.SetCollidableObject (NULL);
				 
	// loop through the nearest objects for the left and right lines
	for( unsigned int objindex = 0; objindex < NearestObjectsToLeft.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjectsToLeft[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());
	
		vec3f CPA, CPB;
		if ( m_AltLineToLeft.LineToAABB (objAABB, CPA, CPB) )
		{
			m_AltLineToLeft.SetEndPoint (CPA);
		}
	}
	
	for( unsigned int objindex = 0; objindex < NearestObjectsToRight.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjectsToRight[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());
	
		vec3f CPA, CPB;
		if ( m_AltLineToRight.LineToAABB (objAABB, CPA, CPB) )
		{
			m_AltLineToRight.SetEndPoint (CPA);
		}
	}
	
	// get a vector for the left and right lines
	vec2f LeftAlt = vec2f (m_AltLineToLeft.GetEndPoint2D().x - m_AltLineToLeft.GetStartPoint2D().x, 0.0f);
	vec2f RightAlt = vec2f (m_AltLineToRight.GetEndPoint2D().x - m_AltLineToRight.GetStartPoint2D().x, 0.0f);
	
	// get the length of both lines
	float Left = LeftAlt.magnitude();
	float Right = RightAlt.magnitude();

	// if the left length minus the right length is less then 10
	if( abs ( Left - Right ) <= FLT_EPSILON + 10.0f )
	{
		// change the animation to the steam attack
		if( pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipSteamAttackState::GetState() )
		{
		// Transition Sludge From Idle State to the Pull Lever Down State.
			((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeSteamAttackState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
			((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipSteamAttackState::GetState() );
		}

		m_pParentCollcmp->CleanRemoveVelocity ();

		// when the animation is done switch the current attack to the steam attack.
		if ( pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipSteamAttackState::GetState() && pAnimComp->GetInterpolator().IsAnimationDone() )
		{
			// Transition Sludge From Idle State to the Pull Lever Down State.
			((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
			// Transition Sludges Ship from Idle to Pull Lever Down State.
			((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );

			if(m_pSoundCmp)
			{
				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_LAUGH);
				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP);
			}

			m_CurrentAttack = &BossAttackAI::SteamAttack;

			// find out which direction to move the boss during the steam attack.
			IBaseObject* pPlayer = CObjectManager::GetInstance()->GetPlayer();
			if( pPlayer )
			{
				vec2f VecToPlayer = vec2f( pPlayer->GetWorldPos().x - m_pParentObject->GetWorldPos().x, 0.0f );
				VecToPlayer.y = 0.0f;
				float distance = VecToPlayer.magnitude();
				if (distance)
				{
					VecToPlayer /= distance;
				}
				else
				{
					VecToPlayer.x = 1.0f;
				}

				m_v3MoveDir.x = VecToPlayer.x;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SteamAttack(): Update the steam collision and move the boss to the player	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::SteamAttack ()
{
	// update the steam line and the steam collision
	Line SteamLine;
	SteamLine.SetStartPoint( m_pParentObject->GetWorldPos() );
	vec3f end = m_pParentObject->GetWorldPos() + m_v3MoveDir * 1500.0f;
	SteamLine.SetEndPoint( end );

	m_SteamCol.UpdateSteamLine( vec3f (m_pParentObject->GetWorldPos().x, m_pParentObject->GetWorldPos().y - 80.0f, 0.0f) );
 	m_SteamCol.Update (m_fElapsedTime);

	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_DOOR) | (1<<OBJ_WALL_BREAKABLE);
	
	// call the kd tree and get the near objects
	CSceneObject _SteamSceneObj;
	_SteamSceneObj.SetCollidableObject (&SteamLine);
	
	// get the nearest objects
	CKdTree::GetNearObjects ( &_SteamSceneObj, PSFLAG_LINE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);
	_SteamSceneObj.SetCollidableObject (NULL);

	vec3f StopPoint;

	// loop through all the near objects
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		vec3f CPA, CPB;
		if( SteamLine.LineToAABB (objAABB, CPA, CPB) )
		{
			SteamLine.SetEndPoint (CPA);
		}
	}

	// if the steam line length ios less then the steam attack distance
	vec2f Length = SteamLine.GetEndPoint2D() - SteamLine.GetStartPoint2D();
	if( Length.magnitude() <= SteamAttackDis[ m_nCurStage - 1 ] )
	{
		// switch animations and switch the current attack
		m_pParentCollcmp->CleanRemoveVelocity ();
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );

		if( m_nCurStage < 2 )
		{
			m_CurrentAttack = &BossAttackAI::PrepareGooAttack;
		}
		else
		{
			m_CurrentAttack = &BossAttackAI::PrepareCrusherAttack;
	}

		if(m_pSoundCmp)
		{
			m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP);
		}
		return;
	}

	// move the boss in the direction of the player
	m_pParentCollcmp->CleanForce( vec2f (m_v3MoveDir.x, 0.0f) * 5.0f * m_fElapsedTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StageTwo(): 	checks the bosses health to see if the boss needs to switch stages.	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::StageTwo()
{
	CComponent_Health* _pParentHealth = (CComponent_Health*)m_pParentObject->GetComponent(ECOMP_HEALTH);
	if (_pParentHealth && _pParentHealth->GetHealth () <= 40.0f)
	{
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleDamage::GetState() );

		BossMoveAI* _MoveAI = (BossMoveAI*)m_pParentAIcmp->GetLoadedAI(AI_MOVE);
		_MoveAI->SetFadeScreen (true);
		m_pParentAIcmp->SwitchAI (AI_MOVE);
	
		m_fAltTimer = 0.0f;
		m_nNumGoosDroped = 0;
		m_nCurStage = 2;
		
		RemoveGoos ();
		m_CurrentAttack = &BossAttackAI::PrepareGooAttack;
		m_bPlayed = false;
		return;
	}
	
	m_fDropGoosTime -= m_fElapsedTime;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrepareCrusherAttack(): 	Move the boss to player and drop the boss to preform the crusher attack
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::PrepareCrusherAttack ()
{
	// get the player and make sure the max height is reached
	IBaseObject* _pPlayer = CObjectManager::GetInstance()->GetPlayer();
	if (_pPlayer && m_bMaxHeightReached && pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipStartCrusherAttackState::GetState())
	{
		// follow the player
		float CurDistance = FollowObject( _pPlayer->GetWorldPos () );

		// if the distance from the player is less then 30 then check the goo line to start the crusher attack
		if (CurDistance <= 30.0f)
		{
			vec3f CPA, CPB;
			if(m_DropGooLine.LineToAABB ( *((AABB*)_pPlayer->GetCollidableObject ()), CPA, CPB) )
			{
				// Transition Sludge From Idle State to the Pull Lever Down State.
				((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeStartCrusherAttackState::GetState() );
				// Transition Sludges Ship from Idle to Pull Lever Down State.
				((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipStartCrusherAttackState::GetState() );

				m_pParentCollcmp->CleanRemoveVelocity();

				if(m_pSoundCmp && !m_bPlayed)
				{
					m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_LAUGH);
					m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM);
					m_bPlayed = true;
				}
			}
		}
	}

	if (pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipStartCrusherAttackState::GetState() )
	{
		float _fDT = (1.0f/ 60.0f );
		m_pCrusherSteamEffect->m_fPosition = m_pParentObject->GetWorldPos() + vec3f( 100.0f, 0.0f, 0.0f );
		m_pCrusherSteamEffect->m_fDirection = vec3f( 0.3f, -0.7f, 0.0f );
		m_pCrusherSteamEffect->Emit( _fDT );
		m_pCrusherSteamEffect->m_fPosition = m_pParentObject->GetWorldPos() - vec3f( 100.0f, 0.0f, 0.0f );
		m_pCrusherSteamEffect->m_fDirection = vec3f( -0.3f, -0.7f, 0.0f );
		m_pCrusherSteamEffect->Emit( _fDT );
	}

	// when the animation is done switch the current attack to the crusher attack.
	if (pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipStartCrusherAttackState::GetState() && pAnimComp->GetInterpolator().IsAnimationDone())
	{
		m_CurrentAttack = &BossAttackAI::CrusherAttack;

		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeFallCrusherAttackState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipFallCrusherAttackState::GetState() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CrusherAttack(): 	Move the boss down and check if the player is being crushed.
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::CrusherAttack ()
{
	// move the boss down if the animation is the falling animation.
	if (pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipFallCrusherAttackState::GetState() ||
		pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipLandCrusherAttackState::GetState())
	{
		m_pParentCollcmp->CleanForce (vec2f (0.0f, -600.0f) * m_fElapsedTime);
	}

	if (pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipLandCrusherAttackState::GetState())
	{
		std::vector <CSceneObject*> NearestObjects;
		// create a unsigned int that will tell the kd tree what you want put in the return vector
		// this uses bit wise operations so you can have more then one object returned
		// use the return flags enum from the kd tree so you know what you can get back
		unsigned int ReturnParams = 0;
		int ReturnBody = 0;
		int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PLAYER);
	
		// call the kd tree and get the near objects
		CSceneObject _UFOObject;
		_UFOObject.SetCollidableObject (&((CComponent_BossCollision*)m_pParentCollcmp)->GetUFOAABB());
	
		CKdTree::GetNearObjects ( &_UFOObject, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObject);
		_UFOObject.SetCollidableObject (NULL);

		vec3f StopPoint;

		// check all the nearest objects and kill the player if he is under the boss
		// if the boss hits the ground start moving up.
		for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
		{
			IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
			AABB objAABB = *((AABB*)_pObject->GetCollidableObject());
			AABB UFO = ((CComponent_BossCollision*)m_pParentCollcmp)->GetUFOAABB();
			UFO.SetWorldPoint( vec2f(UFO.GetWorldPoint2D().x, UFO.GetWorldPoint2D().y - 10.0f) );

			// preform crusher attack, this is the same as the actual crusher
			vec2f Direction;
			if(UFO.AABBToAABB (objAABB, Direction))
			{
				if (_pObject->GetType() == OBJ_PLAYER)
				{
					if( _pObject->GetAlive() )
					{
						CComponent_Collision* _pPlayerCollision = ((CComponent_Collision*)_pObject->GetComponent(ECOMP_COLLISION));
						float magnitude = dot_product( Direction, Direction);
						if( (_pPlayerCollision->GetCollidingGroundAlone()) && magnitude > 400.0f)
						{
							Direction.y = -Direction.y;
							// Prepare a movement vector.
							vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

							m_pParentObject->GetFrame().GetLocalMat().axis_pos += (MoveDirection);
							m_pParentObject->GetFrame().Update();

							CComponent_Health* health = (CComponent_Health*)_pObject->GetComponent (ECOMP_HEALTH);
							if (health)
							{
								health->TakeDamage (100.0f);
								_pPlayerCollision->GetVel().x = 0.0f;

								CComponent_Animation* pAnimComponent = (CComponent_Animation*)_pObject->GetComponent( ECOMP_ANIMATION );
								if( pAnimComponent )
								{
									pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeDeathByCrusherState::GetState() );
							}
							}

							// Transition Sludge From Idle State to the Pull Lever Down State.
							((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeLandCrusherAttackState::GetState() );
							// Transition Sludges Ship from Idle to Pull Lever Down State.
							((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipLandCrusherAttackState::GetState() );
						}
						else
						{
							vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);
							CComponent_PlayerCollision* pPlayerCol = (CComponent_PlayerCollision*) _pObject->GetComponent( ECOMP_COLLISION );
							if( dot_product( MoveDirection, vec3f( 0.0f, 1.0f, 0.0f)) > 0.5f)
							{
								pPlayerCol->SetCollidingGroundAlone(true);
							}
						}
					}
				}
				if (_pObject->GetType() == OBJ_WORLD_COLLISION)
				{
					m_bPlayed = false;

					if(m_pSoundCmp)
					{
						m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH);
					}

					// Transition Sludge From Idle State to the Pull Lever Down State.
					((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeLandCrusherAttackState::GetState() );
					// Transition Sludges Ship from Idle to Pull Lever Down State.
					((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipLandCrusherAttackState::GetState() );
				}
			}
		}
	}

	// when the land animation is done  change to the prepare crusher attack.
	if (pAnimComp->GetAnimStateMachine()->GetCurrentState() == SludgesShipLandCrusherAttackState::GetState() &&  pAnimComp->GetInterpolator().IsAnimationDone())
	{
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleDamage::GetState() );
		m_nCrusherDrops++;
		m_CurrentAttack = &BossAttackAI::PrepareCrusherAttack;
	}

	// when the number of crusher drops is greater then 3
	// switch to the prepare goo attack  and change animation
	if (m_nCrusherDrops >= 3)
	{
		m_nCrusherDrops = 0;
		m_CurrentAttack = &BossAttackAI::PrepareGooAttack;

		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StageThree(): 	checks the bosses health to see if the boss needs to switch stages.	
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::StageThree()
{
	// check the players health and rest the attack infomation
	CComponent_Health* _pParentHealth = (CComponent_Health*)m_pParentObject->GetComponent(ECOMP_HEALTH);
	if (_pParentHealth && _pParentHealth->GetHealth () <= 0.0f)
	{
		// Transition Sludge From Idle State to the Pull Lever Down State.
		((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
		// Transition Sludges Ship from Idle to Pull Lever Down State.
		((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleDamage::GetState() );

		BossMoveAI* _MoveAI = (BossMoveAI*)m_pParentAIcmp->GetLoadedAI(AI_MOVE);
		_MoveAI->SetDeath (true);
		m_pParentAIcmp->SwitchAI (AI_MOVE);

		if(m_pSoundCmp)
		{
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_DEATH);						
		}

		m_fAltTimer = 0.0f;
		m_nNumGoosDroped = 0;
		m_nCurStage = 2;

		RemoveGoos ();
		m_CurrentAttack = &BossAttackAI::PrepareGooAttack;
		m_bPlayed = false;
		return;
	}
	
	m_fDropGoosTime -= m_fElapsedTime;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckDropGooLine(): 	Check if the goo line is touching the ground or in a goo pit.
//
// Returns:		Bool
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool BossAttackAI::CheckDropGooLine ()
{
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);

	// call the kd tree and get the near objects
	CSceneObject _DropGooObject;
	_DropGooObject.SetCollidableObject (&m_DropGooLine);

	CKdTree::GetNearObjects ( &_DropGooObject, PSFLAG_LINE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_DropGooObject.SetCollidableObject (NULL);

	bool _bDropGoo = false;

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		// check the object the line collided with
		// and change the end of the line to that of the closest point found on that object.
		vec3f CPA, CPB;
		if ( m_DropGooLine.LineToAABB (objAABB, CPA, CPB) )
		{
			m_DropGooLine.SetEndPoint(CPA);

			if (_pObject->GetType() == OBJ_PIT)
			{
				_bDropGoo = false;
			}
			if (_pObject->GetType() == OBJ_WORLD_COLLISION)
			{
				_bDropGoo = true;
			}
		}
	}

	return _bDropGoo;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FollowObject( vec3f _v3Pos ): 	Take in the position of the object and move to the position and then return the
//									distance from the object
//
// Returns:		Float
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float BossAttackAI::FollowObject( vec3f _v3Pos )
{
	// get a vector to the object
	vec3f VecToObject = _v3Pos - m_pParentObject->GetWorldPos();
	VecToObject.y = 0.0f;
	float Distance = VecToObject.magnitude ();		

	// check if the distance from the object is greater then 5
	// then move to that position
	if (Distance > 5.0f)
	{
		VecToObject /= Distance;
		m_pParentCollcmp->CleanForce( vec2f (VecToObject.x, 0.0f) * 4.0f * m_fElapsedTime );
	}

	return Distance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveGoos(): 	Remove all goos spawned by the boss
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::RemoveGoos ()
{
	// loop through all the spawned goos and set their alive to false
	for (unsigned int Index = 0; Index < m_vSpawnedGoos.size(); ++Index)
	{
		IBaseObject* goo = m_vSpawnedGoos[Index];
		if (goo && goo->GetType () == OBJ_GOO)
		{
			goo->SetAlive (false);
		}
	}

	// clear the vector
	m_vSpawnedGoos.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CleanSpawnedVector(): 	remove all objects from the spawned vector
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::CleanSpawnedVector ()
{
	// loop through the spawned vector
	for (std::vector<IBaseObject*>::iterator itr = m_vSpawnedGoos.begin(); itr != m_vSpawnedGoos.end(); )
	{
		// if the object is null then remove it or increment the iterator.
		if (!(*itr) || (*itr)->GetType() == OBJ_NULL)
		{
			itr = m_vSpawnedGoos.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DropGooAttack( vec2f Vel ): 	Drop a goo bassed on the velocity passed in
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::DropGooAttack( vec2f Vel )
{
	// create a goo
	D3DXMATRIX pos;
	D3DXMatrixIdentity( &pos );
	D3DXMatrixTranslation( &pos, m_pParentCollcmp->GetPos().x, m_pParentCollcmp->GetPos().y - 80.0f, m_pParentCollcmp->GetPos().z );
	IBaseObject* Goo = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, pos, m_pParentObject->GetLevelID());
	if( Goo )
	{
		// Get the item component from the goo and set the ingore time
		CComponent_Item* GooItem = ((CComponent_Item*)Goo->GetComponent(ECOMP_ITEM));
		if (GooItem)
		{
			GooItem->SetSpawnIgnoreTime( 0.5f );// set the ingore timer for the little goo;
		}
		
		// get the collision component  and set the velocity to the passed in
		CComponent_LittleGooCollision* GooColcmp = ((CComponent_LittleGooCollision*)Goo->GetComponent(ECOMP_COLLISION));
		if ( GooColcmp )
		{
			GooColcmp->CleanRemoveVelocity();
			GooColcmp->CleanForce( Vel );
		}

		// set the rotation of the goo
		matrix4f _localMat = (*Goo->GetLocalMat());
		matrix4f rotationMatrix;
		rotationMatrix.make_rotation_y( D3DXToRadian(90) );
		rotationMatrix.axis_pos = _localMat.axis_pos;
		_localMat = rotationMatrix;

		Goo->SetLocalMat(&_localMat);

		if(m_pSoundCmp && !m_bPlayed)
		{
			m_bPlayed = true;
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR);
		}
	}
	m_vSpawnedGoos.push_back (Goo);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckCeilingCollision( AABB _Volume ): 	Check collision with the ceiling
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::CheckCeilingCollision ( AABB _Volume )
{
	// check if the celing line has collided with a static aabb
	vec3f CPA, CPB;

	if ( m_LineToCeling.LineToAABB ( _Volume, CPA, CPB ) )
	{
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToCeling.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_bMaxHeightReached = true;
			// move the ship over time so it doesnt seem like it just poped
			m_pParentCollcmp->CleanOffset (PushOut * 10.0f * m_fElapsedTime);
		
			// remove the ships velocity
			m_pParentCollcmp->GetVel ().y = 0.0f;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckWallCollision( AABB _Volume ):	Check collision with the walls
//
// Returns:		Void
//
// Mod. Name:   JF
// Mod. Date:   8/22/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BossAttackAI::CheckWallCollision ( AABB _Volume )
{
	// check if the celing line has collided with a static aabb
	vec3f CPA, CPB;

	if ( m_LineToRight.LineToAABB ( _Volume, CPA, CPB ) )
	{
		m_bCollideWithWall = true;
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToRight.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_pParentCollcmp->GetVel().x = 0.0f;
			m_pParentCollcmp->CleanOffset (PushOut * 10.0f * m_fElapsedTime);
		}
	}

	if ( m_LineToLeft.LineToAABB ( _Volume, CPA, CPB ) )
	{
		m_bCollideWithWall = true;
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToLeft.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_pParentCollcmp->GetVel().x = 0.0f;
			m_pParentCollcmp->CleanOffset (PushOut * 10.0f * m_fElapsedTime);
		}
	}
}