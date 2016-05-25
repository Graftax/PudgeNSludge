#include "CComponent_BoxPipeCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

#define SPAWN_WAIT 1.0f

CComponent_BoxPipeCollision::CComponent_BoxPipeCollision(void) : CComponent_BasePipe()
{
	m_bButtonPressed = false;
	m_bPrevButtonPressed = false;
	CleanRemoveVelocity();
	m_nPipeType = EPIPE_BOX;
	m_unMaxSpawns = 1;
	m_fSpawnTimer = 0.0f;
}

CComponent_BoxPipeCollision::~CComponent_BoxPipeCollision(void)
{

}

bool CComponent_BoxPipeCollision::Initialize(void)
{
	CComponent_BasePipe::Initialize();
	m_bButtonPressed = false;
	m_bPrevButtonPressed = false;
	CleanRemoveVelocity();
	m_nPipeType = EPIPE_BOX;
	m_unMaxSpawns = 1;
	m_fSpawnTimer = 0.0f;
	return true;
}

void CComponent_BoxPipeCollision::Render(void)
{

}

void CComponent_BoxPipeCollision::Update(float fDT)
{
	IBaseObject* pCrate;
	//go through the parent class' update to handle collisions
	CComponent_BasePipe::Update(fDT);

	if(m_fSpawnTimer <= SPAWN_WAIT)
	{
		m_fSpawnTimer += fDT;
	}

	//deal with the button press
	if(m_bButtonPressed && !m_bPrevButtonPressed && m_fSpawnTimer > SPAWN_WAIT)
	{
		m_fSpawnTimer = 0.0f;
		if(m_nActiveSpawns < (int)(m_unMaxSpawns))
		{
			//spawn a new box
			D3DXMATRIX pos;
			D3DXMatrixIdentity( &pos );
			D3DXMatrixTranslation( &pos, m_pParent->GetWorldPos().x, m_pParent->GetWorldPos().y, m_pParent->GetWorldPos().z );
			pCrate = CObjectManager::GetInstance ()->CreateObject(OBJ_CRATE, pos, GetParent()->GetLevelID() );
			if( pCrate )
			{
				AddChild(pCrate);	// This adds the crate to the list of children and increments the number of children.
				((CComponent_Item*)pCrate->GetComponent(ECOMP_ITEM))->SetSpawnIgnoreTime( 3.0f );	// set the ingore timer for the little goo;
				((CComponent_CrateCollision*)pCrate->GetComponent(ECOMP_COLLISION))->SetSpawnObject(m_pParent);
				((CComponent_CrateCollision*)pCrate->GetComponent(ECOMP_COLLISION))->CleanRemoveVelocity();
				CParticleManager::GetInstance()->CreateEffectOneShot( "PushInside", GetParent()->GetWorldPos() + vec3f( 0.0f, -50.0f, 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ) ); 
				CParticleManager::GetInstance()->CreateEffectOneShot( "PushOutside", GetParent()->GetWorldPos() + vec3f( 0.0f, -50.0f, 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ) );
				CParticleManager::GetInstance()->CreateEffectOneShot( "PushSparkles", GetParent()->GetWorldPos() + vec3f( 0.0f, -50.0f, 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ) ); 
			}
		}
		else if(m_nActiveSpawns >= (int)(m_unMaxSpawns))
		{	
			pCrate = m_vChildren[0];
			if(!((CComponent_Item*)pCrate->GetComponent(ECOMP_ITEM))->GetAttached())
			{
				m_vChildren.erase(m_vChildren.begin());
				m_vChildren.push_back(pCrate);
				((CComponent_Item*)pCrate->GetComponent(ECOMP_ITEM))->SetSpawnIgnoreTime( 3.0f );	// set the ingore timer for the little goo;
				((CComponent_Item*)pCrate->GetComponent(ECOMP_ITEM))->SetTarget(nullptr);

				CParticleManager::GetInstance()->CreateEffectOneShot( "LandingSmoke", ((CComponent_CrateCollision*)pCrate->GetComponent(ECOMP_COLLISION))->GetPhysicsBox()->GetCentroid() );
				CParticleManager::GetInstance()->CreateEffectOneShot( "Sparks", ((CComponent_CrateCollision*)pCrate->GetComponent(ECOMP_COLLISION))->GetPhysicsBox()->GetCentroid(), vec3f( 0.0f, 1.0f, 0.0f ) );

				CParticleManager::GetInstance()->CreateEffectOneShot( "PushInside", GetParent()->GetWorldPos() + vec3f( 0.0f, -50.0f, 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ) ); 
				CParticleManager::GetInstance()->CreateEffectOneShot( "PushOutside", GetParent()->GetWorldPos() + vec3f( 0.0f, -50.0f, 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ) );
				CParticleManager::GetInstance()->CreateEffectOneShot( "PushSparkles", GetParent()->GetWorldPos() + vec3f( 0.0f, -50.0f, 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ) ); 

				((CComponent_CrateCollision*)pCrate->GetComponent(ECOMP_COLLISION))->CleanRemoveVelocity();
				((CComponent_CrateCollision*)pCrate->GetComponent(ECOMP_COLLISION))->GetPhysicsBox()->SetPosition(GetPos());
			}
			//recycling is fun!
		}
	}

	if(!m_bButtonPressed)
	{
		m_bPrevButtonPressed = false;
	}
	else
	{
		m_bPrevButtonPressed = true;
	}

	m_bButtonPressed = false;
}

void CComponent_BoxPipeCollision::Shutdown(void)
{
	m_bButtonPressed = false;
	m_bPrevButtonPressed = false;
	CComponent_BasePipe::Shutdown();
}