#include "CSlimeMonsterAttackAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../AnimationLib/AnimationIncludes.h"

#define LOS_BUBBLE 700.0f
#define ARCH_FORCE 1300.0f

CSlimeMonsterAttackAI::CSlimeMonsterAttackAI(IBaseObject* _pParent) : IBaseAI(_pParent)
{
	m_fElapsedTime = 0.0f;
	
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	m_bChucked = false;
}

CSlimeMonsterAttackAI::~CSlimeMonsterAttackAI(void)
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
void CSlimeMonsterAttackAI::Init(void)
{
	IBaseAI::Init ();

	m_fElapsedTime = 0.0f;
	m_tPlayerPos = vec3f(0.0f, 0.0f, 0.0f);

	m_bChucked = false;

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
void CSlimeMonsterAttackAI::Update(float _fDT)
{
	m_fElapsedTime = _fDT;

	if(!m_bChucked)
	{
		//I see the player! ATTACK!
		Chuck();
		m_bChucked = true;
	}

	if(m_pAnimComponent && m_pAnimComponent->GetInterpolator().IsAnimationDone())
	{
		//Whew! I'm tired. Time to Idle
		m_pParentAIcmp->SwitchAI(AI_IDLE);
		m_bChucked = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Chuck():		This function spawns a volatile lil goo and sends it towards the player.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterAttackAI::Chuck(void)
{
	//play attack sound and animations
	if(m_pSoundCmp)
	{
		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK);
		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK);
	}

	
#if _ANIMATIONS
	if(m_pAnimComponent)
	{
		m_pAnimComponent->GetAnimStateMachine()->ChangeState(SlimeMonsterThrowAttackState::GetState());
	}
#endif //_ANIMATIONS

	m_tPlayerPos = CObjectManager::GetInstance()->GetPlayer()->GetWorldPos();

	//spawn lil goo and send him in the direction of the player
	D3DXMATRIX pos;
	D3DXMatrixIdentity(&pos);
	D3DXMatrixTranslation(&pos, m_pParentObject->GetWorldMat()->axis_pos.x, m_pParentObject->GetWorldMat()->axis_pos.y + 50.0f, m_pParentObject->GetWorldMat()->axis_pos.z);
	IBaseObject* Goo = CObjectManager::GetInstance()->CreateObject(OBJ_GOO, pos, m_pParentObject->GetLevelID());
	if(Goo)
	{
		CComponent_Item* GooItem = ((CComponent_Item*)Goo->GetComponent(ECOMP_ITEM));
		if (GooItem)
		{
			GooItem->SetSpawnIgnoreTime(0.5f);// set the ingore timer for the little goo;
		}

#if _ANIMATIONS
		CComponent_Animation* gooAnim = (CComponent_Animation*)Goo->GetComponent(ECOMP_ANIMATION);
		if(gooAnim)
		{
			gooAnim->GetAnimStateMachine()->ChangeState(LilGooFlyingState::GetState());
		}
#endif //_ANIMATIONS

		CComponent_LittleGooCollision* GooCol = ((CComponent_LittleGooCollision*)Goo->GetComponent(ECOMP_COLLISION));
		if (GooCol)
		{
			GooCol->SetSpawnObject(m_pParentObject);
			GooCol->SetPlayerIgnoreTimer(5.0f);
			GooCol->CleanRemoveVelocity();
			//set him to volatile right out of the gate
			CComponent_AI* GooAIComp = (CComponent_AI*)Goo->GetComponent( ECOMP_AI );
			if(GooAIComp)
			{
				GooAIComp->SetActive(true);
			}
			GooCol->GetVolitile() = true;
			GooCol->SetPulled(false);
			//now  send the gooflying at the player
			float throwOffset = dot_product(m_tPlayerPos - m_pParentObject->GetWorldPos(), m_tPlayerPos - m_pParentObject->GetWorldPos());
			throwOffset = sqrt(throwOffset);
			vec2f throwDirection = vec2f(m_tPlayerPos.x - m_pParentObject->GetWorldPos().x, (m_tPlayerPos.y + (throwOffset/2.5f)) - (m_pParentObject->GetWorldPos().y + 50.0f));
			throwDirection.normalize();
			throwDirection = (throwDirection * ARCH_FORCE) * m_fElapsedTime;
			GooCol->CleanForce(throwDirection);
			
			matrix4f _localMat = (*Goo->GetLocalMat());
			matrix4f rotationMatrix;
			if(throwDirection.x <= 0.0f)
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
			Goo->SetLocalMat(&_localMat);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	This will clean up any memory that has been created, and set all the pointers back to null.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterAttackAI::Shutdown(void)
{
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	IBaseAI::Shutdown();
}