/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CTrajectory
//
// Purpose: Manage the trajectory line created when the player
//			has an object
//
// Original Author: Rueben Massey
//
// Creation Date:7/8/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CTrajectory.h"
#include "..//Utilities/KdTree/KdTree.h"
#include "..//ObjectLib/ObjectManager/CObjectManager.h"
#include "..//RenderEngine/TextureManager.h"

CTrajectory::CTrajectory()
{
	m_CollisionVolume = nullptr;
}

CTrajectory::~CTrajectory()
{
	delete m_CollisionVolume;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Initialize()
// Purpose: Initializes data members
// Original Author: Rueben Massey
// Creation Date: 7/11/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrajectory::Initialize( )
{
	// create a new box
	m_CollisionVolume = new Box (*(AABB*)MeshManager::GetInstance()->GetCollisionVolume( "Crate" ));
	// Copy it over to our data member


	m_PosMat.make_identity();
	m_VecToCursor = vec2f( 0.0f, 0.0f );
	m_bHasCollided = false;
	m_fPositionTimer = 0.0f;
	m_fTimeOut = 0.0f;
	m_nTrajectoryCounter = 0;
	m_nFrameCounter = 0;

	m_pTrajectoryTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/BigTrajectoryDot.png" );

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: No purpose yet...
// Original Author: Rueben Massey
// Creation Date: 7/11/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrajectory::Update( float fDT )
{
	tPhyObject* tPhysicsObject = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( m_StartPos, 1.0f );
	tPhysicsObject->ApplyUniformForce( vec3f( m_VecToCursor.x, m_VecToCursor.y, 0.0f ) * THROW_SPEED, 1.0f );

	m_bHasCollided = false;

	while( !m_bHasCollided )
	{
		float timestep = 1.0f/60.0f;
		m_fPositionTimer += timestep;
		m_fTimeOut += timestep;

		// simulate movement
		CObjectManager::GetInstance()->m_PhysicsEnvironment.SimulateObject( tPhysicsObject, timestep );

		m_CollisionVolume->SetWorld( tPhysicsObject->GetCentroid() );

		if( m_fPositionTimer > .05f )
		{
			m_TrajectoryPositionVector.push_back( tPhysicsObject->GetCentroid() );
			m_fPositionTimer = 0.0f;
			std::vector <CSceneObject*> NearestObjects;
			// create a unsigned int that will tell the kd tree what you want put in the return vector
			// this uses bit wise operations so you can have more then one object returned
			// use the return flags enum from the kd tree so you know what you can get back
			int ReturnParams = (1<<RFLAG_BOX) | (1<<RFLAG_AABB);

			CKdTree::m_pMainTree->debugLeafTest.clear();

			// create a scene object
			CSceneObject simulatedObj;
			// set its collision volume to the one we created
			simulatedObj.SetCollidableObject( m_CollisionVolume );		

			CKdTree::m_pMainTree->debugLeafTest.clear();

			// call the kd tree and get the near objects
			CKdTree::GetNearObjects( &simulatedObj, PSFLAG_BOX, ReturnParams, NearestObjects/*, ReturnBody, ReturnObjects*/);

			CKdTree::m_pMainTree->debugLeafTest.clear();

			for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
			{
				// get the collision component of the object
				CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));

				if( curCollision )
				{
					if( curCollision->GetParent()->GetType() == OBJ_PLAYER || curCollision->GetParent()->GetType() == OBJ_PLAYER_ARM )
						continue;

					if( curCollision->GetParent()->GetType() == OBJ_DOOR )
					{
						if( ((CComponent_DoorCollision*)curCollision)->IsDoorOpen() )
						{
							continue;
						}
					}
				}
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
						CheckDynamicCollisions( curVolume, fDT );
					}
					else if( BT_STATIC == curVolume->GetBodyType() )
					{
						CheckStaticCollisions( curVolume, fDT );
					}
				}
			}
		}
		if( m_fTimeOut > 10.0f )
		{
			m_fTimeOut = 0.0f;
			break;
		}
	}
	m_fTimeOut = 0.0f;
	CObjectManager::GetInstance()->m_PhysicsEnvironment.RemovePhyObject( tPhysicsObject );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: UpdateLine()
// Purpose: Update the position vector
// Original Author: Rueben Massey
// Creation Date: 7/14/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrajectory::UpdateLine( void )
{
	unsigned int mag = (unsigned int)m_VecToCursor.magnitude();
	vec3f laservec = m_StartPos;
	for( unsigned int posindex = 0; posindex < mag; ++posindex )
	{
		m_TrajectoryPositionVector.push_back( laservec );
		vec2f vectomoveon = m_VecToCursor.normalize();
		laservec.x += vectomoveon.x;
		laservec.y += vectomoveon.y;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 7/9/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrajectory::CheckDynamicCollisions( CCollisionVolume* pCurVolume, float fDT )
{
	// Variables to catch returns
	vec2f Direction;
	vec2f CP;
	unsigned int numPoints;

	// Check collisions
	// based on the current objects collision volume
	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( m_CollisionVolume->BoxToAABB( *((AABB*)pCurVolume), CP, Direction ) )
			{
				m_bHasCollided = true;
			}
			break;
		}
	case VMT_BOX:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((Box*)pCurVolume)->BoxToBox( m_CollisionVolume, &CP, numPoints, Direction ) )
			{
				m_bHasCollided = true;
			}
			break;
		}
	default:
		break;
	};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckStaticCollisions()
// Purpose: Check collisions with all static objects
// Original Author: Rueben Massey
// Creation Date: 7/9/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrajectory::CheckStaticCollisions( CCollisionVolume* pCurVolume, float fDT )
{
	vec2f Direction;
	vec2f CP;

	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			// handle crate collision
			// with world AABB
			if ( m_CollisionVolume->BoxToAABBWorld( *(AABB*)pCurVolume,CP, Direction ) )
			{
				m_bHasCollided = true;
			}
			break;
		}
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render()
// Purpose: Render the trajectory line
// Original Author: Rueben Massey
// Creation Date: 7/9/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrajectory::Render( )
{
	for( unsigned int posindex = 0; posindex < m_TrajectoryPositionVector.size(); ++posindex )
	{
		D3DXMATRIX posmat, scalemat;
		D3DXMatrixIdentity( &posmat );
		D3DXMatrixIdentity( &scalemat );
		posmat._41 = m_TrajectoryPositionVector[posindex].x;
		posmat._42 = m_TrajectoryPositionVector[posindex].y;
		posmat._43 = m_TrajectoryPositionVector[posindex].z;

		D3DXMatrixScaling( &scalemat, 10.0f, 10.0f, 0.0f );
		if( 0 == (posindex % 4) - m_nTrajectoryCounter )
		{
			D3DXMatrixScaling( &scalemat, 20.0f, 20.0f, 0.0f );
		}
		D3DXMatrixMultiply( &posmat, &scalemat, &posmat );

		RenderEngine::AddGlowToDraw( posmat, m_pTrajectoryTexture );
	}
	++m_nFrameCounter;
	if( m_nFrameCounter > 5 )
	{
		m_nFrameCounter = 0;
		++m_nTrajectoryCounter;
		if( 4 == m_nTrajectoryCounter )
			m_nTrajectoryCounter = 0;
	}
	// clear the positions after we are done rendering
	m_TrajectoryPositionVector.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: RenderLine()
// Purpose: Render the Laser line
// Original Author: Rueben Massey
// Creation Date: 7/14/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTrajectory::RenderLine( void )
{
	for( unsigned int posindex = 0; posindex < m_TrajectoryPositionVector.size(); ++posindex )
	{
		D3DXMATRIX posmat, scalemat;
		D3DXMatrixIdentity( &posmat );
		D3DXMatrixIdentity( &scalemat );
		posmat._41 = m_TrajectoryPositionVector[posindex].x;
		posmat._42 = m_TrajectoryPositionVector[posindex].y;
		posmat._43 = m_TrajectoryPositionVector[posindex].z;

		D3DXMatrixScaling( &scalemat, 10.0f, 10.0f, 0.0f );
		D3DXMatrixMultiply( &posmat, &scalemat, &posmat );

		RenderEngine::AddGlowToDraw( posmat, /*TextureManager::GetInstance()->GetTexture( */m_pTrajectoryTexture /*)*/ );
	}
	// clear the positions after we are done rendering
	m_TrajectoryPositionVector.clear();
}