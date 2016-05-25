#include "CSlimeMonsterHideAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../AnimationLib/AnimationIncludes.h"

#define LOS_BUBBLE 700.0f
#define LOOK_TIME 0.25f

CSlimeMonsterHideAI::CSlimeMonsterHideAI(IBaseObject* _pParent) : IBaseAI(_pParent)
{
	m_fElapsedTime = 0.0f;
	m_fLookTimer = 0.0f;
	
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
}

CSlimeMonsterHideAI::~CSlimeMonsterHideAI(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():		This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterHideAI::Init(void)
{
	IBaseAI::Init ();

	m_fElapsedTime = 0.0f;
	m_fLookTimer = 0.0f;

#if _ANIMATIONS
	m_pAnimComponent = (CComponent_Animation*)m_pParentObject->GetComponent( ECOMP_ANIMATION );
#endif

	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
	m_pItemCmp = (CComponent_Item*)m_pParentObject->GetComponent(ECOMP_ITEM);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	This function updates the actions that are expected to be observed in this state.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterHideAI::Update(float _fDT)
{
	m_fElapsedTime = _fDT;

	//added a timer just to make it so we aren't going into CheckLoS() every frame
	if(m_fLookTimer < LOOK_TIME)
	{
#if _ANIMATIONS
		if(m_pAnimComponent)
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState(SlimeMonsterHideState::GetState());
		}
#endif //_ANIMATIONS

		m_fLookTimer += _fDT;
	}
	else
	{
		if(CheckLoS())
		{
			// There's a player! Get'im!
			m_pParentAIcmp->SwitchAI(AI_ATTACK);
		}

		m_fLookTimer = 0.0f;
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
bool CSlimeMonsterHideAI::CheckLoS(void)
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

	//in our first pass of the nearest objects we didn't see the player so let's get out of here
	if(!bPlayerInRange)
	{
		return false;
	}

	CSceneObject soLineSceneObject;
	Line LineToPlayer;
	LineToPlayer.SetVolumeType(VMT_LINE);
	LineToPlayer.SetStartPoint(vec3f(m_pParentObject->GetWorldPos().x, m_pParentObject->GetWorldPos().y + 50.0f, m_pParentObject->GetWorldPos().z));
	LineToPlayer.SetEndPoint(vec3f(tPlayerPos.x, tPlayerPos.y + 50.0f, tPlayerPos.z));
	soLineSceneObject.SetCollidableObject(&LineToPlayer);

	CKdTree::GetNearObjects(&soLineSceneObject, PSFLAG_LINE, ReturnParams, ReturnVector, ReturnBody, ReturnObjects); 

	soLineSceneObject.SetCollidableObject(nullptr);

	//loop through all the return objects and check collision with them.
	IBaseObject* closestObj = nullptr;
	float fDist = FLT_MAX;
	vec3f cp;

	for(unsigned int i = 0; i < ReturnVector.size(); ++i)
	{
		IBaseObject* pObject = ((IBaseObject*)ReturnVector[i]);
		
		if(pObject->GetType() == OBJ_WORLD_COLLISION)
		{
			vec3f Intersection = vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
			if(LineToPlayer.LineToAABB(*((AABB*)pObject->GetCollidableObject()), Intersection))
			{
				//D3DXMATRIX mat;
				//D3DXMatrixIdentity(&mat);
				//mat._41 = Intersection.x;
				//mat._42 = Intersection.y;
				//mat._43 = -500;
				//DebugShapes::RenderSphere(mat);
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

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	This will clean up any memory that has been created, and set all the pointers back to null.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterHideAI::Shutdown(void)
{
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	IBaseAI::Shutdown();
}