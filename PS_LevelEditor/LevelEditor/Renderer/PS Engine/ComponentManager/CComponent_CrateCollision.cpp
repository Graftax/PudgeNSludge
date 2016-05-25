//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_CrateCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/CWinGameState.h"
#include "../Utilities/KdTree/KdTree.h"

#define IF(x) if(x)(x)

CComponent_CrateCollision::CComponent_CrateCollision()
{
	m_fPlayTimer = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
	m_pSoundComponent = NULL;
	m_bPlayed = false;
	m_bCol = false;
	m_bPrevCol = false;
	m_pCurCollisionObj = nullptr;
	m_pPrevCollisionObj = nullptr;

	this->CleanRemoveVelocity();

}

CComponent_CrateCollision::~CComponent_CrateCollision(void)
{

}

bool CComponent_CrateCollision::Initialize ()
{
	this->CleanRemoveVelocity();

	m_fPlayTimer = 0.0f;
	m_tPrevPos = vec3f(0.0f, 0.0f, 0.0f);
	m_pSoundComponent = NULL;
	m_bPlayed = false;
	m_bCol = false;
	m_bPrevCol = false;
	m_pCurCollisionObj = nullptr;
	m_pPrevCollisionObj = nullptr;
	return true;
}

void CComponent_CrateCollision::Update( float fDT )
{
	if( m_fTimeLeftToIgnorePlayer > 0.0f )
	{
		m_fTimeLeftToIgnorePlayer -= fDT;
	}
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

	m_fPlayTimer += fDT;

	// cap the objects z axis position and velocity
	m_fPos.z = -300.0f;
	m_fVel.z = 0.0f;

	vec3f fAverage;

	//m_fPlayDelay += fDT;
	//vec3f travelled = m_tPrevPos - m_pParent->GetWorldPos();
	//float dist = travelled.magnitude();
	//if(m_bPlayed && m_fPlayDelay > 0.5f && dist > 3.0)
	//{
	//	m_bPlayed = false;
	//	m_fPlayDelay = 0.0f;
	//}
	//
	//if(dist > 10.0f)
	//{
	//	m_nCollisionCount = 0;
	//	m_bCollideSoundCap = false;
	//}

	if( m_pPhysicsBox )
	{
		fAverage = m_pPhysicsBox->GetCentroid();
	}

	m_pParent->SetLocalPos( &fAverage );

	vec3f NewCenter = m_pParent->GetWorldPos();
	BoxCollisionVolume->SetWorld (NewCenter);
	m_tPrevPos = m_pParent->GetWorldPos();

	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects =  (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_WORLD_COLLISION);

	CKdTree::m_pMainTree->debugLeafTest.clear();

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_BOX, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);


	// This is debug stuff for the kd tree please leave this here for I (Josh Fields) use this from time to time Thank you.
	//for (unsigned int LeafIndex = 0; LeafIndex < CKdTree::m_pMainTree->debugLeafTest.size(); LeafIndex++)
	//{
	//	D3DXMATRIX mat;
	//	D3DXMatrixIdentity (&mat);

	//	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().x;
	//	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().y;
	//	mat._43 = -500.0f;

	//	DebugShapes::RenderSphere (mat);												 

	//	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().x;
	//	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().y;
	//	mat._43 = -500.0f;

	//	DebugShapes::RenderSphere (mat);												 

	//	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().x;
	//	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().y;
	//	mat._43 = -500.0f;

	//	DebugShapes::RenderSphere (mat);												 

	//	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().x;
	//	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().y;
	//	mat._43 = -500.0f;
	//}

	CKdTree::m_pMainTree->debugLeafTest.clear();

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
				CheckDynamicCollisions(  curCollision, curVolume, fDT );
			}
			// Collision checks against
			// our static objects
			// this is mostly our level 
			// and hazards
			else if( BT_STATIC == curVolume->GetBodyType() )
			{
				CheckStaticCollisions( curCollision, curVolume, fDT );
			}
		}
	}

	m_pParent->SetLocalPos( &m_pPhysicsBox->GetCentroid() );
	m_pParent->GetFrame().Update();

	// Set the velocity of the Box.
	m_fVel = m_pPhysicsBox->GetCentroid() - m_pPhysicsBox->GetPrevCentroid();
	vec2f fTemPos = GetPos2DClean();
	m_pParent->SetLocalPos( &vec3f( fTemPos.x, fTemPos.y, -300.0f ) );

	if((m_bPrevCol != m_bCol && m_bCol) && m_fPlayTimer > 0.1f)
	{
		m_pSoundComponent->PlaySound(SFX_CRATE_CRASH);
		m_fPlayTimer = 0.0f;
	}

	m_bPrevCol = m_bCol;
	m_bCol = false;
}

void CComponent_CrateCollision::Shutdown(void)
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
void CComponent_CrateCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
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

					
					CleanOffset( Direction * 0.5f );
					CleanForce( Direction * 0.1f );
					Direction.negate();
					pCurCollision->CleanOffset( Direction * 0.5f );
					pCurCollision->CleanForce( Direction * 0.1f );
					m_bCol = true;

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
void CComponent_CrateCollision::CheckStaticCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
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

				// If it has a physics Box.
				//if(!m_bPlayed && !m_bCollideSoundCap)
				//{
				//	if(m_nCollisionCount < 5)
				//	{
				//		m_pSoundComponent->PlaySound(SFX_CRATE_CRASH);
				//		m_bPlayed = true;
				//		++m_nCollisionCount;
				//	}
				//	else
				//	{
				//		m_bCollideSoundCap = true;
				//	}
				//}

				////////// FRICTION
				vec3f fCross;
				cross_product( fCross, MoveDirection, vec3f( 0.0f, 0.0f, 1.0f ) );
				float3 fProj = prjPoint2Line( m_pPhysicsBox->m_pPoints[0]->m_CurrPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos + fCross );
				float3 fPrevPush = fProj - m_pPhysicsBox->m_pPoints[0]->m_PrevPos;
				fPrevPush *= -0.1f;
				//////////

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
				m_bCol = true;
			}
			break;
		}
	default:
		break;
	}
}
