#include "CBossMoveAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../RenderEngine/DebugShapes.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../PS/States/GamePlayState.h"
#include "../LevelManager/CLevelManager.h"

// Include All Animation Files.
#include "../AnimationLib/AnimationIncludes.h"


BossMoveAI::BossMoveAI (IBaseObject* _pParent) : IBaseAI (_pParent)
{
	//0 to 1
	//
}

BossMoveAI::~BossMoveAI()
{

}

void BossMoveAI::Init ()
{
	vec3f ParentPos = m_pParentObject->GetLocalMat()->axis_pos;

	IBaseAI::Init ();
	m_FloatSphere.SetRadius (500.0f);
	m_FloatSphere.SetCenter (ParentPos);
	m_v3MoveDir = vec3f (1.0f, 0.0f, 0.0f);

	m_LineToCeling.SetStartPoint ( ParentPos );
	m_LineToCeling.SetEndPoint ( vec2f (ParentPos.x, ParentPos.y + m_FloatSphere.GetRadius() * 0.5f ) );

	m_LineToRight.SetStartPoint ( m_pParentObject->GetLocalMat()->axis_pos );
	m_LineToRight.SetEndPoint ( vec2f (ParentPos.x +  m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_LineToLeft.SetStartPoint ( ParentPos );
	m_LineToLeft.SetEndPoint ( vec2f (ParentPos.x - m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	DebugShapes::CreateSphere (30.0f, 5, 5);
	m_fElapsedTime = 0.0f;

	m_bMaxHeightReached = true;

	pSludge = NULL;
	pSludgesShip = NULL;
	pAnimComp = (CComponent_Animation*)m_pParentCollcmp->GetParent()->GetComponent( ECOMP_ANIMATION );
	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);

	m_bInNextArea = false;
	m_fFadeTime = 1.0f;
	m_bFadeIn = false;
	m_bFadeOut = true;
	m_bDead = false;
	m_nCurArea = 0;
	m_vPlayerPos.push_back (vec3f (33121.0f, 1055.0f, 0.0f));
	m_vSludgePos.push_back (vec3f (33337.0f, 1700.0f, 0.0f));

	m_vSludgeTpPos.push_back (vec3f (34271.0f, 1700.0f, 0.0f));

	m_vPlayerPos.push_back (vec3f (34889.0f, 1117.0f, 0.0f));
	m_vSludgePos.push_back (vec3f (35302.0f, 1700.0f, 0.0f));

	m_vSludgeTpPos.push_back (vec3f (36141.0f, 1700.0f, 0.0f));

	m_vPlayerPos.push_back (vec3f (36604.0f, 1117.0f, 0.0f));
	m_vSludgePos.push_back (vec3f (36990.0f, 1700.0f, 0.0f));
}

void BossMoveAI::Shutdown ()
{

}

void BossMoveAI::Update (float _fElapsedTime)
{
	if( !pSludge )
		pSludge = CObjectManager::GetInstance()->GetSludge();
	if( !pSludgesShip )
		pSludgesShip = CObjectManager::GetInstance()->GetSludgesShip();		

	//if(m_pSoundCmp && !m_bPlayed && !m_bDead && m_bMaxHeightReached)
	//{
	//	m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP);
	//	m_bPlayed = true;
	//}

	m_fElapsedTime = _fElapsedTime;

	m_bMaxHeightReached = false;

	vec3f ParentPos = m_pParentCollcmp->GetPos();

	m_FloatSphere.SetCenter ( ParentPos );

	m_LineToCeling.SetStartPoint ( ParentPos );
	m_LineToCeling.SetEndPoint ( vec2f (ParentPos.x, ParentPos.y + m_FloatSphere.GetRadius() * 0.5f ) );

	m_LineToRight.SetStartPoint ( m_pParentObject->GetLocalMat()->axis_pos );
	m_LineToRight.SetEndPoint ( vec2f (ParentPos.x +  m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_LineToLeft.SetStartPoint ( ParentPos );
	m_LineToLeft.SetEndPoint ( vec2f (ParentPos.x - m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

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

	CKdTree::GetNearObjects ( &_FloatSceneObj, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_FloatSceneObj.SetCollidableObject (NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		ChecKCelingCollision ( objAABB );
		CheckWallCollision ( objAABB );
	}

	if( m_bDead )
	{
		//if(m_pSoundCmp)
		//{
		//	m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP);
		//}
		Death ();
		return;
	}

	if (m_bFadeScreen)
	{
		FadeScreen ();
		return;
	}

	CGame::GetInstance()->GetInputManager()->IgnoreInputForSetAmountOfTime (_fElapsedTime, 0.3f);

	MoveToNextArea ();
}

void BossMoveAI::FadeScreen()
{
	m_pParentCollcmp->CleanRemoveVelocity ();


	if (m_bFadeOut && LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().GetTimeToRender () >= 1.0f)
	{
		
		LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().SetRaiseBrightness (false);
		LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().SetTimeToRender (1.0f);
		m_bFadeOut = false;
		return;
	}

	if (m_bFadeIn && LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().GetTimeToRender () <= 0.0f)
	{
		LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().SetRaiseBrightness (true);
		LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().SetTimeToRender (0.0f);
		m_bFadeIn = false;
		return;
	}

	CGame::GetInstance()->GetInputManager()->ResetIgnoreTimer();
	if (LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().GetTimeToRender () <= 0.0f)
	{
		CGame::GetInstance()->GetInputManager()->ResetIgnoreTimer();
		m_bFadeIn = true;
		IBaseObject* _pPlayer = CObjectManager::GetInstance()->GetPlayer();
		//vec3f Pos = vec3f (33193.0f, 1055.0f, 0.0f);
		_pPlayer->SetLocalPos ( &m_vPlayerPos[m_nCurArea]);
	
		CComponent_Input* _PlayerInput = (CComponent_Input*)_pPlayer->GetComponent(ECOMP_INPUT);
		if (_PlayerInput)
		{
			//_PlayerInput->SetStopInput (true);
			_PlayerInput->ResetPulledObject ();
		}

		m_pParentObject->SetLocalPos ( &m_vSludgePos[m_nCurArea] );
		return;
	}

	if (LevelManager::GetInstance()->GetCurrentLevel ()->GetLightManager().GetTimeToRender () >= 1.0f)
	{
		CGame::GetInstance()->GetInputManager()->ResetIgnoreTimer();
		if(m_pSoundCmp)
		{
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_LAUGH);
		}
		m_bFadeOut = true;
		m_bFadeScreen = false;
		return;
	}
}

void BossMoveAI::Death()
{
	m_pParentCollcmp->CleanForce ( vec2f (0.0f, -1.0f) * 120.0f * m_fElapsedTime );

	std::vector <CSceneObject*> NearestObjects;
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT) | (1<<OBJ_PLAYER);

	// call the kd tree and get the near objects
	CSceneObject _FloatSceneObj;
	_FloatSceneObj.SetCollidableObject (&m_FloatSphere);

	CKdTree::GetNearObjects ( &_FloatSceneObj, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_FloatSceneObj.SetCollidableObject (NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		vec3f CP;
		vec2f CPA;
			
		if (objAABB.AABBToAABB ( ((CComponent_BossCollision*)m_pParentCollcmp)->GetUFOAABB(), CP, CPA) )
		{
			m_pParentCollcmp->CleanRemoveVelocity();

			CComponent_Animation* SludeAnimation = ((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ));

			if( SludeAnimation->GetAnimStateMachine()->GetCurrentState() != SludgeFlyAway::GetState() )
			{
				if(m_pSoundCmp)
				{
					m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION);
				}

				CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosion", m_pParentObject->GetWorldPos() );
				CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosionSparks", m_pParentObject->GetWorldPos() );
				CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosionSmoke", m_pParentObject->GetWorldPos() + vec3f( 0.0f, 0.0f, 80.0f )  );

				// Transition Sludge From Idle State to the Pull Lever Down State.
				SludeAnimation->GetAnimStateMachine()->ChangeState( SludgeFlyAway::GetState() );
				((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleDamage::GetState() );
			}
		}
	}

	CComponent_Animation* SludeAnimation = ((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ));
	if( SludeAnimation->GetInterpolator().IsAnimationDone() && SludeAnimation->GetAnimStateMachine()->GetCurrentState() == SludgeFlyAway::GetState() )
	{
		CGamePlayState::GetInstance()->WinGameFade();
		CObjectManager::GetInstance()->AddObjectToRemove(m_pParentObject);
	}
}

void BossMoveAI::MoveToNextArea ()
{
	if( pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipIdleState::GetState() || pAnimComp->GetAnimStateMachine()->GetCurrentState() != SludgesShipIdleDamage::GetState())
	{
		CComponent_Health* health = (CComponent_Health*)m_pParentObject->GetComponent(ECOMP_HEALTH);
		if (health && health->GetHealth() > 50.0f)
		{
			// Transition Sludge From Idle State to the Pull Lever Down State.
			((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
			// Transition Sludges Ship from Idle to Pull Lever Down State.
			((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );
		}
		else
		{
			if (health && health->GetHealth() <= 50.0f)
			{
				// Transition Sludge From Idle State to the Pull Lever Down State.
				((CComponent_Animation*)pSludge->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
				// Transition Sludges Ship from Idle to Pull Lever Down State.
				((CComponent_Animation*)pSludgesShip->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleDamage::GetState() );
			}
		}
	}

	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_WALL_BREAKABLE);

	// call the kd tree and get the near objects
	CSceneObject _FloatSceneObj;
	_FloatSceneObj.SetCollidableObject (&m_FloatSphere);

	CKdTree::GetNearObjects ( &_FloatSceneObj, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_FloatSceneObj.SetCollidableObject (NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		if (_pObject->GetType () == OBJ_WALL_BREAKABLE)
		{
			vec2f CPA;
			vec3f CP;
			if (_pObject->GetAlive() && objAABB.AABBToAABB ( ((CComponent_BossCollision*)m_pParentCollcmp)->GetUFOAABB (), CP, CPA) )
			{
				CParticleManager::GetInstance()->CreateEffectOneShot( "BreakableWall", _pObject->GetWorldPos() );

				D3DXMATRIX pos;
				D3DXMatrixIdentity( &pos );
				D3DXMatrixTranslation( &pos, _pObject->GetWorldPos().x, _pObject->GetWorldPos().y, _pObject->GetWorldPos().z );

				unsigned int chunkAmount = rand() % 2;
				chunkAmount++;

				for( unsigned int i = 0; i < chunkAmount; ++i )
				{
					(( CComponent_Collision*)( CObjectManager::GetInstance()->CreateObject( OBJ_WALLCHUNK, pos, m_pParentObject->GetLevelID() )->GetComponent( ECOMP_COLLISION )))->CleanForce( vec2f( -m_pParentCollcmp->GetVel().x, m_pParentCollcmp->GetVel().y ) * 0.5f );
				}
				_pObject->SetAlive( false );
				CComponent_BreakableWallCollision* WallCol = (CComponent_BreakableWallCollision*)_pObject->GetComponent (ECOMP_COLLISION);
				CObjectManager::GetInstance()->AddObjectToRemove (_pObject);

				for (unsigned int Index = 0; Index < WallCol->GetAssociatedObjects ().size(); Index++)
				{
					D3DXMatrixIdentity( &pos );
					D3DXMatrixTranslation( &pos, WallCol->GetAssociatedObjects ()[Index]->GetWorldPos().x, WallCol->GetAssociatedObjects ()[Index]->GetWorldPos().y, WallCol->GetAssociatedObjects ()[Index]->GetWorldPos().z );

					unsigned int chunkAmount = rand() % 2;
					chunkAmount++;

					for( unsigned int i = 0; i < chunkAmount; ++i )
					{
						(( CComponent_Collision*)( CObjectManager::GetInstance()->CreateObject( OBJ_WALLCHUNK, pos, m_pParentObject->GetLevelID() )->GetComponent( ECOMP_COLLISION )))->CleanForce( vec2f( -m_pParentCollcmp->GetVel().x, m_pParentCollcmp->GetVel().y ) * 0.5f );
					}
					WallCol->GetAssociatedObjects ()[Index]->SetAlive( false );

					CParticleManager::GetInstance()->CreateEffectOneShot( "BreakableWall", WallCol->GetAssociatedObjects ()[Index]->GetWorldPos() );
					CObjectManager::GetInstance()->AddObjectToRemove (WallCol->GetAssociatedObjects ()[Index]);
				}
			}
			continue;
		}

	/*	ChecKCelingCollision ( objAABB );
		CheckWallCollision ( objAABB );*/
	}
	
	if (m_bMaxHeightReached)
	{
		m_pParentCollcmp->CleanForce ( vec2f (1.0f, 0.0f) * 13.0f * m_fElapsedTime );
	}

	CGame::GetInstance()->GetInputManager()->ResetIgnoreTimer();

	if ( m_pParentObject->GetWorldPos ().x >= m_vSludgeTpPos[m_nCurArea].x/*34271.0f*/ )
	{
		m_nCurArea++;
		//vec3f Pos = vec3f ( 35102.0f, 1500.0f, 0.0f);
		m_pParentObject->SetLocalPos ( &m_vSludgePos[m_nCurArea] );
		m_pParentCollcmp->CleanRemoveVelocity();

		m_pParentAIcmp->SwitchAI (AI_IDLE);

		IBaseObject* _pPlayer = CObjectManager::GetInstance()->GetPlayer();
		CComponent_Input* _PlayerInput = (CComponent_Input*)_pPlayer->GetComponent(ECOMP_INPUT);
		//if (_PlayerInput)
		//{
		//	_PlayerInput->SetStopInput (false);
		//}
		
	}
}

void BossMoveAI::ChecKCelingCollision ( AABB _Volume )
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
			m_pParentCollcmp->CleanOffset (PushOut * m_fElapsedTime);
		
			// remove the ships velocity
			m_pParentCollcmp->GetVel ().y = 0.0f;
		}
	}
}

void BossMoveAI::CheckWallCollision ( AABB _Volume )
{
	// check if the celing line has collided with a static aabb
	vec3f CPA, CPB;

	if ( m_LineToRight.LineToAABB ( _Volume, CPA, CPB ) )
	{
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToRight.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_pParentCollcmp->CleanOffset (PushOut * m_fElapsedTime);
		}
	}

	if ( m_LineToLeft.LineToAABB ( _Volume, CPA, CPB ) )
	{
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToLeft.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{

			m_pParentCollcmp->CleanOffset (PushOut * m_fElapsedTime);
		}
	}
}
