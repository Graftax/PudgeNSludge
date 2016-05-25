/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_OutletCollision
//
// Purpose: Handle all collisions for the plug object
//
// Original Author: Rueben Massey
//
// Creation Date: 6/4/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_OutletCollision.h"

#include "../PS/config.h"
#include "Component_Emitter.h"
#include "CComponent_PlugCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../AudioLib/AudioManager.h"
#include "../EffectsLib/ParticleManager.h"
#include "../EffectsLib/Emitter.h"
#include "../EffectsLib/Effects.h"
#include "../Utilities/KdTree/KdTree.h"

CComponent_OutletCollision::CComponent_OutletCollision()
{
	m_pAssociatedObj = nullptr;
	m_pCollisionObject = nullptr;
	m_bIsPlugged = false;
	m_nChannel = -1;
	this->CleanRemoveVelocity();

}

CComponent_OutletCollision::~CComponent_OutletCollision()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Initialize()
// Purpose: Initialize all data members
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_OutletCollision::Initialize( )
{
	CComponent_Collision::Initialize();
	m_bIsPlugged = false;
	m_nDirection = -1;
	m_nChannel = -1;
	m_bIsGettingPlugged = false;
	m_pPlug = nullptr;
	this->CleanRemoveVelocity();
	m_pAssociatedObj = nullptr;
	m_pCollisionObject = nullptr;
	m_OutletAABB.SetMaxPoint(  vec2f( 0.0f, 61.0f ) );
	m_OutletAABB.SetMinPoint(  vec2f( -122.0f, -61.0f ) );
	m_OutletAABB.SetBodyType( BT_DYNAMIC );
	m_OutletAABB.SetVolumeType( VMT_AABB );

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: Update all data members
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_OutletCollision::Update( float fDT )
{
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
	if( m_bIsGettingPlugged )
	{
		MovePlug( fDT );
	}

	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	m_OutletAABB.SetWorldPoint (NewCenter);
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (NewCenter);

	/*D3DXMATRIX mat;
	mat._41 = m_OutletAABB.GetMax2DTransformed().x;
	mat._42 = m_OutletAABB.GetMax2DTransformed().y;
	mat._43 = -600.0f;
	DebugShapes::RenderSphere (mat);

	mat._41 = m_OutletAABB.GetMin2DTransformed().x;
	mat._42 = m_OutletAABB.GetMin2DTransformed().y;
	mat._43 = -600.0f;
	DebugShapes::RenderSphere (mat);

	mat._41 = m_OutletAABB.GetMin2DTransformed().x;
	mat._42 = m_OutletAABB.GetMax2DTransformed().y;
	mat._43 = -600.0f;
	DebugShapes::RenderSphere (mat);

	mat._41 = m_OutletAABB.GetMax2DTransformed().x;
	mat._42 = m_OutletAABB.GetMin2DTransformed().y;
	mat._43 = -600.0f;
	DebugShapes::RenderSphere (mat);*/

	CheckPlugCollisions( fDT );
	CheckCollisions( fDT );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown()
// Purpose: clean up memory
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_OutletCollision::Shutdown(void)
{	
	m_bIsPlugged = false;
	m_nDirection = -1;
	m_nChannel = -1;
	m_bIsGettingPlugged = false;
	m_pPlug = nullptr;
	this->CleanRemoveVelocity();
	m_pAssociatedObj = nullptr;
	m_pCollisionObject = nullptr;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision with the steampipe
//			and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_OutletCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects );

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
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
// Function Name: CheckPlugCollisions()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/16/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_OutletCollision::CheckPlugCollisions( float fDT )
{

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLUG);

	CSceneObject _door;
	_door.SetCollidableObject( &m_OutletAABB );

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( &_door, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects );

	// Variables to catch returns
	vec2f Direction;
	vec2f CP;
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{			
			if( m_bIsPlugged )
			{
				break;
			}
			if( ((Box*)curVolume)->BoxToAABB( m_OutletAABB, CP, Direction ) )
			{					
				// We have collided
				// get the plug's collision component
				// so we can move it
				m_pPlug = curCollision;
				((CComponent_PlugCollision*)m_pPlug)->SetPlugged( true );
				// set this flag
				// so we can move the plug
				// in update
				m_bIsGettingPlugged = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_OutletCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
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
				// update the AABB
				((AABB*)pCurCollision->GetParent()->GetCollidableObject())->SetWorldPoint( pCurCollision->GetPos2DClean() );
				// set the player to grounded
				// if he is on top of us
				if( OBJ_PLAYER == pCurCollision->GetParent()->GetType() )
				{
					if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
					{
						((CComponent_PlayerCollision*)pCurCollision)->SetCollidingGroundAlone( true );
					}
				}
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
// Function Name: MovePlug()
// Purpose: move the plug to the correct position
// Original Author: Rueben Massey
// Creation Date: 6/14/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_OutletCollision::MovePlug( float fDT )
{
	if( m_pPlug )
	{
		// Place Plug in the proper position
		// Set the plug to plugged
		vec3f _pluggedPos = vec3f(  m_pCollisionObject->GetMin2DTransformed().x - 65.0f, 
			m_pCollisionObject->GetMax2DTransformed().y - 75.0f, 
			-300.0f );
		// position of the plug
		vec3f curPlugPos = m_pPlug->GetParent()->GetPositionMatrix().axis_pos;
		// vector from the plug
		// to the position it needs to end at
		vec3f vecToPluggedPos = _pluggedPos - curPlugPos;

		// Get the new position to move the plug
		vec3f newPlugPos = curPlugPos + vecToPluggedPos * (fDT * 20.0f);					
		// Move the Plug
		m_pPlug->GetParent()->SetLocalPos( &newPlugPos );					

		// update the AABB
		((AABB*)m_pPlug->GetParent()->GetCollidableObject())->SetWorldPoint( m_pPlug->GetPos2DClean() );

		// If the plug is close enough to the correct position
		// Plug that shit!
		if( dot_product( vecToPluggedPos, vecToPluggedPos ) < 1.0f )
		{
			_pluggedPos = curPlugPos + vec3f( 20.0f, 0.0f, 0.0f );
			m_pPlug->GetParent()->SetLocalPos( &_pluggedPos );
			m_bIsPlugged = true;
			m_bIsGettingPlugged = false;
			// Rotate the bridge
			if( m_pAssociatedObj )
			{
				//((CComponent_BridgeCollision*)m_pAssociatedObj->GetComponent (ECOMP_COLLISION))->Initialize();
				((CComponent_BridgeCollision*)m_pAssociatedObj->GetComponent (ECOMP_COLLISION))->SetIsActivated( true );
			}
			// remove the plug from the KdTree
			m_pPlug->GetParent()->SetCollidableObject( NULL );
		}
	}
}