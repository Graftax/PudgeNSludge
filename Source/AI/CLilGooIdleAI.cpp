#include "CLilGooIdleAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

#define PATROL_JUMP 700.0f
#define MOVEMENT_SPEED 200.0f
#define Y_CHECK (GAME_UNIT + 61)

CLilGooIdleAI::CLilGooIdleAI(IBaseObject* _pParent) : IBaseAI(_pParent)
{
	m_fElapsedTime = 0.0f;
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
}

CLilGooIdleAI::~CLilGooIdleAI(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():	This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLilGooIdleAI::Init(void)
{
	IBaseAI::Init();

	m_fElapsedTime = 0.0f;

	//get all the components that we need
	m_pParentCollcmp = (CComponent_LittleGooCollision*)m_pParentObject->GetComponent( ECOMP_COLLISION );
	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
	m_pItemCmp = (CComponent_Item*)m_pParentObject->GetComponent(ECOMP_ITEM);

#if _ANIMATIONS
	m_pAnimComponent = (CComponent_Animation*)m_pParentObject->GetComponent( ECOMP_ANIMATION );
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	This function updates the actions that are expected to be observed in this state.
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLilGooIdleAI::Update(float _fDT)
{
	m_fElapsedTime = _fDT;

	m_bAlive = m_pParentObject->GetAlive();

	if(!m_bAlive)
	{
		//bleh! I'm dead!
		m_pParentAIcmp->SwitchAI(AI_DEAD);
		return;
	}

	if(m_pItemCmp)
	{
		//if I'm ready to jump
		if(m_pItemCmp->GetSpawnIgnoreTime() < 0.0f && m_pParentCollcmp->GetCollidingGroundAlone())
		{
			//check which direction we're going to jump first
			if(SimulateMoveLeft())
			{
				//make me face left
				matrix4f _localMat = (*m_pParentObject->GetLocalMat());
				matrix4f rotationMatrix;
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				m_pParentObject->SetLocalMat(&_localMat);

				//now let's move
				m_pParentAIcmp->SwitchAI(AI_MOVE);
			}
			else if(SimulateMoveRight())
			{
				//make me face right
				matrix4f _localMat = (*m_pParentObject->GetLocalMat());
				matrix4f rotationMatrix;
				rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				m_pParentObject->SetLocalMat(&_localMat);

				//now let's move
				m_pParentAIcmp->SwitchAI(AI_MOVE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SimulateMoveRight():	Checks if the little goo will be on the same platform when he jumps and moves to the right
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CLilGooIdleAI::SimulateMoveRight(void)
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
	int ReturnObjects = /*(1<<OBJ_PLAYER) | */(1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);
	
	// call the kd tree and get the near objects
	CKdTree::GetNearObjects(&jumpSphere, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
	m_vCurNearObjects = ReturnVector;

	//tPhyObject physObj;
	//physObj = *m_pParentCollcmp->GetPhysicsBox();
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
	if(m_pParentCollcmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
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


	//envi.RemovePhyObject(&physObj);

	return ReturnBool;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SimulateMoveLeft():	Checks if the little goo will be on the same platform when he jumps and moves to the left
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CLilGooIdleAI::SimulateMoveLeft(void)
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
	int ReturnObjects = /*(1<<OBJ_PLAYER) | */(1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT);
	
	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (&jumpSphere, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
	m_vCurNearObjects = ReturnVector;

	
	//physObj = *m_pParentCollcmp->GetPhysicsBox();
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
	if(m_pParentCollcmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
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

	//envi.RemovePhyObject(&physObj);

	return ReturnBool;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown(): This will clean up any memory that has been created, and set all the pointers back to null.
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLilGooIdleAI::Shutdown(void)
{
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	IBaseAI::Shutdown();
}