#include "CSlimeMonsterIdleAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../AnimationLib/AnimationIncludes.h"

#define IDLE_TIME 1.0f
#define LOS_BUBBLE 700.0f

CSlimeMonsterIdleAI::CSlimeMonsterIdleAI(IBaseObject* _pParent) : IBaseAI(_pParent)
{
	m_fElapsedTime = 0.0f;
	m_fIdleTimer = 0.0f;
	m_pAnimComponent = nullptr;
	m_pItemCmp = nullptr;
	m_bIdleSound = false;
}

CSlimeMonsterIdleAI::~CSlimeMonsterIdleAI(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():	This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
//
// Returns:		void
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterIdleAI::Init(void)
{
	m_fElapsedTime = 0.0f;
	m_fIdleTimer = 0.0f;
	m_bIdleSound = false;
	m_pItemCmp = (CComponent_Item*)m_pParentObject->GetComponent(ECOMP_ITEM);

#if _ANIMATIONS
	m_pAnimComponent = (CComponent_Animation*)m_pParentObject->GetComponent(ECOMP_ANIMATION);
#endif

	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	This function updates the actions that are expected to be observed in this state.
//
// Returns:		void
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterIdleAI::Update(float _fDT)
{
	if(m_fIdleTimer < IDLE_TIME)
	{
		//we stay idle this long to space out attacks
		m_fIdleTimer += _fDT;
#if _ANIMATIONS
		if(m_pAnimComponent)
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState(SlimeMonsterIdleState::GetState());
		}
#endif
	}
	else
	{
		if(CheckLoS())
		{
			//if the player is in line of sight of the slime monster attack
			m_pParentAIcmp->SwitchAI(AI_ATTACK);
			m_bIdleSound = false;
		}
		else
		{
			//can't see the player. time to hide.
			m_pParentAIcmp->SwitchAI(AI_HIDE);
			m_bIdleSound = false;
		}

		m_fIdleTimer = 0.0f;
	}

	if(!m_bIdleSound && m_fIdleTimer >= IDLE_TIME*0.5f)
	{
		if(m_pSoundCmp)
		{
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE);
		}
		m_bIdleSound = true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckLoS():	This function looks at the player and sees if the player is in range and in line of sight.
//
// Returns:		bool = true if the player is in line of sight
//
// Mod. Name: Josh Morgan
// Mod. Date:8/14/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSlimeMonsterIdleAI::CheckLoS(void)
{
	//creating the sound sphere
	Sphere LoSRadius;
	LoSRadius.SetRadius(LOS_BUBBLE);
	LoSRadius.SetCenter(m_pParentObject->GetWorldPos());
	CSceneObject LoSSphere;
	LoSSphere.SetCollidableObject(&LoSRadius);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = 1<<OBJ_PLAYER | 1<<OBJ_WORLD_COLLISION;

	CKdTree::GetNearObjects(&LoSSphere, PSFLAG_SPHERE, ReturnParams, ReturnVector, ReturnBody, ReturnObjects);

	//bool for checking if there's the player, and the position of the player
	bool bPlayerInRange = false;
	vec3f tPlayerPos = vec3f(0.0f, 0.0f, 0.0f);

	for(unsigned int i = 0; i < ReturnVector.size(); i++)
	{
		IBaseObject* pObject = ((IBaseObject*)ReturnVector[i]);

		if(pObject->GetType() == OBJ_PLAYER)
		{
			bPlayerInRange = true;
			tPlayerPos = pObject->GetWorldPos();
			break;
		}
	}

	if(!bPlayerInRange)
	{
		return false;
	}

	//we make a line to the player since he's in aggro range
	CSceneObject soLineSceneObject;
	Line LineToPlayer;
	LineToPlayer.SetVolumeType(VMT_LINE);
	LineToPlayer.SetStartPoint(vec3f(m_pParentObject->GetWorldPos().x, m_pParentObject->GetWorldPos().y + 100.0f, m_pParentObject->GetWorldPos().z));
	LineToPlayer.SetEndPoint(vec3f(tPlayerPos.x, tPlayerPos.y + 100.0f, tPlayerPos.z));
	soLineSceneObject.SetCollidableObject(&LineToPlayer);

	CKdTree::GetNearObjects(&soLineSceneObject, PSFLAG_LINE, ReturnParams, ReturnVector, ReturnBody, ReturnObjects); 

	soLineSceneObject.SetCollidableObject(nullptr);

	//loop through all the return objects again and check collision with them.
	for(unsigned int i = 0; i < ReturnVector.size(); ++i)
	{
		IBaseObject* pObject = ((IBaseObject*)ReturnVector[i]);
		
		if(pObject->GetType() == OBJ_WORLD_COLLISION)
		{
			//check to see if our line to the player is obstructed by this ocject
			vec3f Intersection = vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
			if(LineToPlayer.LineToAABB(*((AABB*)pObject->GetCollidableObject()), Intersection))
			{
				//D3DXMATRIX mat;
				//D3DXMatrixIdentity(&mat);
				//mat._41 = Intersection.x;
				//mat._42 = Intersection.y;
				//mat._43 = -500;
				//DebugShapes::RenderSphere(mat);

				//we see that there's something between us so I don't have line of sight
				return false;
			}
		}
	}

	//set the slime monster to face the player
	matrix4f _localMat = (*m_pParentObject->GetLocalMat());
	matrix4f rotationMatrix;
	vec2f DtoP = LineToPlayer.GetEndPoint2D() - LineToPlayer.GetStartPoint2D();
	if(DtoP.x <= 0.0f)
	{
		//spawn facing left
		rotationMatrix.make_rotation_y( D3DXToRadian(90) );
	}
	else
	{
		//spawn to face right
		rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
	}
	rotationMatrix.axis_pos = _localMat.axis_pos;
	_localMat = rotationMatrix;
	m_pParentObject->SetLocalMat(&_localMat);
	//I SEE HIM! HE'S RIGHT THERE!
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown(): This will clean up any memory that has been created, and set all the pointers back to null.
//
// Returns:		void
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterIdleAI::Shutdown(void)
{
	m_fElapsedTime = 0.0f;
	m_fIdleTimer = 0.0f;
	m_pAnimComponent = nullptr;
	m_pItemCmp = nullptr;
}