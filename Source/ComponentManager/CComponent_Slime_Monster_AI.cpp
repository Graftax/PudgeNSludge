////////////////////////////////////////////////////////////////////////////////////////
//// Filename:	CComponent_Slime_Monster_AI.cpp
//// Author:		Josh Fields
//// Date:		5/24/12
//// Purpose:		This class holds the information for the Slime Monster AI
////////////////////////////////////////////////////////////////////////////////////////
//#include "CComponent_Slime_Monster_AI.h"
//#include "../Utilities/KdTree/KdTree.h"
//#include "CComponent_Health.h"
//#include "../ObjectLib/ObjectManager/CObjectManager.h"
//#include "../AnimationLib/AnimationIncludes.h"
//#include "CComponent_Sound.h"
//
//#define PATROL_JUMP 500.0f
//#define RUN_JUMP 400.f
//#define MOVEMENT_SPEED 125.0f
//#define RUN_SPEED 500.0f
//#define ATTACK_SPEED 320.0f
//#define ATTACK_RADIUS 500.0f
//#define AGGRO_RADIUS 800.0f
//#define PERSONAL_RADIUS 200.0f
//#define ATTACK_WAIT 2.45833f
//#define PATROL_WAIT 0.5f
//#define HIDE_WAIT 5.0f
//#define RUN_WAIT 0.006f
//#define STUN_WAIT 0.5f
//#define THROW_HEIGHT 500.0f
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// CComponent_Slime_Monster_AI():	Default Constructor
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CComponent_Slime_Monster_AI::CComponent_Slime_Monster_AI ()
//{
//	//m_bStuned = false;
//	//m_bHidden = true;
//	//m_bSneakAttack = true;
//	//m_bMoveDirection = false;
//	////m_bOnGround = false;
//	//m_nPrevDirection = 0;
//	//m_bPrevDead = false;
//	//
//	//m_fAttackTimer = 0.0f;
//	//m_fJumpTimer = 0.0f;
//	//m_fHideTimer = 0.0f;
//	//
//	//m_nMaxLilGoos = 1;
//	//m_nActiveGoos = 0;
//	//
//	//m_pAnimComponent = NULL;
//	//m_pSoundCmp = nullptr;
//	//m_pCollisionCmp = nullptr;
//	//
//	//RotatedRight = RotatedLeft = false;
//}
//
//CComponent_Slime_Monster_AI::~CComponent_Slime_Monster_AI ()
//{
//	Shutdown ();
//}
//
//bool CComponent_Slime_Monster_AI::Initialize ()
//{
//	//DebugShapes::CreateSphere (
//	//m_aiType = AI_SLIME_MONSTER;
//	//m_spAggro.SetRadius (AGGRO_RADIUS);
//	//m_spAttack.SetRadius (ATTACK_RADIUS);
//	//m_spPersonalSpace.SetRadius (PERSONAL_RADIUS);
//	//
//	//m_bStuned = false;
//	//m_bHidden = true;
//	//m_bSneakAttack = true;
//	//m_bMoveDirection = false;
//	////m_bOnGround = false;
//	//m_bPrevDead = false;
//	//m_nPrevDirection = 0;
//	//
//	//m_fAttackTimer = 0.0f;
//	//m_fJumpTimer = 0.0f;
//	//m_fHideTimer = 0.0f;
//	//m_fStunTimer = 0.0f;
//	//
//	//m_nMaxLilGoos = 1;
//	//m_nActiveGoos = 0;
//	//
//	//m_pAnimComponent = NULL;
//	//
//	//RotatedRight = RotatedLeft = false;
//	//m_pSoundCmp = nullptr;
//	//m_pCollisionCmp = nullptr;
//
//	return true;
//}
//
//void CComponent_Slime_Monster_AI::Shutdown ()
//{
//	//m_aiType = AI_SLIME_MONSTER;
//	//m_spAggro.SetRadius (AGGRO_RADIUS);
//	//m_spAttack.SetRadius (ATTACK_RADIUS);
//	//m_spPersonalSpace.SetRadius (PERSONAL_RADIUS);
//	//
//	//m_bStuned = false;
//	//m_bHidden = true;
//	//m_bSneakAttack = true;
//	//m_bMoveDirection = false;
//	////m_bOnGround = false;
//	//m_nPrevDirection = 0;
//	//
//	//m_fAttackTimer = 0.0f;
//	//m_fJumpTimer = 0.0f;
//	//m_fHideTimer = 0.0f;
//	//m_fStunTimer = 0.0f;
//	//
//	//m_nMaxLilGoos = 1;
//	//m_nActiveGoos = 0;
//	//
//	//m_pAnimComponent = NULL;
//	//m_pSoundCmp = nullptr;
//	//m_pCollisionCmp = nullptr;
//	//
//	//RotatedRight = RotatedLeft = false;
//}
//
//bool CComponent_Slime_Monster_AI::CheckLineOfSight ( )
//{
//	// create a return vector to hold all the objects the kd tree returns
//	//std::vector <CSceneObject*> ReturnVector;
//	//// create a unsigned int that will tell the kd tree what you want put in the return vector
//	//// this uses bit wise operations so you can have more then one object returned
//	//// use the return flags enum from the kd tree so you know what you can get back
//	//unsigned int ReturnPrams = (1<<RFLAG_AABB);
//	//int ReturnBody = 0;
//	//int ReturnObjects = (1<<OBJ_WORLD_COLLISION);
//	//
//	//// call the kd tree and get the near objects
//	//CSceneObject AggroObject;
//	//
//	//
//	//Line PlayerLine;
//	//Line SlimeLine;
//	//
//	////PlayerLine.SetStartPoint ( vec2f (m_pTarget->GetWorldPos().x, m_pTarget->GetWorldPos().y) );
//	////PlayerLine.SetEndPoint ( vec2f (m_pTarget->GetWorldPos().x, m_pParent->GetWorldPos().y) );
//	//
//	//SlimeLine.SetStartPoint ( vec2f (m_pParent->GetWorldPos().x, m_pParent->GetWorldPos().y) );
//	////SlimeLine.SetEndPoint ( vec2f (m_pTarget->GetWorldPos().x, m_pParent->GetWorldPos().y) );
//	//
//	//
//	//AggroObject.SetCollidableObject (&SlimeLine);
//	//
//	//CKdTree::GetNearObjects (&AggroObject, PSFLAG_LINE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
//	////m_vCurNearObjects = ReturnVector;
//	////AggroObject.SetCollidableObject (NULL);
//	//
//	//vec2f CPA, CPB;
//	//
//	//SlimeLine.LineToLine (PlayerLine, CPA, CPB);
//	//
//	//float ClosestDistance = dot_product ( (CPB - CPA), (CPB - CPA) );
//	////IBaseObject* ClosestObject = m_pTarget;
//	//
//	////TODO:
//	//// Try and move this KD Tree stuff to the Collision Component
//	//
//	//// loop through all the return objects and check collision with them.
//	//for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
//	//{
//	//	// get the base object pointer
//	//	IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);
//	//
//	//	D3DXMATRIX mat;
//	//	D3DXMatrixIdentity (&mat);
//	//
//	//	//mat._41 = LineSight.GetStartPoint2D ().x;
//	//	//mat._42 = LineSight.GetStartPoint2D ().y;
//	//	//mat._43 = -500.0f;
//	//	//DebugShapes::RenderSphere ( mat );
//	//
//	//	vec3f CPA, CPB;
//	//	if ( SlimeLine.LineToAABB (*((AABB*)_pObject->GetCollidableObject()), CPA, CPB) )
//	//	{
//	//		mat._41 = CPA.x;
//	//		mat._42 = CPA.y;
//	//		mat._43 = -500.0f;
//	//		DebugShapes::RenderSphere ( mat );
//	//
//	//		mat._41 = CPB.x;
//	//		mat._42 = CPB.y;
//	//		mat._43 = -500.0f;
//	//		DebugShapes::RenderSphere ( mat );
//	//
//	//		float D = dot_product ( (CPA - CPB), (CPA - CPB) );
//	//				
//	//		if ( D <= ClosestDistance )
//	//		{
//	//			ClosestDistance = D;
//	//			// set the target to the plyer and leave
//	//			//ClosestObject = _pObject;
//	//		}
//	//	}
//	//
//	//	if ( PlayerLine.LineToAABB (*((AABB*)_pObject->GetCollidableObject()), CPA, CPB) )
//	//	{
//	//		mat._41 = CPA.x;
//	//		mat._42 = CPA.y;
//	//		mat._43 = -500.0f;
//	//		DebugShapes::RenderSphere ( mat );
//	//
//	//		mat._41 = CPB.x;
//	//		mat._42 = CPB.y;
//	//		mat._43 = -500.0f;
//	//		DebugShapes::RenderSphere ( mat );
//	//
//	//		float D = dot_product ( (CPA - CPB), (CPA - CPB) );
//	//				
//	//		if ( D <= ClosestDistance )
//	//		{
//	//			ClosestDistance = D;
//	//			// set the target to the plyer and leave
//	//			//ClosestObject = _pObject;
//	//		}
//	//	}
//	//
//	//}
//	//
//	////if ( ClosestObject == m_pTarget)
//	//{
//	//	return true;
//	//}
//
//	return false;
//}
//
//void CComponent_Slime_Monster_AI::Update ( float _fElapsedTime )
//{
//	//if (!m_pParent->GetAlive() && !m_bPrevDead)
//	//{
//	//	if( m_pAnimComponent )
//	//	{
//	//		m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterDeathState::GetState() );
//	//	}
//	//
//	//	if(m_pSoundCmp)
//	//	{
//	//		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH);
//	//		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH);
//	//	}
//	//
//	//	m_bPrevDead = true;
//	//
//	//	return;
//	//}
//	//
//	//if( !m_pAnimComponent )
//	//{
//	//	m_pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
//	//}
//	//
//	//if(!m_pSoundCmp)
//	//{
//	//	m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);
//	//}
//	//
//	//// stun the slime monster
//	//if ( m_bStuned )
//	//{
//	//	m_fStunTimer -= _fElapsedTime;
//	//	if (m_fStunTimer < 0.0f )
//	//	{
//	//		m_fStunTimer = 0.0f;
//	//		m_bStuned = false;
//	//	}
//	//	//Stun Animation goes here
//	//	if( m_pAnimComponent )
//	//	{
//	//		m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterStunState::GetState() );
//	//	}
//	//	return;
//	//}
//	//
//	//if (m_bHidden)
//	//{
//	//	if( m_pAnimComponent )
//	//			m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterIdleState::GetState() );
//	//}
//	//
//	//if(!m_pCollisionCmp)
//	//{
//	//	m_pCollisionCmp = (CComponent_SlimeMonsterCollision*)m_pParent->GetComponent( ECOMP_COLLISION );	
//	//}
//	//
//	////if (m_bOnGround)
//	////{
//	////	m_fAttackTimer += _fElapsedTime;
//	////	m_fJumpTimer += _fElapsedTime;
//	////	if (m_pCollisionCmp)
//	////		m_pCollisionCmp->GetVel().x = 0.0f;
//	////}
//	////else
//	////{
//	////	m_fJumpTimer = 0.0f;
//	////	m_fAttackTimer = 0.0f;
//	////}
//	//
//	//
//	//
//	//// get some objects from the KD Tree to find a target and to avoid the crushers
//	//
//	//// get the elapsed time
//	//m_fElapsedTime = _fElapsedTime;
//	//
//	//// move the attack and aggro spheres with the little goo
//	//m_spAggro.SetCenter ( m_pParent->GetWorldPos() );
//	//m_spAttack.SetCenter ( m_pParent->GetWorldPos() );
//	//m_spPersonalSpace.SetCenter ( m_pParent->GetWorldPos() );
//	//
//	//// create a return vector to hold all the objects the kd tree returns
//	//std::vector <CSceneObject*> ReturnVector;
//	//// create a unsigned int that will tell the kd tree what you want put in the return vector
//	//// this uses bit wise operations so you can have more then one object returned
//	//// use the return flags enum from the kd tree so you know what you can get back
//	//unsigned int ReturnPrams = (1<<RFLAG_AABB);
//	//int ReturnBody = 0;
//	//int ReturnObjects = (1<<OBJ_PLAYER);
//	//
//	//// call the kd tree and get the near objects
//	//CSceneObject AggroObject;
//	//AggroObject.SetCollidableObject (&m_spAggro);
//	//
//	//CKdTree::GetNearObjects (&AggroObject, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
//	//m_vCurNearObjects = ReturnVector;
//	//
//	//// loop through all the return objects and check collision with them.
//	//for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
//	//{
//	//	// get the base object pointer
//	//	IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);
//	//
//	//	// check if the base object is the player
//	//	if ( _pObject->GetType () == OBJ_PLAYER)
//	//	{
//	//		// check if the player is with in the aggro sphere
//	//		vec2f Useless;
//	//		if ( m_spAggro.SphereToAABB (*((AABB*)_pObject->GetCollidableObject()), Useless))
//	//		{
//	//			// set the target to the plyer and leave
//	//			//m_pTarget = _pObject;
//	//			break;
//	//		}
//	//	}
//	//}
//	//
//	//// make sure we have a target
//	////if (m_pTarget)
//	//{
//	//	// if we have a target dont try to hide
//	//	m_fHideTimer = 0.0f;
//	//
//	//	// get the targets collision volume
//	//	//CCollisionVolume* _pTargetVolume = m_pTarget->GetCollidableObject();
//	//	//if ( _pTargetVolume && _pTargetVolume->GetVolumeType () == VMT_AABB)
//	//	{
//	//		// check if the target is in the aggro sphere
//	//		vec2f Useless;
//	//		//bool _bAggro = m_spAggro.SphereToAABB (*((AABB*)_pTargetVolume), Useless);
//	//
//	//		// if the target is not in the aggro sphere lose that target and return
//	//		//if (!_bAggro)
//	//		{
//	//			//m_pTarget = NULL;
//	//			return;
//	//		}
//	//
//	//		// check if the target is in the attack sphere and if it is in the personal space sphere
//	//		//bool _bAttack = m_spAttack.SphereToAABB (*((AABB*)_pTargetVolume), Useless);
//	//		//bool _bSpace = m_spPersonalSpace.SphereToAABB (*((AABB*)_pTargetVolume), Useless);
//	//
//	//		// if the target is in the aggro sphere and not in the attack sphere and the personal space sphere
//	//		// and we are not hiding move to the target
//	//		//if ( _bAggro && !_bAttack && !m_bHidden && !_bSpace)
//	//		{
//	//			//Move (m_pTarget);
//	//		}
//	//		
//	//		// if the target is in the attack sphere and not in the personal space
//	//		//if (_bAttack && !_bSpace && !m_bSneakAttack)
//	//		{
//	//			//Attack ();
//	//		}
//	//		
//	//		// if the target is in the personal space try and move away 
//	//		// unless the slime monster are able to sneak attack
//	//		//if (_bSpace)
//	//		{
//	//			if (m_bSneakAttack)
//	//			{
//	//				SneakAttack ();
//	//				m_bSneakAttack = false;
//	//			}
//	//			m_bHidden = false;
//	//		}
//	//	}
//	//}
//	////else
//	//{
//	//	// if we are not hiding then move and update the time before we start hiding again
//	//	if ( !m_bHidden )
//	//	{
//	//		m_fHideTimer += m_fElapsedTime;
//	//
//	//		Move ();
//	//		//Avoide ( );
//	//		if (m_fHideTimer >= HIDE_WAIT)
//	//		{
//	//			m_bSneakAttack = true;
//	//			m_bHidden = true;
//	//			m_fHideTimer = 0.0f;
//	//		}
//	//	}
//	//	else
//	//	{
//	//		if( m_pAnimComponent )
//	//			m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterIdleState::GetState() );
//	//	}
//	//}
//	//
//	//m_bOnGround = false;
//	//
//	//if( RotatedRight )
//	//{
//	//
//	//	// Passed Half Space Test Set Pudge to face right.
//	//
//	//	//RotatedLeft = false;
//	//	//RotatedRight = true;
//	//	matrix4f _localMat = (*m_pParent->GetLocalMat());
//	//	matrix4f rotationMatrix;
//	//	rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
//	//	rotationMatrix.translate_post( _localMat.axis_pos );
//	//	_localMat = rotationMatrix;
//	//
//	//	m_pParent->SetLocalMat(&_localMat);
//	//	m_pParent->GetWorldMat();
//	//
//	//}
//	//else if( RotatedLeft )
//	//{
//	//	//RotatedRight = false;
//	//	//RotatedLeft = true;
//	//
//	//	// Failed Half Space Test Set Pudge to face left
//	//	matrix4f _localMat = (*m_pParent->GetLocalMat());
//	//	matrix4f rotationMatrix;
//	//	rotationMatrix.make_rotation_y( D3DXToRadian(90) );
//	//	rotationMatrix.translate_post( _localMat.axis_pos );
//	//	_localMat = rotationMatrix;
//	//
//	//	m_pParent->SetLocalMat(&_localMat);
//	//	m_pParent->GetWorldMat();
//	//}
//}
//
//void CComponent_Slime_Monster_AI::Attack ()
//{
//	// attack the target
//	// get the targets health component
//	//IComponent* _pTargetHealthCMP = m_pTarget->GetComponent (ECOMP_HEALTH);
//	//CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//
//	// if the target gets a health component
//	//if ( _pTargetHealthCMP)
//	{
//		if (m_fAttackTimer >= ATTACK_WAIT && m_nActiveGoos < m_nMaxLilGoos)
//		{
//			if( m_pAnimComponent )
//				m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterThrowAttackState::GetState() );
//
//			if(m_pSoundCmp)
//			{
//				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK);
//				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK);
//			}
//
//			// get a vector to the target 
//			//vec3f TargetPos = m_pTarget->GetWorldPos();
//			//vec3f fVelocity = (TargetPos- m_pParent->GetWorldPos());
//			
//			// get the speed we need to move at
//			///float Speed = fVelocity.magnitude();
//			// get rid of the y axis on the velocity so we get a direction on the x
//			//fVelocity.y = 0.0f;
//			m_fAttackTimer = 0.0f;
//			//pCollider->GetVel().x = 0.0f;
//
//			// TODO:
//			// Get Joint info here 
//			//////////////////////////////////////////////////////////////////////////////////////////////
//			D3DXMATRIX _ObjectMatrix;
//			D3DXMatrixIdentity( &_ObjectMatrix );
//			_ObjectMatrix = *((D3DXMATRIX*)&(*m_pParent->GetWorldMat()));
//			
//			//if (fVelocity.x < 0.0f)
//			{
//				//fVelocity.x = -1.0f;
//				_ObjectMatrix._41 += -100;
//
//				RotatedLeft = true;
//				RotatedRight = false;
//			}
//			
//			//if (fVelocity.x > 0.0f)
//			{
//				//fVelocity.x = 1.0f;
//				_ObjectMatrix._41 += 100;
//				RotatedLeft = false;
//				RotatedRight = true;
//			}
//
//			_ObjectMatrix._42 += 80.0f;
//
//			/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//			// create the little goo and throw it at the target
//			IBaseObject* _pLilGoo = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, _ObjectMatrix);
//			IBaseObject* _pLilGooTrail = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO_PARTICLE_EFFECT, _ObjectMatrix);
//			// Change the Emitter particle effect to the trail.
//			((CComponent_Emitter*)_pLilGooTrail->GetComponent(ECOMP_EMITTER))->SetType( GOO_TRAIL_FX );
//
//			((CComponent_Lil_Goo_AI*)_pLilGoo->GetComponent( ECOMP_AI ))->SetLilGooTrailEffectObject( _pLilGooTrail );
//
//			/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			IComponent* _pLilGooCollision = _pLilGoo->GetComponent(ECOMP_COLLISION);
//
//			((CComponent_Collision*)_pLilGooCollision)->GetVel() = vec3f (0.0f, 0.0f, 0.f);
//			vec3f ThowHeight = vec3f (0.0f, 1.0f, 0.0f);
//			//((CComponent_Collision*)_pLilGooCollision)->GetVel() += (fVelocity * Speed) * m_fElapsedTime;
//			((CComponent_Collision*)_pLilGooCollision)->GetVel() += (ThowHeight * 300.0f) * m_fElapsedTime;
//			((CComponent_LittleGooCollision*)_pLilGooCollision)->SetSpawnObject (m_pParent);
//
//			m_nActiveGoos++;
//		}
//	}
//}
//
//void CComponent_Slime_Monster_AI::SneakAttack ()
//{
//	// this is not finished and is being worked on
//
//
//	// attack the target
//	// get the targets health component
//	//IComponent* _pTargetHealthCMP = m_pTarget->GetComponent (ECOMP_HEALTH);
//	//CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//
//	// TODO:
//	// Check the line of sight here
//
//	if (m_fAttackTimer >= ATTACK_WAIT && m_bHidden && m_nActiveGoos < m_nMaxLilGoos )
//	{
//		m_fAttackTimer = 0.0f;
//			if(m_pSoundCmp)
//			{
//				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK);
//				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE);
//			}
//
//			m_bSneakAttack = false;
//
//			// Transition to Sneak Attack animation.
//			if( m_pAnimComponent )
//				m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterSneakAttackState::GetState() );
//
//			//vec3f TargetPos = m_pTarget->GetWorldPos();
//			//vec3f fVelocity = (TargetPos- m_pParent->GetWorldPos());
//			
//			//float Speed = fVelocity.magnitude() * 2.0f;
//			//fVelocity.y = 0.0f;
//
//
//			// Get Joint info here 
//			//////////////////////////////////////////////////////////////////////////////////////////////
//			D3DXMATRIX _ObjectMatrix;
//			D3DXMatrixIdentity( &_ObjectMatrix );
//			_ObjectMatrix = *((D3DXMATRIX*)&(*m_pParent->GetWorldMat()));
//
//			//if (fVelocity.x < 0.0f)
//			{
//				//fVelocity.x = -1.0f;
//				_ObjectMatrix._41 += -100;
//				RotatedLeft = true;
//				RotatedRight = false;
//			}
//			
//			//if (fVelocity.x > 0.0f)
//			{
//				//fVelocity.x = 1.0f;
//				_ObjectMatrix._41 += 100;
//				RotatedLeft = false;
//				RotatedRight = true;
//			}
//
//
//			_ObjectMatrix._42 += 80.0f;
//
//			/////////////////////////////////////////////////////////////////////////////////////////////////
//
//			IBaseObject* _pLilGoo = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, _ObjectMatrix);
//
//			IComponent* _pLilGooCollision = _pLilGoo->GetComponent(ECOMP_COLLISION);
//
//			((CComponent_Collision*)_pLilGooCollision)->GetVel() = vec3f (0.0f, 0.0f, 0.f);
//			vec3f ThowHeight = vec3f (0.0f, 1.0f, 0.0f);
//			//((CComponent_Collision*)_pLilGooCollision)->GetVel() += (fVelocity * Speed) * m_fElapsedTime;
//			((CComponent_Collision*)_pLilGooCollision)->GetVel() += (ThowHeight * 800.0f) * m_fElapsedTime;
//			((CComponent_LittleGooCollision*)_pLilGooCollision)->SetSpawnObject (m_pParent);
//
//			_ObjectMatrix._42 += 20.0f;
//			_pLilGoo = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, _ObjectMatrix);
//
//			_pLilGooCollision = _pLilGoo->GetComponent(ECOMP_COLLISION);
//
//			((CComponent_Collision*)_pLilGooCollision)->GetVel() = vec3f (0.0f, 0.0f, 0.f);
//			//((CComponent_Collision*)_pLilGooCollision)->GetVel() += (fVelocity * Speed) * m_fElapsedTime;
//			((CComponent_Collision*)_pLilGooCollision)->GetVel() += (ThowHeight * 100.0f) * m_fElapsedTime;
//			((CComponent_LittleGooCollision*)_pLilGooCollision)->SetSpawnObject (m_pParent);
//
//			m_nActiveGoos += 2;
//	}
//}
//
//void CComponent_Slime_Monster_AI::Move ()
//{
//	//if( m_pAnimComponent )
//	//	m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterWalkState::GetState() );
//	//// do basic movement and edge detection
//	//
//	//// make sure we have a platform to move on
//	////if ( !m_pCurPlatform )
//	////{
//	////	return;
//	////}
//	//
//	//// get the parents collision component
//	//CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//	//
//	//// create a velocity and a jump vector;
//	//vec3f fVelocity = vec3f (0.0f, 0.0f, 0.0f);
//	//vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
//	//
//	//// make sure the little goo is on the ground
//	//if ( m_bOnGround && (m_fJumpTimer >= PATROL_WAIT))
//	//{
//	//	m_fJumpTimer = 0.0f;
//	//	if(m_pSoundCmp)
//	//	{
//	//		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP);
//	//	}
//	//
//	//	// make the little goo jump
//	//	pCollider->GetVel() += Jump * m_fElapsedTime;
//	//	m_pCollisionCmp->SetJump(true);
//	//
//	//	// check into the futrue moving right and left
//	//	bool MoveRight = SimulateMoveRight ();
//	//	bool MoveLeft = SimulateMoveLeft ();
//	//
//	//	// if we are moving right and the direction we were moving is right then
//	//	// continue moving right
//	//	if ( MoveRight && m_bMoveDirection )
//	//	{
//	//		fVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
//	//		RotatedRight = true;
//	//		RotatedLeft = false;
//	//	}
//	//	else
//	//	{
//	//		// if we are not going to move right set the parents velocity on the x axis
//	//		// to 0
//	//		// and set the move direction to false so the little goo will move to the left
//	//		pCollider->GetVel().x = 0.0f;
//	//		m_bMoveDirection = false;
//	//		RotatedRight = false;
//	//		RotatedLeft = true;
//	//	}
//	//	
//	//	// if we are moving to the left and the direction we were moving is to the left then
//	//	// continue moving to the left
//	//	if ( MoveLeft && !m_bMoveDirection)
//	//	{
//	//		fVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED; 
//	//		RotatedLeft = true;
//	//		RotatedRight = false;
//	//	}
//	//	else
//	//	{
//	//		// if we are not going to move left set the parents velocity on the x axis
//	//		// to 0 and set the move direction to true so the little goo will move to the right
//	//		pCollider->GetVel().x = 0.0f;
//	//		//fVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
//	//		m_bMoveDirection = true;
//	//		RotatedRight = true;
//	//		RotatedLeft = false;
//	//	}
//	//	// update the parents velocity
//	//	pCollider->GetVel() += fVelocity * m_fElapsedTime;
//	//}
//	//
//	//if(pCollider->CollidingWall())
//	//{
//	//	m_bMoveDirection = !m_bMoveDirection;
//	//	RotatedRight = !RotatedRight;
//	//	RotatedLeft = !RotatedLeft;
//	//}
//}
//
//void CComponent_Slime_Monster_AI::Move (IBaseObject* _pTarget)
//{
//	//if( m_pAnimComponent )
//	//	m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterWalkState::GetState() );
//	//
//	//// get the parents collision component
//	////CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//	//
//	//// move towards the target
//	//
//	//// get the targets position
//	////vec3f TargetPos = m_pTarget->GetWorldPos();
//	//
//	//// make sure the little goo is on the ground
//	//if ( m_bOnGround && (m_fJumpTimer >= PATROL_WAIT))
//	//{
//	//	m_fJumpTimer = 0.0f;
//	//
//	//	if(m_pSoundCmp)
//	//	{
//	//		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP);
//	//	}
//	//
//	//	// get the direction to move
//	//	//vec3f fVelocity = (TargetPos- m_pParent->GetWorldPos());
//	//	// set the velocity to 0 so the little goo doesnt jump down
//	//	//fVelocity.y = 0.0f;
//	//	// normalize the velocity
//	//	//fVelocity.normalize();
//	//		
//	//	// check if the little goo was moving one way and is now moving another way 
//	//	// if so set the parents velocity to 0
//	//	//if ((m_nPrevDirection < 0.0f && fVelocity.x > 0.0f) || (fVelocity.x < 0.0f && m_nPrevDirection > 0.0f))
//	//	{
//	//		m_pCollisionCmp->GetVel().x = 0.0f;
//	//	}
//	//	// set the prev direction to the current velocity
//	//	//m_nPrevDirection = (int)fVelocity.x;
//	//
//	//	// update the velocity and the jump
//	//	//fVelocity *= ATTACK_SPEED;
//	//	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
//	//
//	//	// update the parents velocity
//	//	//m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime; 
//	//	m_pCollisionCmp->GetVel() += Jump * m_fElapsedTime;	
//	//	m_pCollisionCmp->SetJump(true);
//	//
//	//	if( m_nPrevDirection > 0.0f )
//	//	{
//	//		RotatedRight = true;
//	//		RotatedLeft = false;
//	//	}
//	//	else if( m_nPrevDirection < 0.0f )
//	//	{
//	//		RotatedLeft = true;
//	//		RotatedRight = false;
//	//	}
//	//}
//}
//
//void CComponent_Slime_Monster_AI::Avoide (IBaseObject* _pHazard)
//{
//	//if( m_pAnimComponent && m_pAnimComponent->GetInterpolator().IsAnimationDone())
//	//{
//	//	m_pAnimComponent->GetAnimStateMachine()->ChangeState( SlimeMonsterWalkState::GetState() );
//	//}
//	//
//	//// get the parents collision component
//	////CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//	//
//	//// move towards the target
//	//
//	//// get the targets position
//	////vec3f TargetPos = m_pTarget->GetWorldPos();
//	//
//	//// make sure the little goo is on the ground
//	//if ( m_bOnGround && (m_fJumpTimer >= RUN_WAIT) )
//	//{
//	//	m_fJumpTimer = 0.0f;
//	//	// get the direction to move
//	//	//vec3f fVelocity = (m_pParent->GetWorldPos() - TargetPos);
//	//	// set the velocity to 0 so the little goo doesnt jump down
//	//	//fVelocity.y = 0.0f;
//	//	// normalize the velocity
//	//	//fVelocity.normalize();
//	//		
//	//	// check if the little goo was moving one way and is now moving another way 
//	//	// if so set the parents velocity to 0
//	//	//if ((m_nPrevDirection < 0.0f && fVelocity.x > 0.0f) || (fVelocity.x < 0.0f && m_nPrevDirection > 0.0f))
//	//	{
//	//		m_pCollisionCmp->GetVel().x = 0.0f;
//	//	}
//	//	// set the prev direction to the current velocity
//	//	//m_nPrevDirection = (int)fVelocity.x;
//	//
//	//	// update the velocity and the jump
//	//	//fVelocity *= RUN_SPEED;
//	//	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * RUN_JUMP;
//	//
//	//	// update the parents velocity
//	//	//m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime; 
//	//	m_pCollisionCmp->GetVel() += Jump * m_fElapsedTime;			
//	//}
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// SimulateMoveRight():	Checks if the little goo will be on the same platform when he jumps and moves to the right
////
//// Returns:		bool
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool CComponent_Slime_Monster_AI::SimulateMoveRight ()
//{
//	// get the parents collision component
//	//CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//
//	// get the parents collision sphere
//	AABB NewCenter = *(AABB*)m_pParent->GetCollidableObject();
//	
//	// create the direction, gravity and jump velocitys
//	vec3f DirVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
//	vec3f Gravity = vec3f( 0.0f, -35.0f, 0.0f );
//	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
//
//	// get the parents velocity
//	vec3f NewVelocity = m_pCollisionCmp->GetVel ();
//
//	// if the parent is on the ground 
//	// update its velocity
//	//if ( m_bOnGround )
//	//{
//	//	NewVelocity += DirVelocity * m_fElapsedTime;
//	//	//NewVelocity += Jump * m_fElapsedTime;
//	//}
//
//	// loop several times and check where the little goo will be
//	vec2f Useless;
//	int Interation = 0;
//	bool ReturnBool = false;
//	while ( Interation < 100 )
//	{
//		// update the little goes velocity
//		NewVelocity += Gravity * m_fElapsedTime;
//
//		// cap that velocity
//		if ( NewVelocity.y < -10.0f )
//			NewVelocity.y = -10.0f;
//		if ( NewVelocity.x < -20.0f )
//			NewVelocity.x = -20.0f;
//		if ( NewVelocity.x > 20.0f )
//			NewVelocity.x = 20.0f;
//
//		// update the future goos position
//		NewCenter.SetWorldPoint (NewCenter.GetWorldPoint3D() + NewVelocity);
//
//		//check to see if my position is greater than the platform I'm on.
//		//if(m_pCurPlatform->GetCollidableObject()->GetVolumeType() == VMT_AABB)
//		{
//			//if(((AABB*)(m_pCurPlatform->GetCollidableObject()))->GetMaxPoint2D().x <
//				//m_pParent->GetWorldPos().x)
//			{
//				ReturnBool = false;
//				break;
//			}
//		}
//
//		// this shows where the little goo will be
//		/*D3DXMATRIX mat;
//		D3DXMatrixIdentity (&mat);
//
//		mat._41 = NewCenter.GetWorldPoint2D ().x;
//		mat._42 = NewCenter.GetWorldPoint2D ().y;
//		mat._43 = -500.0f;
//
//		DebugShapes::RenderSphere (mat);*/
//
//		// loop through the objects that were found within the aggro range
//		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size (); NearIndex++)
//		{
//			// get the back object
//			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];
//			
//			// check the sphere against that aabb.
//			if ( NewCenter.AABBToAABB ( *((AABB*)pObject->GetCollidableObject()), Useless) )
//			{
//				// if the little goos future sphere is colliding with the current platform
//				//if (pObject == m_pCurPlatform)
//				{
//					// return ture so the little goo moves to the right
//					ReturnBool = true;
//					break;
//				}
//				//if (pObject != m_pCurPlatform)
//				{
//
//				}
//			}
//		}
//
//		if(ReturnBool)
//		{
//			break;
//		}
//
//		Interation++;
//	}
//
//	// if the little goo made it through the iterations without hitting the current platform return false
//	return ReturnBool;
//}
//
//bool CComponent_Slime_Monster_AI::SimulateMoveLeft ()
//{
//	// get the parents collision component
//	//CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//
//	// get the parents collision sphere
//	AABB NewCenter = *(AABB*)m_pParent->GetCollidableObject();
//	
//	// create the direction, gravity and jump velocitys
//	vec3f DirVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
//	vec3f Gravity = vec3f( 0.0f, -35.0f, 0.0f );
//	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
//
//	// get the parents velocity
//	vec3f NewVelocity = m_pCollisionCmp->GetVel ();
//
//	// if the parent is on the ground 
//	// update its velocity
//	//if ( m_bOnGround )
//	//{
//	//	NewVelocity += DirVelocity * m_fElapsedTime;
//	//	//NewVelocity += Jump * m_fElapsedTime;
//	//}
//
//	// loop several times and check where the little goo will be
//	vec2f Useless;
//	int Interation = 0;
//	bool ReturnBool = false;
//	while ( Interation < 100 )
//	{
//		// update the little goes velocity
//		NewVelocity += Gravity * m_fElapsedTime;
//
//		if ( NewVelocity.y < -10.0f )
//			NewVelocity.y = -10.0f;
//		if ( NewVelocity.x < -20.0f )
//			NewVelocity.x = -20.0f;
//		if ( NewVelocity.x > 20.0f )
//			NewVelocity.x = 20.0f;
//
//		// update the future goos position
//		NewCenter.SetWorldPoint (NewCenter.GetWorldPoint3D() + NewVelocity);
//
//		//check to see if my position is greater than the platform I'm on.
//		///if(m_pCurPlatform->GetCollidableObject()->GetVolumeType() == VMT_AABB)
//		{
//			//if(((AABB*)(m_pCurPlatform->GetCollidableObject()))->GetMinPoint2D().x  >
//				//m_pParent->GetWorldPos().x)
//			{
//				ReturnBool = false;
//				break;
//			}
//		}
//
//		/*D3DXMATRIX mat;
//		D3DXMatrixIdentity (&mat);
//
//		mat._41 = NewCenter.GetCenter2D ().x;
//		mat._42 = NewCenter.GetCenter2D ().y;
//		mat._43 = -500.0f;
//
//		DebugShapes::RenderSphere (mat);*/
//
//		// loop through the objects that were found within the aggro range
//		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size (); NearIndex++)
//		{
//			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];
//			
//			// check the sphere against that aabb.
//			if ( NewCenter.AABBToAABB ( *((AABB*)pObject->GetCollidableObject()), Useless) )
//			{
//				// if the little goos future sphere is colliding with the current platform
//				//if (pObject == m_pCurPlatform)
//				{
//					// return ture so the little goo moves to the right
//					ReturnBool = true;
//				}
//			}
//		}
//
//		if(ReturnBool)
//		{
//			break;
//		}
//
//		Interation++;
//	}
//
//	// if the little goo made it through the iterations without hitting the current platform return false
//	return ReturnBool;
//}
//
//void CComponent_Slime_Monster_AI::Damaged ()
//{
//	//vec3f TargetPos = m_pTarget->GetWorldPos();
//	//vec3f fVelocity = (TargetPos- m_pParent->GetWorldPos());
//			
//	// Get Joint info here 
//	//////////////////////////////////////////////////////////////////////////////////////////////
//	D3DXMATRIX _ObjectMatrix;
//	D3DXMatrixIdentity( &_ObjectMatrix );
//	_ObjectMatrix = *((D3DXMATRIX*)&(*m_pParent->GetWorldMat()));
//			
//	//if (fVelocity.x < 0.0f)
//	{
//		_ObjectMatrix._41 += -150;
//	}
//			
//	//if (fVelocity.x > 0.0f)
//	{
//		_ObjectMatrix._41 += 150;
//	}
//
//	CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, *((D3DXMATRIX*)&(*m_pParent->GetWorldMat())));
//}
//
//void CComponent_Slime_Monster_AI::Damage (float _fDamageAmount)
//{
//	m_fStunTimer = STUN_WAIT;
//	m_bStuned = true;
//	Damaged ();
//	CComponent_Health* slimeHealth = (CComponent_Health*)m_pParent->GetComponent (ECOMP_HEALTH);
//	
//	slimeHealth->TakeDamage (_fDamageAmount);
//
//	if(slimeHealth->GetHealth() <= 0.0f)
//	{
//		if(m_pSoundCmp)
//		{
//			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH);
//			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH);
//		}
//	}
//}