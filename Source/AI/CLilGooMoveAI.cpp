//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CComponent_Lil_Goo_AI.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds the information for the little goo AI
//////////////////////////////////////////////////////////////////////////////////////
#include "CLilGooMoveAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../ComponentManager/CComponent_Health.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../PS/CGame.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../RenderEngine/DebugShapes.h"
#include "../ComponentManager/CComponent_Item.h"

#define PATROL_JUMP 700.0f
#define MOVEMENT_SPEED 200.0f
#define PATROL_TIME 0.75f
#define DIRCHANGE_WAIT 0.4f
#define Y_CHECK (GAME_UNIT + 61)
#define NEEDED_MOVE_AMOUNT 1.0f

CLilGooMoveAI::CLilGooMoveAI(IBaseObject* _pParent) : IBaseAI (_pParent)
{
	m_fJumpTimer = 0.0f;
	m_pSoundCmp = nullptr;
	m_bMoveDirection = true;
	m_nPrevDirection = 0;
	m_fJumpTimer = 0.0f;
	m_pAnimComponent = nullptr;
	m_pItemCmp = nullptr;
	m_plilGooTrailEffect = NULL;
	m_fChangeDirTimer = 0.0f;
	m_fAmountMoved = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
}

CLilGooMoveAI::~CLilGooMoveAI ()
{
	Shutdown();
}

void CLilGooMoveAI::Init ()
{
	IBaseAI::Init ();
	m_bMoveDirection = false;
	m_fJumpTimer = 0.0f;
	m_pSoundCmp = nullptr;
	m_pAnimComponent = nullptr;
	m_pItemCmp = nullptr;
	m_plilGooTrailEffect = NULL;
	m_fChangeDirTimer = 0.0f;
	m_fAmountMoved = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);

	m_pParentCollcmp = (CComponent_LittleGooCollision*)m_pParentObject->GetComponent( ECOMP_COLLISION );
	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
	m_pItemCmp = (CComponent_Item*)m_pParentObject->GetComponent(ECOMP_ITEM);

#if _ANIMATIONS
	m_pAnimComponent = (CComponent_Animation*)m_pParentObject->GetComponent( ECOMP_ANIMATION );
#endif
}

void CLilGooMoveAI::Shutdown ()
{
	m_fJumpTimer = 0.0f;
	m_pSoundCmp = nullptr;
	m_bMoveDirection = true;
	m_nPrevDirection = 0;
	m_fJumpTimer = 0.0f;
	m_pAnimComponent = nullptr;
	m_pItemCmp = nullptr;
	m_plilGooTrailEffect = NULL;
	m_fChangeDirTimer = 0.0f;
	m_fAmountMoved = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
	IBaseAI::Shutdown();
}

void CLilGooMoveAI::Update (float _fElapsedTime)
{
	m_bAlive = m_pParentObject->GetAlive();

	if(!m_bAlive)
	{
		m_pParentAIcmp->SwitchAI(AI_DEAD);
		return;
	}

	// get the elapsed time
	m_fElapsedTime = _fElapsedTime;

	m_fJumpTimer += _fElapsedTime;
	if(m_fChangeDirTimer <= DIRCHANGE_WAIT)
	{
		m_fChangeDirTimer += m_fElapsedTime;
	}

	if(m_fJumpTimer > PATROL_TIME && m_pParentCollcmp->CollidingGround())
	{
		//this will check to see if the lil goo hasn't moved, and if it hasn't then we should change directions
		if(m_pParentCollcmp)
		{
			m_fAmountMoved = dot_product((m_tPrevPos - m_pParentCollcmp->GetPhysicsBox()->GetCentroid()), (m_tPrevPos - m_pParentCollcmp->GetPhysicsBox()->GetCentroid()));
			m_tPrevPos = m_pParentCollcmp->GetPhysicsBox()->GetCentroid();
		}

		if(m_fAmountMoved < NEEDED_MOVE_AMOUNT)
		{
			//we didn't move enought so let's turn around
			m_bMoveDirection = !m_bMoveDirection;
		}

		Move();
		m_fJumpTimer = 0.0f;
	}

	if((m_pParentCollcmp->CollidingGround() && !m_pParentCollcmp->GetCollidingGroundLast()) || (m_pParentCollcmp->GetInGoo() && !m_pParentCollcmp->GetCollidingGroundLast()))
	{
		float y = m_pParentCollcmp->GetVel().y;
		m_pParentCollcmp->CleanRemoveVelocity();
		m_pParentCollcmp->CleanForce(vec2f(0.0f, y));
		m_fJumpTimer = 0.0f;
	}

	if(m_bMoveDirection)
	{
		//rotate me to face right
		matrix4f _localMat = (*m_pParentObject->GetLocalMat());
		matrix4f rotationMatrix;
		rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
		rotationMatrix.axis_pos = _localMat.axis_pos;
		_localMat = rotationMatrix;

		m_pParentObject->SetLocalMat(&_localMat);
	}
	else
	{
		matrix4f _localMat = (*m_pParentObject->GetLocalMat());
		matrix4f rotationMatrix;
		rotationMatrix.make_rotation_y( D3DXToRadian(90) );
		rotationMatrix.axis_pos = _localMat.axis_pos;
		_localMat = rotationMatrix;

		m_pParentObject->SetLocalMat(&_localMat);
	}
}

void CLilGooMoveAI::Move ()
{
	// check if the collision component is valid
	if(!m_pParentCollcmp)
	{
		return; 
	}

	// create a velocity and a jump vector;
	vec2f fVelocity = vec2f(0.0f, 0.0f);
	vec2f Jump = (vec2f (0.0f, 1.0f)) * PATROL_JUMP;

	float y = m_pParentCollcmp->GetVel().y;
	m_pParentCollcmp->CleanRemoveVelocity();
	m_pParentCollcmp->CleanForce(vec2f(0.0f, y));
		
#if _ANIMATIONS
	if(m_pAnimComponent)
	{
		m_pAnimComponent->GetAnimStateMachine()->ChangeState(LilGooWalkState::GetState());
		m_pAnimComponent->GetInterpolator().SetTime(0.0f);
	}
#endif

	if(m_bMoveDirection)
	{
		if(SimulateMoveRight())
		{
			fVelocity = vec2f(1.0f, 0.0f) * MOVEMENT_SPEED;
			
			if(m_pSoundCmp)
			{
				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP);
			}
		}
		else
		{
			//I just failed moving to the right. Let's see if we can go the other way.
			SetMoveDirection(false);
		}
	}
	else
	{
		//we must have been moving left.
		if(SimulateMoveLeft())
		{
			fVelocity = vec2f(-1.0f, 0.0f) * MOVEMENT_SPEED;
			
			if(m_pSoundCmp)
			{
				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP);
			}
		}
		else
		{
			//I just failed moving to the left. Let's see if we can go the other way.
			SetMoveDirection(true);
		}
	}

	// update the parents velocity
	m_pParentCollcmp->CleanForce((Jump * m_fElapsedTime) + (fVelocity * m_fElapsedTime));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SimulateMoveRight():	Checks if the little goo will be on the same platform when he jumps and moves to the right
//
// Returns:		bool
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CLilGooMoveAI::SimulateMoveRight ()
{
	if(!m_pParentCollcmp)
	{
		return false; 
	}

	//creating the jumping sphere
	Sphere jumpRadius;
	jumpRadius.SetRadius(300.0f);
	jumpRadius.SetCenter(m_pParentCollcmp->GetPhysicsBox()->GetCentroid());
	CSceneObject jumpSphere;
	jumpSphere.SetCollidableObject(&jumpRadius);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);
	
	// call the kd tree and get the near objects
	CKdTree::GetNearObjects(&jumpSphere, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
	m_vCurNearObjects = ReturnVector;

	CPhysicsEnvironment* envi = &CObjectManager::GetInstance()->m_PhysicsEnvironment;
	envi->SetGravity(vec3f(0.0f, -20.0f, 0.0f));
	tPhyObject* physObj = envi->AddPhyObject(m_pParentCollcmp->GetPos(), 1.0f);
	Box NewCenter = *((Box*)(m_pParentObject->GetCollidableObject()));

	// create the direction, gravity and jump velocitys
	vec3f DirVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
	vec3f Gravity = vec3f( 0.0f, -20.0f, 0.0f );
	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
	
	// get the parents velocity
	vec3f NewVelocity = m_pParentCollcmp->GetVel();
	
	// if the parent is on the ground 
	// update its velocity
	if(m_pParentCollcmp->CollidingGround())
	{
		NewVelocity += DirVelocity * m_fElapsedTime;
		NewVelocity += Jump * m_fElapsedTime;
	}

	physObj->ApplyUniformForce(NewVelocity, 1.0f);

	bool ReturnBool = true;
	bool bOnGround = false;
	int nIteration = 0;
	while(nIteration < 70)
	{
		envi->SimulateObject(physObj, (1.0f / 60.0f));
		NewCenter.SetWorld(physObj->GetCentroid());

		//D3DXMATRIX debug;
		//D3DXMatrixIdentity(&debug);
		//debug._41 = NewCenter.GetWorld3D().x;
		//debug._42 = NewCenter.GetWorld3D().y;
		//debug._43 = NewCenter.GetWorld3D().z;
		//DebugShapes::RenderSphere(debug);


		if(NewCenter.GetWorld3D().y < m_pParentObject->GetWorldPos().y - Y_CHECK && !bOnGround)
		{
			ReturnBool = false;
			break;
		}
	
		// loop through the objects that were found within the aggro range
		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size(); NearIndex++)
		{
			// get the back object
			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];

			vec2f CP;
			vec2f Direction;
			
			if(NewCenter.BoxToAABBWorld(*((AABB*)pObject->GetCollidableObject()), CP, Direction))
			{
				if(pObject->GetType() == OBJ_WORLD_COLLISION)
				{
					bOnGround = true;
				}
				else if(pObject->GetType() == OBJ_PIT)
				{
					ReturnBool = false;
				}
			}
		}

		if(ReturnBool == false)
		{
			break;
		}
	
		if(bOnGround)
		{
			break;
		}

		++nIteration;
	}

	if( (nIteration >= 200) && (bOnGround == false) )
		ReturnBool = false;

	return ReturnBool;
}

bool CLilGooMoveAI::SimulateMoveLeft ()
{
	// check if the collision component is valid
	if(!m_pParentCollcmp)
	{
		return false; 
	}

	//creating the jumping sphere
	Sphere jumpRadius;
	jumpRadius.SetRadius(300.0f);
	jumpRadius.SetCenter(m_pParentCollcmp->GetPhysicsBox()->GetCentroid());
	CSceneObject jumpSphere;
	jumpSphere.SetCollidableObject(&jumpRadius);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);
	
	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (&jumpSphere, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
	m_vCurNearObjects = ReturnVector;

	CPhysicsEnvironment* envi = &CObjectManager::GetInstance()->m_PhysicsEnvironment;
	envi->SetGravity(vec3f(0.0f, -20.0f, 0.0f));
	tPhyObject* physObj = envi->AddPhyObject(m_pParentCollcmp->GetPos(), 1.0f);
	Box NewCenter = *((Box*)(m_pParentObject->GetCollidableObject()));

	// create the direction, gravity and jump velocitys
	vec3f DirVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
	vec3f Gravity = vec3f( 0.0f, -20.0f, 0.0f );
	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;
	
	// get the parents velocity
	vec3f NewVelocity = m_pParentCollcmp->GetVel();
	
	// if the parent is on the ground 
	// update its velocity
	if(m_pParentCollcmp->CollidingGround())
	{
		NewVelocity += DirVelocity * m_fElapsedTime;
		NewVelocity += Jump * m_fElapsedTime;
	}

	physObj->ApplyUniformForce(NewVelocity, 1.0f);

	bool ReturnBool = true;
	bool bOnGround = false;
	int nIteration = 0;
	while(nIteration < 70)
	{
		envi->SimulateObject(physObj, (1.0f / 60.0f));
		NewCenter.SetWorld(physObj->GetCentroid());

		//D3DXMATRIX debug;
		//D3DXMatrixIdentity(&debug);
		//debug._41 = NewCenter.GetWorld3D().x;
		//debug._42 = NewCenter.GetWorld3D().y;
		//debug._43 = NewCenter.GetWorld3D().z;
		//DebugShapes::RenderSphere(debug);

		if(NewCenter.GetWorld3D().y < m_pParentObject->GetWorldPos().y - Y_CHECK && !bOnGround)
		{
			ReturnBool = false;
			break;
		}
	
		// loop through the objects that were found within the aggro range
		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size(); NearIndex++)
		{
			// get the back object
			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];

			vec2f CP;
			vec2f Direction;
			// check the sphere against that aabb.
			if(NewCenter.BoxToAABBWorld(*((AABB*)pObject->GetCollidableObject()), CP, Direction))
			{
				if(pObject->GetType() == OBJ_WORLD_COLLISION)
				{
					bOnGround = true;
				}
				else if(pObject->GetType() == OBJ_PIT)
				{
					ReturnBool = false;
				}
			}
		}
	
		if(ReturnBool == false)
		{
			break;
		}
	
		if(bOnGround)
		{
			break;
		}

		++nIteration;
	}

	if( (nIteration >= 200) && (bOnGround == false) )
		ReturnBool = false;

	return ReturnBool;
}

void CLilGooMoveAI::SetMoveDirection(bool _bMoveDir)
{ 
	if(m_fChangeDirTimer > DIRCHANGE_WAIT)
	{
		m_fChangeDirTimer = 0.0f;
		m_bMoveDirection = _bMoveDir;
	} 
}