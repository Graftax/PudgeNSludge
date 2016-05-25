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

CComponent_Checkpoint::CComponent_Checkpoint() : IComponent( ECOMP_CHECKPOINT )
{
	m_pCmp = nullptr;
	m_pPlayer = nullptr;
	m_pInput = nullptr;
	m_pAnimComponent = nullptr;
	m_bSector1 = false;
}

CComponent_Checkpoint::~CComponent_Checkpoint()
{

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
	m_pCmp = nullptr;
	m_pPlayer = nullptr;
	m_pInput = nullptr;
	m_pAnimComponent = nullptr;
	m_bSector1 = false;

	//Init the hard coded collision box
	m_Sector1CheckPoint.SetMaxPoint(vec3f( 7500.0f, -358.0f, 382.0f ) );
	m_Sector1CheckPoint.SetMinPoint(vec3f( 5600.0f, 1200.0f, -400.0f ) );
	  
	m_Sector1HealingPoint.SetMaxPoint(vec3f( 3500.0f, 358.0f, 382.0f ) );
	m_Sector1HealingPoint.SetMinPoint(vec3f( 3050.0f, 94.0f, 167.0f ) );

// 	m_Sector2CheckPoint.SetMaxPoint(vec3f( 4000.0f, 358.0f, 382.0f ) );
// 	m_Sector2CheckPoint.SetMinPoint(vec3f( 3500.0f, 94.0f, 167.0f ) );
// 			
// 	m_Sector2HealingPoint.SetMaxPoint(vec3f( 3500.0f, 358.0f, 382.0f ) );
// 	m_Sector2HealingPoint.SetMinPoint(vec3f( 3050.0f, 94.0f, 167.0f ) );


	m_bSector1 = true;
	return true;
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
	if(!m_pPlayer)
	{
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();
		m_pCmp = m_pPlayer->GetComponent (ECOMP_HEALTH);
	}
	if(!m_pInput)
		m_pInput = CGame::GetInstance()->GetInputManager();

// 	if(m_pInput->KeyPressed(DIK_NUMPAD3))
// 		((CComponent_Health*)m_pCmp)->RemoveSludge(100.0f);
// 
// 	if(m_pInput->KeyPressed(DIK_NUMPAD4))
// 		((CComponent_Health*)m_pCmp)->AddSludge(10.0f);
// 
// 	if(m_pInput->KeyPressed(DIK_NUMPAD1))
// 		((CComponent_Health*)m_pCmp)->AddSludge(10.0f);
// 	if(m_pInput->KeyPressed(DIK_NUMPAD2))
// 		((CComponent_Health*)m_pCmp)->AddSludge(15.0f);


// 	D3DXMATRIX mat;
//// 
// 	D3DXMatrixIdentity (&mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMax3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMax3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMax3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMax3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMax3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMin3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMax3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMin3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMax3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMax3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMin3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMin3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMin3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMin3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMin3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMin3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMin3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMax3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 =((AABB)m_HealingPoint).GetMin3DTransformed().x;
// 	mat._42 =((AABB)m_HealingPoint).GetMax3DTransformed().y;
// 	mat._43 =((AABB)m_HealingPoint).GetMin3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 	mat._41 = ((AABB)m_HealingPoint).GetMin3DTransformed().x;
// 	mat._42 = ((AABB)m_HealingPoint).GetMax3DTransformed().y;
// 	mat._43 = ((AABB)m_HealingPoint).GetMax3DTransformed().z;
// 	DebugShapes::RenderSphere (mat);
// 
// 
// 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 =((AABB)m_Sector1CheckPoint).GetMin3DTransformed().x;
 	//mat._42 =((AABB)m_Sector1CheckPoint).GetMax3DTransformed().y;
 	//mat._43 =((AABB)m_Sector1CheckPoint).GetMin3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);
 
 	//mat._41 = ((AABB)m_Sector1CheckPoint).GetMin3DTransformed().x;
 	//mat._42 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().y;
 	//mat._43 = ((AABB)m_Sector1CheckPoint).GetMax3DTransformed().z;
 	//DebugShapes::RenderSphere (mat);

	// useless vec
	// passed in to AABBToAABB Method
	
	
static bool Healed = false;

	vec2f useless;
	if (!Healed)
	{
		
		// If you are collided with the checkpoint AABB
		if( ((AABB*)m_pParent->GetCollidableObject())->AABBToAABB (m_Sector1HealingPoint, useless) )
		{
			//Heals the player when he hits the healing shower
			((CComponent_Health*)m_pCmp)->RemoveSludge(100.0f);
			Healed = true;
		}
	}

	// If you are collided with the checkpoint AABB
	if( ((AABB*)m_pParent->GetCollidableObject())->AABBToAABB (m_Sector1CheckPoint, useless) )
	{
		// Set the pulled object to false
		RenderEngine::SetObjPulled(false);
		
		char buffer[64];
		sprintf_s(buffer, "./Resources/Models/Sector%d.InfiniteHPScene", 2);
		LoadNextLevel(buffer);

		// Change state to Win after all the
		// updates are done
		CGamePlayState::GetInstance()->WinGameFade();
	}

	//LoadProgress();
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
bool CComponent_Checkpoint::LoadNextLevel( char* szFilename )
{

	return true;
}

// CComponent_Checkpoint::tCheckpoint CComponent_Checkpoint::GetCurrentLevel()
// {
// 
// 	return 
// }
