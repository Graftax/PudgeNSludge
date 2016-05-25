#include "CComponent_IntakePipeCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../RenderEngine/DebugShapes.h"
#include "CComponent_SteamPipeCollision.h"
#define IF(x) if(x)(x)

CComponent_IntakePipeCollision::CComponent_IntakePipeCollision()
{
	m_bCorked = false;
	m_pObjectMngr = NULL;
	m_pPartner = nullptr;
	m_abTarget.SetMaxPoint (vec2f (40.0f, 0.0f));
	m_abTarget.SetMinPoint (vec2f (-40.0f, -40.0f));
	m_abTarget.SetVolumeType( VMT_AABB );

	m_bCorkable = true;
	m_nChannel = -1;
	m_pCollisionObject = nullptr;

	this->CleanRemoveVelocity();

}

CComponent_IntakePipeCollision::~CComponent_IntakePipeCollision()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize():	
//
// Returns:			bool
//
// Mod. Name:		Ethan Pendergraft
// Mod. Date:		6/2/2012
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_IntakePipeCollision::Initialize()
{
	m_bCorked = false;
	m_pObjectMngr = NULL;
	m_pPartner = nullptr;
	m_bCorkable = true;
	m_nChannel = -1;
	m_pCollisionObject = nullptr;
	this->CleanRemoveVelocity();
	m_pCork = nullptr;
	m_bIsGettingCorked = false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():		
//
// Returns:			Void
//
// Mod. Name:		Ethan Pendergraft
// Mod. Date:		6/2/2012
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::Update( float fDT )
{
	// We only need to check collisions if
	// the pipe is corkable
	

	if( m_pParent )
	{		
		// Set the collision object 
		// to the parent's collision object 
		if( !m_pCollisionObject )
		{
			m_pCollisionObject = (AABB*)m_pParent->GetCollidableObject();
		}
	}

	// only move the plug
	// if we aren't plugged yet
	if( m_bIsGettingCorked )
	{
		MoveCork( fDT );
	}

	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	m_abTarget.SetWorldPoint (NewCenter);
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (NewCenter);

	CheckCollisions( fDT );
	
	// don't check for the cork
	// if you can't cork this pipe
	if( !m_bCorkable )
	{
		return;
	}

	CheckCorkableAABB( fDT );

	//D3DXMATRIX mat;

	//D3DXMatrixIdentity (&mat);

	//mat._41 = m_abTarget.GetMax2DTransformed().x;
	//mat._42 = m_abTarget.GetMax2DTransformed().y;
	//mat._43 = -300.0f;

	//DebugShapes::RenderSphere (mat);

	//mat._41 = m_abTarget.GetMin2DTransformed().x;
	//mat._42 = m_abTarget.GetMin2DTransformed().y;
	//mat._43 = -300.0f;

	//DebugShapes::RenderSphere (mat);

	// We only need to check collisions if
	// the pipe is corkable
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():		
//
// Returns:			Void
//
// Mod. Name:		Ethan Pendergraft
// Mod. Date:		6/3/2012
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::Shutdown(void)
{
	m_bCorked = false;
	m_pObjectMngr = NULL;
	m_pPartner = nullptr;
	m_abTarget.SetMaxPoint (vec2f (40.0f, 0.0f));
	m_abTarget.SetMinPoint (vec2f (-40.0f, -40.0f));

	m_bCorkable = true;
	m_nChannel = -1;
	m_pCollisionObject = nullptr;
	m_pCork = nullptr;
	m_bIsGettingCorked = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: BuildCorkableAABB()
// Purpose: Create an AABB for the cork to collide with
// Original Author: Rueben Massey
// Creation Date: 6/5/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::BuildCorkableAABB( int nDirection )
{
	switch( nDirection)
	{
	case 0:
		{
			m_abTarget.SetMaxPoint (vec2f ( INTAKECORKABLE_AABB_WIDTH, INTAKECORKABLE_AABB_OFFSET + INTAKECORKABLE_AABB_HEIGHT ));
			m_abTarget.SetMinPoint (vec2f ( -INTAKECORKABLE_AABB_WIDTH, INTAKECORKABLE_AABB_OFFSET ));
			break;
		}
	case 1:
		{
			m_abTarget.SetMaxPoint (vec2f ( INTAKECORKABLE_AABB_WIDTH, -INTAKECORKABLE_AABB_OFFSET ));
			m_abTarget.SetMinPoint (vec2f ( -INTAKECORKABLE_AABB_WIDTH, -INTAKECORKABLE_AABB_OFFSET - INTAKECORKABLE_AABB_HEIGHT  ));
			break;
		}
	case 2:
		{
			m_abTarget.SetMaxPoint (vec2f ( -INTAKECORKABLE_AABB_OFFSET, INTAKECORKABLE_AABB_WIDTH ));
			m_abTarget.SetMinPoint (vec2f ( -INTAKECORKABLE_AABB_OFFSET - INTAKECORKABLE_AABB_HEIGHT, -INTAKECORKABLE_AABB_WIDTH ));
			break;
		}
	case 3:
		{
			m_abTarget.SetMaxPoint (vec2f ( INTAKECORKABLE_AABB_OFFSET + INTAKECORKABLE_AABB_HEIGHT, INTAKECORKABLE_AABB_WIDTH ));
			m_abTarget.SetMinPoint (vec2f ( INTAKECORKABLE_AABB_OFFSET, -INTAKECORKABLE_AABB_WIDTH ));
			break;
		}
	};
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCorkableAABB()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::CheckCorkableAABB( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_CORK);

	CSceneObject intakeAABB;
	intakeAABB.SetCollidableObject( &m_abTarget );

	// call the kd tree and get the near objects
	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( &intakeAABB, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects );
	//intakeAABB.SetCollidableObject( NULL );
	vec2f Direction;
	vec2f CP;
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex]))->GetComponent (ECOMP_COLLISION);
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{	
			if( m_bCorked )
			{
				break;
			}			
			// We have collided
			// Do stuff to the object
			if( ((Box*)curVolume)->BoxToAABB( m_abTarget, CP, Direction ) )
			{
													
				// We have collided
				// get the plug's collision component
				// so we can move it
				m_pCork = curCollision;
				// set this flag
				// so we can move the plug
				// in update
				m_bIsGettingCorked = true;				
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_PLUG) | (1<<OBJ_CORK);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects );

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();

		if( curCollision->GetParent()->GetType() == OBJ_CORK )
		{
			if( ((CComponent_CorkCollision*)curCollision)->GetCorked() )
			{
				return;
			}
		}
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{

			// Check if the body type is dynamic
			// meaning it's a moving object in the game
			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				CheckDynamicCollisions( curCollision, curVolume, fDT );
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/11/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	// Variables to catch returns
	vec2f Direction;
	vec2f CP;

	// Check collisions
	// based on the current objects collision volume
	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((AABB*)pCurVolume)->AABBToAABB( *m_pCollisionObject, Direction ) )
			{
				// Apply push-out
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );
			}
			break;
		}
	case VMT_BOX:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((Box*)pCurVolume)->BoxToAABB( *m_pCollisionObject, CP,Direction ) )
			{
				// Apply push-out
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );
			}
			break;
		}
	default:
		break;
	};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: MoveCork()
// Purpose: move the cork to the correct position
// Original Author: Rueben Massey
// Creation Date: 6/16/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_IntakePipeCollision::MoveCork( float fDT )
{
	/*m_bCorked = true;
	((CComponent_CorkCollision*)curCollision)->SetCorked(true);
	curCollision->GetParent()->SetLocalPos( &vec3f( 
	( m_abTarget.GetMax2DTransformed().x + m_abTarget.GetMin2DTransformed().x ) / 2.0f, 
	( m_abTarget.GetMax2DTransformed().y + m_abTarget.GetMin2DTransformed().y ) / 2.0f, 
	-300.0f ) );

	if( m_pPartner )
	{
	CComponent_Collision* pColl = (CComponent_Collision*)m_pPartner->GetComponent( ECOMP_COLLISION );
	if( pColl && pColl->GetCollType() == CCMP_STEAMPIPE )
	{
	((CComponent_SteamPipeCollision*)(pColl))->SetCorked( true );
	}
	}
	*/
	if( m_pCork )
	{
		// Place Plug in the proper position
		// Set the plug to plugged
		vec3f _pluggedPos = vec3f(	( m_abTarget.GetMax2DTransformed().x + m_abTarget.GetMin2DTransformed().x ) / 2.0f, 
			( m_abTarget.GetMax2DTransformed().y + m_abTarget.GetMin2DTransformed().y ) / 2.0f,  
			-300.0f );
		// position of the plug
		vec3f curPlugPos = m_pCork->GetParent()->GetPositionMatrix().axis_pos;
		// vector from the plug
		// to the position it needs to end at
		vec3f vecToPluggedPos = _pluggedPos - curPlugPos;

		// Get the new position to move the plug
		vec3f newPlugPos = curPlugPos + vecToPluggedPos * (fDT * 20.0f);					
		// Move the Plug
		m_pCork->GetParent()->SetLocalPos( &newPlugPos );					

		// update the AABB
		((Box*)m_pCork->GetParent()->GetCollidableObject())->SetCentorid( m_pCork->GetPos2DClean() );

		// If the plug is close enough to the correct position
		// Plug that shit!
		if( dot_product( vecToPluggedPos, vecToPluggedPos ) < 1.0f )
		{
			_pluggedPos = curPlugPos + vec3f( 0.0f, 15.0f, 0.0f );
			m_pCork->GetParent()->SetLocalPos( &_pluggedPos );
			m_bCorked = true;
			m_bIsGettingCorked = false;
			// Rotate the bridge
			if( m_pPartner )
			{
				((CComponent_SteamPipeCollision*)m_pPartner->GetComponent(ECOMP_COLLISION))->SetCorked( true );
			}
			// remove the plug from the KdTree
			m_pCork->GetParent()->SetCollidableObject( NULL );
		}
		((CComponent_CorkCollision*)m_pCork)->SetCorked(true);
	}
}