#include "CObjectManager.h"
#include "../Objects/IBaseObject.h"
#include "../Object Factory/CObjectFactory.h"
#include "../../Utilities/KdTree/KdTree.h"

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
	sceneBounds.SetMaxPoint (vec3f (3000.0f, 800.0f, 30.0f));
	sceneBounds.SetMinPoint (vec3f (-1000.0f, -800.0f, -30.0f));
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
IBaseObject* CObjectManager::CreateObject( eOBJECT_TYPE eType, D3DXMATRIX _ObjectMatrix, IBaseObject* _partner )
{
	//add pObject to m_vAllObjects
	IBaseObject* pNewObject = m_pObjectFactory.CreateObject( eType, _ObjectMatrix, _partner );

	if( OBJ_PLAYER == eType )
	{
		m_pPlayer = pNewObject;
		// This is actually setting the matrix not the position.
		matrix4f _LocalMat = (*pNewObject->GetLocalMat());
#if _ANIMATIONS
		_LocalMat.rotate_y_pre( D3DXToRadian( 90.0f) );
#else
		_LocalMat.rotate_y_pre( D3DXToRadian( -90.0f) );
#endif
		m_pPlayer->SetLocalMat( &_LocalMat);
	}
	else if( OBJ_PLAYER_ARM == eType)
	{
		m_pPlayerArm = pNewObject;
		m_pPlayerArm->SetLocalMat( pNewObject->GetLocalMat() );
		m_pPlayerArm->GetWorldMat();
	}
	else if(OBJ_MONSTER == eType)
	{
		matrix4f _LocalMat = pNewObject->GetFrame().GetLocalMat();
#if _ANIMATIONS
		_LocalMat.rotate_y_pre( D3DXToRadian( 90.0f ) );
#endif
		pNewObject->SetLocalMat( &_LocalMat );
	}
	else if( OBJ_PULL_MESHEFFECT == eType || OBJ_PUSH_MESHEFFECT == eType || OBJ_JUMP_MESHEFFECT == eType)
	{
		m_pEffect = pNewObject;
		matrix4f _LocalMat = (*pNewObject->GetLocalMat());
		_LocalMat.rotate_y_pre( D3DXToRadian( -90.0f) );
		m_pEffect->SetLocalMat( &_LocalMat);
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
	else if(eType == OBJ_BIGHIT_MESHEFFECT || eType == OBJ_LITTLEHIT_MESHEFFECT)
	{
		m_pEffect = pNewObject;
		matrix4f _LocalMat = (*pNewObject->GetLocalMat());
		_LocalMat.rotate_x_pre( D3DXToRadian( 90.0f) );
		m_pEffect->SetLocalMat( &_LocalMat);
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
	// remove pObject from m_vAllObjects
	m_ObjectList.remove( pObject );
	m_ItemList.remove( pObject );
	m_pObjectFactory.ReleaseObject( pObject );
	CKdTree::MasterObjRemove ( pObject );
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
}