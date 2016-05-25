//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_CorkCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/CWinGameState.h"
#include "../Utilities/KdTree/KdTree.h"


CComponent_CorkCollision::CComponent_CorkCollision()
{
	m_bPulled = false;
	m_bCorked = false;
	m_bPlayed = false;
	m_fPlayDelay = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
	m_pSoundComponent = nullptr;

	this->CleanRemoveVelocity();

}

CComponent_CorkCollision::~CComponent_CorkCollision(void)
{

}

bool CComponent_CorkCollision::Initialize ()
{
	m_bPulled = false;
	m_bCorked = false;
	m_bPlayed = false;
	m_fPlayDelay = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
	m_pSoundComponent = nullptr;

	this->CleanRemoveVelocity();
	return true;
}

void CComponent_CorkCollision::Update( float fDT )
{
	// No need to update if the cork is
	// stuck in a pipe

	if( m_fTimeLeftToIgnorePlayer > 0.0f )
	{
		m_fTimeLeftToIgnorePlayer -= fDT;
	}

	if( m_bCorked )
		return;

	// Volume for collision checks
	Box* BoxCollisionVolume;

	if( m_pParent )
	{
		if (m_pParent->GetCollidableObject()->GetVolumeType () != VMT_BOX)
		{
			return;
		}

		if(!m_pSoundComponent )
		{
			m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));
		}
		// Set the Volume
		BoxCollisionVolume = (Box*)m_pParent->GetCollidableObject();
	}

	m_bCollidingGround = false;

	// cap the objects z axis position and velocity
	m_fPos.z = -300.0f;
	m_fVel.z = 0.0f;

	m_fPlayDelay += fDT;
	vec3f travelled = m_tPrevPos - m_pParent->GetWorldPos();
	float dist = travelled.magnitude();
	if(m_bPlayed && m_fPlayDelay > 0.1f && dist > 3.0)
	{
		m_bPlayed = false;
		m_fPlayDelay = 0.0f;
	}

	vec3f fAverage;

	if( m_pPhysicsBox )
	{
		fAverage = m_pPhysicsBox->GetCentroid();
	}

	m_pParent->SetLocalPos( &fAverage );

	vec3f NewCenter = m_pParent->GetWorldPos();
	BoxCollisionVolume->SetWorld (NewCenter);
	m_tPrevPos = m_pParent->GetWorldPos();

	/*D3DXMATRIX mat;

	D3DXMatrixIdentity (&mat);

	mat._41 = ((Box*)m_cvVolume)->GetVertices()[0].Get2DCentorid().x + ((Box*)m_cvVolume)->GetTransform().m_v2Position.x;
	mat._42 = ((Box*)m_cvVolume)->GetVertices()[0].Get2DCentorid().y + ((Box*)m_cvVolume)->GetTransform().m_v2Position.y;
	mat._43 = -300.0f;

	DebugShapes::RenderSphere (mat);

	mat._41 = ((Box*)m_cvVolume)->GetVertices()[1].Get2DCentorid().x + ((Box*)m_cvVolume)->GetTransform().m_v2Position.x;
	mat._42 = ((Box*)m_cvVolume)->GetVertices()[1].Get2DCentorid().y + ((Box*)m_cvVolume)->GetTransform().m_v2Position.y;
	mat._43 = -300.0f;

	DebugShapes::RenderSphere (mat);

	mat._41 = ((Box*)m_cvVolume)->GetVertices()[2].Get2DCentorid().x + ((Box*)m_cvVolume)->GetTransform().m_v2Position.x;
	mat._42 = ((Box*)m_cvVolume)->GetVertices()[2].Get2DCentorid().y + ((Box*)m_cvVolume)->GetTransform().m_v2Position.y;
	mat._43 = -300.0f;

	DebugShapes::RenderSphere (mat);

	mat._41 = ((Box*)m_cvVolume)->GetVertices()[3].Get2DCentorid().x + ((Box*)m_cvVolume)->GetTransform().m_v2Position.x;
	mat._42 = ((Box*)m_cvVolume)->GetVertices()[3].Get2DCentorid().y + ((Box*)m_cvVolume)->GetTransform().m_v2Position.y;
	mat._43 = -300.0f;

	DebugShapes::RenderSphere (mat);*/

	//static int aabbIndex = 0;
	//static float fAABBIndex = 0;

	//mat._41 = CObjectManager::GetInstance ()->m_AABBs[aabbIndex].GetMin3DTransformed().x;
	//mat._42 = CObjectManager::GetInstance ()->m_AABBs[aabbIndex].GetMin3DTransformed().y;
	//mat._43 = -500.0f;

	//DebugShapes::RenderSphere (mat);

	//mat._41 = CObjectManager::GetInstance ()->m_AABBs[aabbIndex].GetMax3DTransformed().x;
	//mat._42 = CObjectManager::GetInstance ()->m_AABBs[aabbIndex].GetMax3DTransformed().y;
	//mat._43 = -500.0f;

	//fAABBIndex += 1.0f * fDT;
	//aabbIndex = fAABBIndex;
	//DebugShapes::RenderSphere (mat);

	//if (aabbIndex > 47)
	//	aabbIndex = 0;

	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_WORLD_COLLISION);

	CKdTree::m_pMainTree->debugLeafTest.clear();

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_BOX, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		IBaseObject* curObj = (IBaseObject*)NearestObjects[objindex];
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume )
		{
			vec2f contactpoints[2];
			unsigned int numPoints = 0;
			vec2f Direction;
			vec2f Contact;

			// Collision checks against
			// our moveable objects
			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				CheckDynamicCollisions( curCollision, curVolume, fDT );
			}
			else if( BT_STATIC == curVolume->GetBodyType() )
			{
				CheckStaticCollisions( curCollision, curVolume, fDT );
			}
		}
	}

	m_pParent->SetLocalPos( &m_pPhysicsBox->GetCentroid() );
	m_fVel = m_pPhysicsBox->GetCentroid() - m_pPhysicsBox->GetPrevCentroid();
}

void CComponent_CorkCollision::Shutdown(void)
{

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Ethan Pendergraft
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_CorkCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	Box* BoxCollisionVolume = (Box*)m_pParent->GetCollidableObject();
	vec2f Contact;
	vec2f Direction;
	vec2f contactpoints[4];
	unsigned int numPoints;

	if( pCurCollision && pCurCollision != this )
	{
		// collision checks
		// based on volume type
		switch( pCurVolume->GetVolumeType() )
		{
		case VMT_BOX:
			{

				CComponent_Item* pThisItem = (CComponent_Item*)m_pParent->GetComponent( ECOMP_ITEM );
				if( pThisItem )
				{
					if( pThisItem->GetTarget() )
						return;
				}
				CComponent_Item* pThatItem = (CComponent_Item*)pCurCollision->GetParent()->GetComponent( ECOMP_ITEM );
				if( pThatItem )
				{
					if( pThatItem->GetTarget() )
						return;
				}

				if ( BoxCollisionVolume->BoxToBox( (Box*)pCurVolume, contactpoints, numPoints, Direction ) )
				{
					// Collision reaction with other boxes in the game
					vec3f MoveDirection = vec3f( Direction.x, Direction.y, 0.0f );
					vec3f fCross;
					cross_product( fCross, MoveDirection, vec3f( 0.0f, 0.0f, 1.0f ) );
					float3 fProj = prjPoint2Line( m_pPhysicsBox->m_pPoints[0]->m_CurrPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos + fCross );
					float3 fPrevPush = fProj - m_pPhysicsBox->m_pPoints[0]->m_PrevPos;
					fPrevPush *= -0.1f;

					CleanForce( Direction * 0.1f );
					CleanOffset( Direction * 0.5f );
					Direction.negate();
					pCurCollision->CleanOffset( Direction * 0.5f );
					pCurCollision->CleanForce( Direction * 0.1f );

					vec3f NormalizedDirection = vec3f (Direction.x, Direction.y, 0.0f).normalize();
					float fDotProductUp = dot_product( NormalizedDirection, vec3f( 0.0f, 1.0f, 0.0f ) );
					if( fDotProductUp > 0.5f )
					{
						m_bCollidingGround = true;
					}
					if( fDotProductUp < -0.5f )
					{
						pCurCollision->SetCollidingGroundAlone( true );
					}
				}
				break;
			}
		}
		if( m_pPhysicsBox )
		{
			BoxCollisionVolume->SetWorld( m_pPhysicsBox->GetCentroid() );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckStaticCollisions()
// Purpose: Check collisions with all static objects
// Original Author: Ethan Pendergraft
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_CorkCollision::CheckStaticCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	Box* BoxCollisionVolume = (Box*)m_pParent->GetCollidableObject();
	vec2f Contact;
	vec2f Direction;

	switch( pCurVolume->GetVolumeType() )
	{

	case VMT_AABB:
		{
			// handle crate collision
			// with world AABB
			if ( BoxCollisionVolume->BoxToAABBWorld( *(AABB*)pCurVolume, Contact, Direction ) )
			{
				// Create an offset vector.
				vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);
				//Save the last offset for use elsewhere.
				m_fLastOffset = vec3f( Direction.x, Direction.y, 0.0f );

				////////// FRICTION
				vec3f fCross;
				cross_product( fCross, MoveDirection, vec3f( 0.0f, 0.0f, 1.0f ) );
				float3 fProj = prjPoint2Line( m_pPhysicsBox->m_pPoints[0]->m_CurrPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos + fCross );
				float3 fPrevPush = fProj - m_pPhysicsBox->m_pPoints[0]->m_PrevPos;
				fPrevPush *= -0.1f;
				//////////

				//for (unsigned int physicIndex = 0; physicIndex < 3; physicIndex++)
				//{
				//	// Update its position to reflect the changes.
				//	m_pPhysicsBox->m_pPoints[physicIndex]->m_CurrPos += MoveDirection;
				//	// Apply friction to the objects previous position.
				//	m_pPhysicsBox->m_pPoints[physicIndex]->m_PrevPos += fPrevPush;
				//}

				vec3f NormalizedDirection = vec3f (Direction.x, Direction.y, 0.0f).normalize();
				float fDotProductUp = dot_product( NormalizedDirection, vec3f( 0.0f, 1.0f, 0.0f ) );
				float fDotProductSide = dot_product( NormalizedDirection, vec3f( 1.0f, 0.0f, 0.0f ) );
				if( fDotProductUp > 0.5f )
				{
					m_bCollidingGround = true;
				}
				if( abs( fDotProductSide ) > 0.5f )
				{
					fPrevPush = vec3f( 0.0f, 0.0f, 0.0f );
				}
				
				CleanOffset( vec2f( MoveDirection.x, MoveDirection.y ) );
				CleanForce( vec2f( fPrevPush.x, fPrevPush.y ) + vec2f( MoveDirection.x, MoveDirection.y ) * 0.1f );
				
			}
			break;
		}
	default:
		break;
	}
}
