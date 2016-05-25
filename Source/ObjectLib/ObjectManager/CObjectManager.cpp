#include "CObjectManager.h"
#include "../Objects/IBaseObject.h"
#include "../Object Factory/CObjectFactory.h"
#include "../../Utilities/KdTree/KdTree.h"
#include "../../AnimationLib/State.h"

CObjectManager* CObjectManager::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CObjectManager
// Purpose: Default Constructor
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CObjectManager::CObjectManager ()
{
	m_pPlayer = nullptr;
	m_bFlushKdTree = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~CObjectManager
// Purpose: Default Destructor
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CObjectManager::~CObjectManager ()
{
	Shutdown ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns an instance.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CObjectManager* CObjectManager::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CObjectManager();
	}
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: DeleteInstance
// Purpose: Deletes the instance.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::DeleteInstance ()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Init
// Purpose: Inits all information for ObjectManager
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::Init ()
{
	m_PhysicsEnvironment.SetGravity( vec3f( 0.0f, -20.0f, 0.0 ) );
	m_pObjectFactory.Init();
	AABB sceneBounds;
	sceneBounds.SetMaxPoint (vec3f (40000.0f, 4000.0f, 30.0f));
	sceneBounds.SetMinPoint (vec3f (-1000.0f, -4000.0f, -30.0f));
	CKdTree::Init (sceneBounds);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown
// Purpose: Shuts down the ObjectManager
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::Shutdown ()
{
	//remove all objects and shutdown the kd tree
	ClearAll ();
	//m_pComponentManager.Clear();
	m_pObjectFactory.Shutdown();
	m_PhysicsEnvironment.Clear();
	CKdTree::Shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: Updates all objects in the ObjectManager
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::Update ( float fElapsedTime )
{
	//TODO:
	// Add a create object next frame function so objects dont render before they get updated
	if( m_bFlushKdTree )
	{
		CKdTree::FlushKDTree();
		m_bFlushKdTree = false;
	}

	//update all objects
	m_PhysicsEnvironment.Update( fElapsedTime );
	m_pComponentManager.Update( fElapsedTime );
	CKdTree::Update();

	// Remove all the objects that have been queued to be removed.
	for( list<IBaseObject*>::iterator itr = m_ObjectsToRemove.begin(); itr != m_ObjectsToRemove.end(); itr++)
	{
		RemoveObject( *itr);
	}
	m_ObjectsToRemove.clear();

}

void CObjectManager::Render()
{
	//Renders any component that has particles//If problems come up talk to the particleman
	m_pComponentManager.Render();
	////////////////////////////////////////////
	std::list< IBaseObject* >::iterator itrObj =  m_ObjectList.begin();

	while( itrObj != m_ObjectList.end() )
	{
		(*itrObj)->Render();
		itrObj++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateObject
// Purpose: Creates an object based on the Type of object and puts that object in the ObjectManager
//			and puts the object in the kd tree
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CObjectManager::CreateObject( eOBJECT_TYPE eType, D3DXMATRIX _ObjectMatrix, char _LevelID, IBaseObject* _partner )
{
	//add pObject to m_vAllObjects
	IBaseObject* pNewObject = m_pObjectFactory.CreateObject( eType, _ObjectMatrix, _partner );
	pNewObject->SetLevelID( _LevelID );
	// Set its matrix
	pNewObject->SetLocalMat( (matrix4f*)&_ObjectMatrix );
	pNewObject->GetFrame().Update();
	pNewObject->GetWorldMat();

	if (!pNewObject)
	{
		LOG(LOG_DEBUG, "CObjectManager::CreateObject", "Failed to create object of type: %d", eType);
		return nullptr;
	}

	if( OBJ_PLAYER == eType )
	{
		m_pPlayer = pNewObject;
		// This is actually setting the matrix not the position.
		matrix4f _LocalMat = (*pNewObject->GetLocalMat());
		m_pPlayer->SetLocalMat( &_LocalMat);
	}
	else if( OBJ_PLAYER_ARM == eType)
	{
		m_pPlayerArm = pNewObject;
		m_pPlayerArm->SetLocalMat( pNewObject->GetLocalMat() );
		m_pPlayerArm->GetWorldMat();
	}
	else if( OBJ_SLUDGE == eType )
	{
		m_pSludge = pNewObject;
		m_pSludge->SetLocalMat( (matrix4f*)_ObjectMatrix.m );
		m_pSludge->GetWorldMat();
	}
	else if( OBJ_BOSS == eType )
	{
		m_pSludgesShip = pNewObject;
		m_pSludgesShip->SetLocalMat( (matrix4f*)_ObjectMatrix.m );
		m_pSludgesShip->GetWorldMat();
	}
	else if(OBJ_MONSTER == eType)
	{
		matrix4f _LocalMat = pNewObject->GetFrame().GetLocalMat();
#if _ANIMATIONS
		_LocalMat.rotate_y_pre( D3DXToRadian( 90.0f ) );
#endif
		pNewObject->SetLocalMat( &_LocalMat );
	}
	else if( eType == OBJ_PLUG )
	{
		vec3f oldPos = vec3f(_ObjectMatrix._41, _ObjectMatrix._42, _ObjectMatrix._43 );
		//pNewObject->SetLocalMat( (matrix4f*)&_ObjectMatrix );
		matrix4f mat; mat.make_identity();
		mat.rotate_z_pre( D3DXToRadian( -90.0f ));
		mat.axis_pos = oldPos;
		pNewObject->SetLocalMat( &mat );
	}
	else if( eType != OBJ_CRATE && eType != OBJ_CORK && eType != OBJ_LAND_MESHEFFECT)
	{
		// Set its matrix
		pNewObject->SetLocalMat( (matrix4f*)&_ObjectMatrix );
		pNewObject->GetFrame().Update();
		pNewObject->GetWorldMat();
	}
	else
	{
		// add the object to the item list
		m_ItemList.push_back(pNewObject);
	}
	m_ObjectList.push_back( pNewObject );
	CKdTree::MasterObjInsert (pNewObject );

	return pNewObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateAnimatedBGObject
// Purpose: Adds the animated background object to the ObjectManager and adds a reference to that object
// Original Author: Rueben Massey
// Creation Date: 7/21/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CObjectManager::CreateAnimatedBGObject ( eOBJECT_TYPE eType, D3DXMATRIX _ObjectMatrix, const char* _szRenderNode, State* _AnimationState, char _levelId )
{
	//add pObject to m_vAllObjects
	IBaseObject* pNewObject = m_pObjectFactory.CreateAnimatedBGObject( eType, _ObjectMatrix, _szRenderNode, _AnimationState);

	if (!pNewObject)
	{
		LOG(LOG_DEBUG, "CObjectManager::CreateObject", "Failed to create object of type: %d", eType);
		return nullptr;
	}

	// Set its matrix
	pNewObject->SetLocalMat( (matrix4f*)&_ObjectMatrix );
	pNewObject->GetFrame().Update();
	pNewObject->GetWorldMat();

	pNewObject->SetLevelID( _levelId );

	m_ObjectList.push_back( pNewObject );
	CKdTree::MasterObjInsert (pNewObject );
	
	return pNewObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: AddObject
// Purpose: Creates a new object from the ObjectFactory and fills out the Objects CollisionVolume
//			before adding it to the ObjectManager and then to the KD Tree.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CObjectManager::AddObject ( CCollisionVolume* _pVolume, eOBJECT_TYPE eType)
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity (&identity);
	IBaseObject* pNewObject = m_pObjectFactory.CreateObject( eType, identity );

	pNewObject->SetCollidableObject (_pVolume);

	// add it to the item list if its an item
	if( OBJ_CORK == eType || OBJ_CRATE == eType )
	{
		m_ItemList.push_back( pNewObject );
	}
	m_ObjectList.push_back( pNewObject );
	CKdTree::MasterObjInsert (pNewObject );

	return pNewObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: AddObjectToRemove
// Purpose: Queues the Object to be removed after update.
// Original Author: Andy Madruga
// Creation Date: 5/17/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::AddObjectToRemove ( IBaseObject* pObject)
{
	m_ObjectsToRemove.push_back( pObject );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions
// Purpose: Removes the object from the ObjectManager and removes a reference to that object
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::RemoveObject ( IBaseObject* pObject )
{
	// If object removed is the player set pointer to it to null.
	if( pObject->GetType() == OBJ_PLAYER )
		m_pPlayer = NULL;
	// If object removed is the arm set pointer to it to null.
	if( pObject->GetType() == OBJ_PLAYER_ARM )
	{
		if( pObject->GetFrame().GetParent() )
		{
			 pObject->GetFrame().GetParent()->RemoveChild( (Node*)& pObject->GetFrame() );
		}
		m_pPlayerArm = NULL;
	}

	// remove pObject from m_vAllObjects
	CKdTree::MasterObjRemove ( pObject );
	m_ObjectList.remove( pObject );
	m_ItemList.remove( pObject );
	m_pObjectFactory.ReleaseObject( pObject );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: RemoveItem
// Purpose: Removes the object from the ObjectManager and removes a reference to that object
// Original Author: Rueben Massey
// Creation Date: 5/15/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::RemoveItem( IBaseObject* pObject)
{
	m_ItemList.remove( pObject );
	m_pObjectFactory.ReleaseObject( pObject );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ClearAll
// Purpose: Clears all objects from the ObjectManger and the references to thoses objects
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::ClearAll ()
{
	// clear all objects
	std::list< IBaseObject* >::iterator itrObject = m_ObjectList.begin();
	while( itrObject != m_ObjectList.end() )
	{
		// Some kind of removal happens here.
		CKdTree::MasterObjRemove ( (*itrObject) );
		m_pObjectFactory.ReleaseObject( (*itrObject) );
		itrObject++;
	}
	CKdTree::Shutdown();
	m_pComponentManager.Clear();
	m_ObjectList.clear();
	m_ItemList.clear();

	m_pPlayer = m_pPlayerArm = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ResetAllHazardsandMoveableObjects
// Purpose: Removes All the Hazards in the Level and Reloads them.
// Original Author: Andy Madruga
// Creation Date: 7/21/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::ResetAllHazardsandMoveableObjects(void)
{
	// Loop through the Object List.
	for( list<IBaseObject*>::iterator itr = m_ObjectList.begin(); itr != m_ObjectList.end(); itr++)
	{
		IBaseObject* pObject = (*itr);
		// If any of these objects are hazards... Lets Remove them.
		if( pObject->GetType() == OBJ_CRUSHER || pObject->GetType() == OBJ_CRUSHERBASE || pObject->GetType() == OBJ_BRIDGE ||
			pObject->GetType() == OBJ_DOOR || pObject->GetType() == OBJ_DOORBUTTON || pObject->GetType() == OBJ_BOXBUTTON ||
			pObject->GetType() == OBJ_PIT || pObject->GetType() == OBJ_PIPE_STEAM || pObject->GetType() == OBJ_PIPE_INTAKE ||
			pObject->GetType() == OBJ_PIPE_SPAWN || pObject->GetType() == OBJ_PIPE_BOX || pObject->GetType() == OBJ_POOL || 
			pObject->GetType() == OBJ_OUTLET || pObject->GetType() == OBJ_WALL_BREAKABLE || pObject->GetType() == OBJ_WALLCHUNK || 
			pObject->GetType() == OBJ_CRATE || pObject->GetType() == OBJ_CORK || pObject->GetType() == OBJ_GOO || pObject->GetType() == OBJ_PLAYER_DEATH_EFFECT ||
			pObject->GetType() == OBJ_ANIMATED_BG )
		{
			AddObjectToRemove( pObject );
		}
	}

	// Remove all the objects that have been queued to be removed.
	for( list<IBaseObject*>::iterator itr = m_ObjectsToRemove.begin(); itr != m_ObjectsToRemove.end(); itr++)
	{
		RemoveObject( *itr);
	}
	m_ObjectsToRemove.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ClearObjectByLevel
// Purpose: Removes All the objects that correspond to the level id passed in.
// Original Author: Andy Madruga
// Creation Date: 8/2/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectManager::ClearObjectByLevel( char _id )
{
		// Loop through the Object List.
	for( list<IBaseObject*>::iterator itr = m_ObjectList.begin(); itr != m_ObjectList.end(); itr++)
	{
		IBaseObject* pObject = (*itr);
		// If the id of the object is the same as the id passed in remove it.
		if( _id == pObject->GetLevelID() )
			AddObjectToRemove( pObject );
	}

	m_bFlushKdTree = true;

	// Remove all the objects that have been queued to be removed.
	for( list<IBaseObject*>::iterator itr = m_ObjectsToRemove.begin(); itr != m_ObjectsToRemove.end(); itr++)
	{
		RemoveObject( *itr);
	}
	m_ObjectsToRemove.clear();
}

void CObjectManager::ResetBoss( void )
{
	m_pSludge = NULL;
	m_pSludgesShip = NULL;
}