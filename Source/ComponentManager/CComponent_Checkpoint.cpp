/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_Checkpoint
//
// Purpose: Saves players progress, heals the player
//
// Original Author: Kevin Clouden
//
// Creation Date: 6/10/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_Checkpoint.h"
#include "..//ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/GamePlayState.h"
#include "..//RenderEngine/RenderEngine.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../PS/CGame.h"
#include "../PS/States/CLoadingState.h"
#include "../LevelManager/CLevelManager.h"
#include "../EffectsLib/ParticleManager.h"

CComponent_Checkpoint::CComponent_Checkpoint() : IComponent( ECOMP_CHECKPOINT )
{
	m_pCmp = nullptr;
	m_pPlayer = nullptr;
	m_pInput = nullptr;
	m_pAnimComponent = nullptr;
	m_bSector1 = false;
	m_szNextLevel = "";
	m_pLvlMng = nullptr;
	m_pPartMng = nullptr;
	m_pSoundCmp = nullptr;

	m_pShowerEffect1 = nullptr;
	m_pShowerEffect2 = nullptr;
	m_pShowerEffect3 = nullptr;
	m_bPlayed= false;
}

CComponent_Checkpoint::~CComponent_Checkpoint()
{
	m_szNextLevel = "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Initialize()
// Purpose: Initializes data members
// Original Author: Kevin Clouden
// Creation Date: 6/10/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Checkpoint::Initialize( void )
{
	m_bSector1 = false;

	m_szNextLevel = "";
	m_bHasPrevLevelBeenUnloaded = false;

	m_bSector1 = true;
	m_bColliding = m_bPrevCollided = false;

	m_pPlayer = CObjectManager::GetInstance()->GetPlayer();
	m_pCmp = m_pPlayer->GetComponent(ECOMP_HEALTH);
	m_pInput = CGame::GetInstance()->GetInputManager();
	m_pAnimComponent = (CComponent_Animation*)m_pPlayer->GetComponent(ECOMP_ANIMATION);
	m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);

	m_pLvlMng = LevelManager::GetInstance();
	m_pPartMng = CParticleManager::GetInstance();

	m_pShowerEffect1 = m_pPartMng->CreateEffect("Showers", m_pParent->GetWorldPos() + vec3f( -279.0f, 130.0f, 75.0f ) );
	m_pShowerEffect2 = m_pPartMng->CreateEffect("Showers", m_pParent->GetWorldPos() + vec3f( -35.0f,  130.0f, 75.0f ) ); // THE CENTER
	m_pShowerEffect3 = m_pPartMng->CreateEffect("Showers", m_pParent->GetWorldPos() + vec3f(  209.0f, 130.0f, 75.0f ) );
	m_pFloorSplashEffect = m_pPartMng->CreateEffect( "FloorSplash", m_pParent->GetWorldPos() + vec3f( -35.0f, -110.0f, 0.0f ) );

	m_bPlayed = false;

	return true;
}

void CComponent_Checkpoint::Shutdown ()
{
	m_pPartMng->DestroyEffect( m_pShowerEffect1 );
	m_pPartMng->DestroyEffect( m_pShowerEffect2 );
	m_pPartMng->DestroyEffect( m_pShowerEffect3 );
	m_pPartMng->DestroyEffect( m_pFloorSplashEffect );

	m_szNextLevel = "";
}

void CComponent_Checkpoint::BuildAABB ()
{
	float ValueToAddOnX = 450.0f;
	float ValueToAddOnY = 100.0f;

	m_Sector1CheckPoint.SetMaxPoint(vec3f( m_vPosition.x + ValueToAddOnX, m_vPosition.y + ValueToAddOnY, m_vPosition.z - 200.0f) );
	m_Sector1CheckPoint.SetMinPoint(vec3f( m_vPosition.x - ValueToAddOnX, m_vPosition.y - ValueToAddOnY, m_vPosition.z + 200.0f ) );

	m_Sector1HealingPoint.SetMaxPoint(vec3f( m_vPosition.x + ValueToAddOnX, m_vPosition.y + ValueToAddOnY, m_vPosition.z - 200.0f) );
	m_Sector1HealingPoint.SetMinPoint(vec3f( m_vPosition.x - ValueToAddOnX, m_vPosition.y - ValueToAddOnY, m_vPosition.z + 200.0f ) );

	m_Sector1CheckPoint.SetBodyType( BT_DYNAMIC );
	m_Sector1CheckPoint.SetVolumeType( VMT_AABB );

	m_Sector1HealingPoint.SetBodyType( BT_DYNAMIC );
	m_Sector1HealingPoint.SetVolumeType( VMT_AABB );

	m_pParent->SetCollidableObject ( &m_Sector1CheckPoint );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: No purpose yet...
// Original Author: Kevin Clouden
// Creation Date: 6/10/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Checkpoint::Update( float fDT )
{
	m_bColliding = false;

	m_pShowerEffect1->Emit( fDT );
	m_pShowerEffect2->Emit( fDT );
	m_pShowerEffect3->Emit( fDT );
	m_pFloorSplashEffect->Emit( fDT );

	if(m_pLvlMng->GetCurrentLevelIndex() == (m_pParent->GetLevelID()))
	{
		if(m_pSoundCmp && !m_bPlayed)
		{
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_SPRINKLERS);
			m_bPlayed = true;
		}
	}

	// useless vec
	// passed in to AABBToAABB Method
	vec2f useless;		
	// If you are collided with the checkpoint AABB
	if( ((AABB*)m_pPlayer->GetCollidableObject())->AABBToAABB (m_Sector1HealingPoint, useless) )
	{
		//Heals the player when he hits the healing shower
		((CComponent_Health*)m_pCmp)->RemoveSludge( 50.0f * fDT );
	}

	//If you are collided with the checkpoint AABB
	if( ((AABB*)m_pPlayer->GetCollidableObject())->AABBToAABB (m_Sector1CheckPoint, useless) )
	{		
		if( m_pLvlMng->GetCurrentLevelIndex() == (m_pParent->GetLevelID() - 1) )
			m_pLvlMng->GetCurrentLevel()->GetLightManager().SetRaiseBrightness( false );

		// Set colliding to true.
		m_bColliding = true;
		IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
		/*vec3f PlayerPos = player->GetFrame().GetWorldMat().axis_pos;
		vec3f CPPos = m_pParent->GetFrame().GetWorldMat().axis_pos;

		vec3f VectorFromMaxPosToPlayerPos = (PlayerPos - CPPos );
		float Distance = dot_product( vec3f( 1.0f, 0.0f, 0.0f), VectorFromMaxPosToPlayerPos );

		if( Distance > 0.0f )
		{
			if( !m_bHasPrevLevelBeenUnloaded )
			{
				m_pLvlMng->SetLoadNextLevel (true);
				((CComponent_Input*)player->GetComponent( ECOMP_INPUT ))->ResetPulledObject();
				RenderEngine::SetObjPulled(false);
				m_pLvlMng->SetUnloadFirstLevel( true );
				m_bHasPrevLevelBeenUnloaded = true;
			}
		}*/

		if( !m_bHasPrevLevelBeenUnloaded )
		{
			m_pLvlMng->SetLoadNextLevel (true);
			((CComponent_Input*)player->GetComponent( ECOMP_INPUT ))->ResetPulledObject();
			RenderEngine::SetObjPulled(false);
			m_pLvlMng->SetUnloadFirstLevel( true );
			m_bHasPrevLevelBeenUnloaded = true;
		}
	}

	// If we were previously colliding with the checkpoint and we are no longer colliding.
	if( m_bPrevCollided && !m_bColliding )
	{
		// Perform a HalfSpace Test and determine if the Player is past the max of the checkpoint room. 
		// If so then go ahead and unload the first level.
		vec3f PlayerPos = CObjectManager::GetInstance()->GetPlayer()->GetFrame().GetWorldMat().axis_pos;
		vec3f MaxCPPos = m_Sector1CheckPoint.GetMax3DTransformed();

		vec3f VectorFromMaxPosToPlayerPos = (PlayerPos - MaxCPPos );
		float Distance = dot_product( vec3f( 1.0f, 0.0f, 0.0f), VectorFromMaxPosToPlayerPos );

		if( Distance > 0.0f )
		{
			// If we reach this point it means we passed the halfspace test, which means we are to the right
			// of the center of the checkpoint. So now lets check to see if we are no longer colliding with
			// the checkpoint and we are to the right of it.
			m_pLvlMng->SetUnloadFirstLevel( true );
			// Load up the next Checkpoint.
			m_pLvlMng->SetLoadNextLevel (true);

			if(m_pSoundCmp)
			{
				m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_SPRINKLERS);
			}
		}
	}

	// Set previous colliding to colliding.
	m_bPrevCollided = m_bColliding;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: SaveProgress()
// Purpose: Saves player progress
// Original Author: Kevin Clouden
// Creation Date: 6/10/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Checkpoint::SaveProgress(  )
{

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: LoadProgress()
// Purpose: Loads player progress
// Original Author: Kevin Clouden
// Creation Date: 6/10/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Checkpoint::LoadProgress(  )
{
	m_pPlayer->SetLocalPos(&m_pParent->GetLocalMat()->axis_pos);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: LoadNextLevel()
// Purpose: Loads next lvl
// Original Author: Kevin Clouden
// Creation Date: 6/10/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Checkpoint::LoadNextLevel( char* szLevelname )
{
	CGame::GetInstance()->SwapLevels (szLevelname);
	return true;
}

// CComponent_Checkpoint::tCheckpoint CComponent_Checkpoint::GetCurrentLevel()
// {
// 
// 	return 
// }
