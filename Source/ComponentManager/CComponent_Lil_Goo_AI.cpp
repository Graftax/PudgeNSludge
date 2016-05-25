////////////////////////////////////////////////////////////////////////////////////////
//// Filename:	CComponent_Lil_Goo_AI.cpp
//// Author:		Josh Fields
//// Date:		5/24/12
//// Purpose:		This class holds the information for the little goo AI
////////////////////////////////////////////////////////////////////////////////////////
//#include "CComponent_Lil_Goo_AI.h"
//#include "../Utilities/KdTree/KdTree.h"
//#include "CComponent_Health.h"
//#include "../ObjectLib/ObjectManager/CObjectManager.h"
//#include "../PS/CGame.h"
//#include "../AnimationLib/AnimationIncludes.h"
//#include "../PhysicsEnvironment/PhysicsEnvironment.h"
//#include "../RenderEngine/DebugShapes.h"
//#include "CComponent_Item.h"
////#include "Component_Emitter.h"
//
////#define ATTACK_JUMP 300.0f
////#define SUCIDE_JUMP 400.0f
//#define PATROL_JUMP 700.0f
//#define MOVEMENT_SPEED 200.0f
//#define ATTACK_SPEED 140.0f
//#define SUCIDE_SPEED 500.0f
//#define ATTACK_RADIUS 160.0f
//#define AGGRO_RADIUS 350.0f
//#define ATTACK_TIME 0.5f
//#define PATROL_TIME 0.75f
//#define DIRCHANGE_WAIT 1.0f
//#define Y_CHECK (GAME_UNIT + 61)
//#define NEEDED_MOVE_AMOUNT 1.0f
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// CComponent_Lil_Goo_AI():	Default Constructor
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CComponent_Lil_Goo_AI::CComponent_Lil_Goo_AI ()
//{
//	m_pCollisionCmp = nullptr;
//	m_fJumpTimer = 0.0f;
//	m_pSoundCmp = nullptr;
//	//m_bDead = false;
//	m_bAlive = false;
//	m_bMoveDirection = true;
//	m_nPrevDirection = 0;
//	m_fJumpTimer = 0.0f;
//	m_fSucideTimer = 0.0f;
//	//m_bInGoo = false;
//	m_bJumpAttack = false;
//	m_bAttacking = false;
//	m_bSucideing = false;
//	m_pAnimComponent = nullptr;
//	m_pItemCmp = nullptr;
//	m_nState = 0;
//	m_plilGooTrailEffect = NULL;
//	m_fChangeDirTimer = 0.0f;
//	m_fAmountMoved = 0.0f;
//	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// CComponent_Lil_Goo_AI():	Default Destructor
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CComponent_Lil_Goo_AI::~CComponent_Lil_Goo_AI ()
//{
//	Shutdown();
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Initialize():	Inits all the information for the little goo AI
////
//// Returns:		bool
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool CComponent_Lil_Goo_AI::Initialize ()
//{
//	CComponent_AI::Initialize ();
//	m_aiType = AI_LIL_GOO;
//	m_bMoveDirection = false;
//	m_fJumpTimer = 0.0f;
//	m_fSucideTimer = 0.0f;
//	//m_bInGoo = false;
//	m_bJumpAttack = false;
//	m_bAttacking = false;
//	m_bSucideing = false;
//	m_pSoundCmp = nullptr;
//	m_pAnimComponent = nullptr;
//	m_pItemCmp = nullptr;
//	//m_bDead = false;
//	m_plilGooTrailEffect = NULL;
//	m_bAlive = false;
//	m_nState = 0;
//	m_fChangeDirTimer = 0.0f;
//	m_fAmountMoved = 0.0f;
//	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
//
//	return true;
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Shutdown():	Shutdowns the little goo AI
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CComponent_Lil_Goo_AI::Shutdown ()
//{
//
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Update():	Updates the little goo AI
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CComponent_Lil_Goo_AI::Update ( float _fElapsedTime )
//{
//	m_bAlive = m_pParent->GetAlive();
//
//	if(!m_bAlive)
//	{
//		m_nState = DEAD;
//	}
//
//	if(!m_pCollisionCmp)
//	{
//		m_pCollisionCmp = (CComponent_LittleGooCollision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
//	}
//
//#if _ANIMATIONS
//	if( !m_pAnimComponent )
//	{
//		m_pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
//	}
//#endif
//
//	if( !m_bActive )
//		return;
//
//	if(!m_pSoundCmp)
//	{
//		m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);
//	}
//
//	if(!m_pItemCmp)
//	{
//		m_pItemCmp = (CComponent_Item*)m_pParent->GetComponent(ECOMP_ITEM);
//	}
//
//	// get the elapsed time
//	m_fElapsedTime = _fElapsedTime;
//
//	switch(m_nState)
//	{
//	case IDLE:
//		{
//			if(m_pItemCmp->GetSpawnIgnoreTime() < 0.0f && m_pCollisionCmp->GetCollidingGroundAlone())
//			{
//				if(SimulateMoveLeft() || SimulateMoveRight())
//				{
//					m_nState = MOVE;
//				}
//			}
//		}
//		break;
//	case MOVE:
//		{
//			m_fJumpTimer += _fElapsedTime;
//			if(m_fChangeDirTimer <= DIRCHANGE_WAIT)
//			{
//				m_fChangeDirTimer += m_fElapsedTime;
//			}
//
//			if(m_fJumpTimer > PATROL_TIME && m_pCollisionCmp->CollidingGround())
//			{
//				//this will check to see if the lil goo hasn't moved, and if it hasn't then we should change directions
//				if(m_pCollisionCmp)
//				{
//					m_fAmountMoved = dot_product((m_tPrevPos - m_pCollisionCmp->GetPhysicsBox()->GetCentroid()), (m_tPrevPos - m_pCollisionCmp->GetPhysicsBox()->GetCentroid()));
//					m_tPrevPos = m_pCollisionCmp->GetPhysicsBox()->GetCentroid();
//				}
//
//				if(m_fAmountMoved < NEEDED_MOVE_AMOUNT)
//				{
//					//we didn't move enought so let's turn around
//					m_bMoveDirection = !m_bMoveDirection;
//				}
//
//				Move();
//				m_fJumpTimer = 0.0f;
//			}
//		}
//		break;
//	case DEAD:
//		{
//			m_pCollisionCmp->CleanRemoveVelocity();
//
//			CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooExplosion", m_pParent->GetWorldPos() );
//			CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooSpittle", m_pParent->GetWorldPos() );
//
//			if( ((CComponent_Item*)(GetParent()->GetComponent(ECOMP_ITEM)))->GetTarget() )
//			{
//				((CComponent_Input*)(CObjectManager::GetInstance()->GetPlayer()->GetComponent( ECOMP_INPUT )))->SetObjectPulled( nullptr );
//			}
//
//			CObjectManager::GetInstance()->AddObjectToRemove(m_pParent);
//
//			return;
//		}
//		break;
//	default:
//		{
//			m_nState = IDLE;
//		}
//		break;
//	}
//	if((m_pCollisionCmp->CollidingGround() && !m_pCollisionCmp->GetCollidingGroundLast()) || (m_pCollisionCmp->GetInGoo() && !m_pCollisionCmp->GetCollidingGroundLast()))
//	{
//		//m_pCollisionCmp->GetVel().x = 0.0f;
//		float y = m_pCollisionCmp->GetVel().y;
//		m_pCollisionCmp->CleanRemoveVelocity();
//		m_pCollisionCmp->CleanForce(vec2f(0.0f, y));
//		m_fJumpTimer = 0.0f;
//	}
//
//	if(m_bMoveDirection)
//	{
//		//rotate me to face right
//		matrix4f _localMat = (*m_pParent->GetLocalMat());
//		matrix4f rotationMatrix;
//		rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
//		rotationMatrix.axis_pos = _localMat.axis_pos;
//		_localMat = rotationMatrix;
//
//		m_pParent->SetLocalMat(&_localMat);
//	}
//	else
//	{
//		matrix4f _localMat = (*m_pParent->GetLocalMat());
//		matrix4f rotationMatrix;
//		rotationMatrix.make_rotation_y( D3DXToRadian(90) );
//		rotationMatrix.axis_pos = _localMat.axis_pos;
//		_localMat = rotationMatrix;
//
//		m_pParent->SetLocalMat(&_localMat);
//	}
//}
//
//void CComponent_Lil_Goo_AI::Attack ()
//{
//}
//
//// this moves around the level
//void CComponent_Lil_Goo_AI::Move ()
//{
//	// check if the collision component is valid
//	if(!m_pCollisionCmp)
//	{
//		return; 
//	}
//
//	// create a velocity and a jump vector;
//	vec2f fVelocity = vec2f(0.0f, 0.0f);
//	vec2f Jump = (vec2f (0.0f, 1.0f)) * PATROL_JUMP;
//
//	//m_pCollisionCmp->GetVel().x = 0.0f;
//	float y = m_pCollisionCmp->GetVel().y;
//	m_pCollisionCmp->CleanRemoveVelocity();
//	m_pCollisionCmp->CleanForce(vec2f(0.0f, y));
//		
//#if _ANIMATIONS
//	if(m_pAnimComponent)
//	{
//		m_pAnimComponent->GetAnimStateMachine()->ChangeState(LilGooWalkState::GetState());
//		m_pAnimComponent->GetInterpolator().SetTime(0.0f);
//	}
//#endif
//
//	if(m_bMoveDirection)
//	{
//		if(SimulateMoveRight())
//		{
//			fVelocity = vec2f(1.0f, 0.0f) * MOVEMENT_SPEED;
//			
//			if(m_pSoundCmp)
//			{
//				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP);
//			}
//		}
//		else
//		{
//			//I just failed moving to the right. Let's see if we can go the other way.
//			SetMoveDirection(false);
//		}
//	}
//	else
//	{
//		//we must have been moving left.
//		if(SimulateMoveLeft())
//		{
//			fVelocity = vec2f(-1.0f, 0.0f) * MOVEMENT_SPEED;
//			
//			if(m_pSoundCmp)
//			{
//				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP);
//			}
//		}
//		else
//		{
//			//I just failed moving to the left. Let's see if we can go the other way.
//			SetMoveDirection(true);
//		}
//	}
//
//	// update the parents velocity
//	//m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime;
//	m_pCollisionCmp->CleanForce((Jump * m_fElapsedTime) + (fVelocity * m_fElapsedTime));
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Move():	Takes in a IBaseObject pointer (_pTarget). this will move the little goo toward the target passed in
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CComponent_Lil_Goo_AI::Move (IBaseObject* _pTarget)
//{
////	// check if the collision component is valid
////	if(!m_pCollisionCmp)
////	{
////		return; 
////	}
////
////	// move towards the target
////
////	// get the targets position
////	vec3f TargetPos = m_pTarget->GetWorldPos();
////
////	// make sure the little goo is on the ground
////	if(m_fJumpTimer > ATTACK_TIME && m_pCollisionCmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
////	{
////#if _ANIMATIONS
////		if( m_pAnimComponent )
////		{
////			m_pAnimComponent->GetAnimStateMachine()->ChangeState( LilGooWalkState::GetState() );
////		}
////#endif
////
////		// get the direction to move
////		vec3f fVelocity = (TargetPos- m_pParent->GetWorldPos());
////		// set the velocity to 0 so the little goo doesnt jump down
////		fVelocity.y = 0.0f;
////		// normalize the velocity
////		fVelocity.normalize();
////			
////		if(fVelocity.x < -0.5f)
////		{
////			m_bMoveDirection = false;
////		}
////		
////		if(fVelocity.x > 0.5f)
////		{
////			m_bMoveDirection = true;
////		}
////			
////		// check if the little goo was moving one way and is now moving another way 
////		// if so set the parents velocity to 0
////		if ((m_nPrevDirection < 0.0f && fVelocity.x > 0.0f) || (fVelocity.x < 0.0f && m_nPrevDirection > 0.0f))
////		{
////			m_pCollisionCmp->GetVel().x = 0.0f;
////		}
////		// set the prev direction to the current velocity
////		m_nPrevDirection = (int)fVelocity.x;
////
////		// update the velocity and the jump
////		fVelocity *= ATTACK_SPEED;
////		vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * ATTACK_JUMP;
////
////		// update the parents velocity
////		m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime; 
////		m_pCollisionCmp->GetVel() += Jump * m_fElapsedTime;	
////
////		if(m_pSoundCmp && m_bInRange)
////		{
////			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP);
////		}
////	}
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Avoide():	the little goo should try and avoid crushers
////
//// Returns:		Void
////
//// Mod. Name: JF
//// Mod. Date: 6/2/12
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CComponent_Lil_Goo_AI::Avoide (IBaseObject* _pHazard)
//{
//	// avoid crushers
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
//bool CComponent_Lil_Goo_AI::SimulateMoveRight ()
//{
//	// check if the collision component is valid
//	if(!m_pCollisionCmp)
//	{
//		return false; 
//	}
//
//	//creating the jumping sphere
//	Sphere jumpRadius;
//	jumpRadius.SetRadius(300.0f);
//	jumpRadius.SetCenter(m_pCollisionCmp->GetPhysicsBox()->GetCentroid());
//	CSceneObject jumpSphere;
//	jumpSphere.SetCollidableObject(&jumpRadius);
//
//	// create a return vector to hold all the objects the kd tree returns
//	std::vector <CSceneObject*> ReturnVector;
//	// create a unsigned int that will tell the kd tree what you want put in the return vector
//	// this uses bit wise operations so you can have more then one object returned
//	// use the return flags enum from the kd tree so you know what you can get back
//	int ReturnPrams = 0;
//	int ReturnBody = 0;
//	int ReturnObjects = /*(1<<OBJ_PLAYER) | */(1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);
//	
//	// call the kd tree and get the near objects
//	CKdTree::GetNearObjects(&jumpSphere, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
//	m_vCurNearObjects = ReturnVector;
//
//	//tPhyObject physObj;
//	//physObj = *m_pCollisionCmp->GetPhysicsBox();
//	CPhysicsEnvironment* envi = &CObjectManager::GetInstance()->m_PhysicsEnvironment;
//	envi->SetGravity(vec3f(0.0f, -20.0f, 0.0f));
//	tPhyObject* physObj = envi->AddPhyObject(m_pCollisionCmp->GetPos(), 1.0f);
//	Box NewCenter = *((Box*)(m_pParent->GetCollidableObject()));
//
//	// create the direction, gravity and jump velocitys
//	vec3f DirVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
//	vec3f Gravity = vec3f( 0.0f, -20.0f, 0.0f );
//	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
//	
//	// get the parents velocity
//	vec3f NewVelocity = m_pCollisionCmp->GetVel();
//	
//	// if the parent is on the ground 
//	// update its velocity
//	if(m_pCollisionCmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
//	{
//		NewVelocity += DirVelocity * m_fElapsedTime;
//		NewVelocity += Jump * m_fElapsedTime;
//	}
//
//	physObj->ApplyUniformForce(NewVelocity, 1.0f);
//
//	bool ReturnBool = true;
//	bool bOnGround = false;
//	int nIteration = 0;
//	while(nIteration < 70)
//	{
//		envi->SimulateObject(physObj, (1.0f / 60.0f));
//		NewCenter.SetWorld(physObj->GetCentroid());
//
//		//D3DXMATRIX debug;
//		//D3DXMatrixIdentity(&debug);
//		//debug._41 = NewCenter.GetWorld3D().x;
//		//debug._42 = NewCenter.GetWorld3D().y;
//		//debug._43 = NewCenter.GetWorld3D().z;
//		//DebugShapes::RenderSphere(debug);
//
//
//		if(NewCenter.GetWorld3D().y < m_pParent->GetWorldPos().y - Y_CHECK && !bOnGround)
//		{
//			ReturnBool = false;
//			break;
//		}
//	
//		// loop through the objects that were found within the aggro range
//		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size(); NearIndex++)
//		{
//			// get the back object
//			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];
//
//			vec2f CP;
//			vec2f Direction;
//			
//			if(NewCenter.BoxToAABBWorld(*((AABB*)pObject->GetCollidableObject()), CP, Direction))
//			{
//				if(pObject->GetType() == OBJ_WORLD_COLLISION)
//				{
//					bOnGround = true;
//				}
//				else if(pObject->GetType() == OBJ_PIT)
//				{
//					ReturnBool = false;
//				}
//			}
//		}
//
//		if(ReturnBool == false)
//		{
//			break;
//		}
//	
//		if(bOnGround)
//		{
//			break;
//		}
//
//		++nIteration;
//	}
//
//	if( (nIteration >= 200) && (bOnGround == false) )
//		ReturnBool = false;
//
//
//	//envi.RemovePhyObject(&physObj);
//
//	return ReturnBool;
//}
//
//bool CComponent_Lil_Goo_AI::SimulateMoveLeft ()
//{
//	// check if the collision component is valid
//	if(!m_pCollisionCmp)
//	{
//		return false; 
//	}
//
//	//creating the jumping sphere
//	Sphere jumpRadius;
//	jumpRadius.SetRadius(300.0f);
//	jumpRadius.SetCenter(m_pCollisionCmp->GetPhysicsBox()->GetCentroid());
//	CSceneObject jumpSphere;
//	jumpSphere.SetCollidableObject(&jumpRadius);
//
//	// create a return vector to hold all the objects the kd tree returns
//	std::vector <CSceneObject*> ReturnVector;
//	// create a unsigned int that will tell the kd tree what you want put in the return vector
//	// this uses bit wise operations so you can have more then one object returned
//	// use the return flags enum from the kd tree so you know what you can get back
//	int ReturnPrams = 0;
//	int ReturnBody = 0;
//	int ReturnObjects = /*(1<<OBJ_PLAYER) | */(1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);
//	
//	// call the kd tree and get the near objects
//	CKdTree::GetNearObjects (&jumpSphere, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
//	m_vCurNearObjects = ReturnVector;
//
//	
//	//physObj = *m_pCollisionCmp->GetPhysicsBox();
//	CPhysicsEnvironment* envi = &CObjectManager::GetInstance()->m_PhysicsEnvironment;
//	envi->SetGravity(vec3f(0.0f, -20.0f, 0.0f));
//	tPhyObject* physObj = envi->AddPhyObject(m_pCollisionCmp->GetPos(), 1.0f);
//	Box NewCenter = *((Box*)(m_pParent->GetCollidableObject()));
//
//	// create the direction, gravity and jump velocitys
//	vec3f DirVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
//	vec3f Gravity = vec3f( 0.0f, -20.0f, 0.0f );
//	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
//	
//	// get the parents velocity
//	vec3f NewVelocity = m_pCollisionCmp->GetVel();
//	
//	// if the parent is on the ground 
//	// update its velocity
//	if(m_pCollisionCmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
//	{
//		NewVelocity += DirVelocity * m_fElapsedTime;
//		NewVelocity += Jump * m_fElapsedTime;
//	}
//
//	physObj->ApplyUniformForce(NewVelocity, 1.0f);
//
//	bool ReturnBool = true;
//	bool bOnGround = false;
//	int nIteration = 0;
//	while(nIteration < 70)
//	{
//		envi->SimulateObject(physObj, (1.0f / 60.0f));
//		NewCenter.SetWorld(physObj->GetCentroid());
//
//		//D3DXMATRIX debug;
//		//D3DXMatrixIdentity(&debug);
//		//debug._41 = NewCenter.GetWorld3D().x;
//		//debug._42 = NewCenter.GetWorld3D().y;
//		//debug._43 = NewCenter.GetWorld3D().z;
//		//DebugShapes::RenderSphere(debug);
//
//		if(NewCenter.GetWorld3D().y < m_pParent->GetWorldPos().y - Y_CHECK && !bOnGround)
//		{
//			ReturnBool = false;
//			break;
//		}
//	
//		// loop through the objects that were found within the aggro range
//		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size(); NearIndex++)
//		{
//			// get the back object
//			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];
//
//			vec2f CP;
//			vec2f Direction;
//			// check the sphere against that aabb.
//			if(NewCenter.BoxToAABBWorld(*((AABB*)pObject->GetCollidableObject()), CP, Direction))
//			{
//				if(pObject->GetType() == OBJ_WORLD_COLLISION)
//				{
//					bOnGround = true;
//				}
//				else if(pObject->GetType() == OBJ_PIT)
//				{
//					ReturnBool = false;
//				}
//			}
//		}
//	
//		if(ReturnBool == false)
//		{
//			break;
//		}
//	
//		if(bOnGround)
//		{
//			break;
//		}
//
//		++nIteration;
//	}
//
//	if( (nIteration >= 200) && (bOnGround == false) )
//		ReturnBool = false;
//
//	//envi.RemovePhyObject(&physObj);
//
//	return ReturnBool;
//}
//
//void CComponent_Lil_Goo_AI::SetMoveDirection(bool _bMoveDir)
//{ 
//	if(m_fChangeDirTimer > DIRCHANGE_WAIT)
//	{
//		m_fChangeDirTimer = 0.0f;
//		m_bMoveDirection = _bMoveDir;
//	} 
//}