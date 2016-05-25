//////////////////////////////////////////////////////////////////////////////////////
// Filename:	KdTree.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds all the information for building and maintaining a KdTree
//////////////////////////////////////////////////////////////////////////////////////
#define _BROKEN_AUDIO
#include "KdTree.h"
#include "KdTreeNodes.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../CollisionLib/CollisionLib.h"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

CKdTree* CKdTree::m_pMainTree = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BuildTree():	Takes in a const AABB reference (_abBounds). This function starts building the Kd Tree.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::BuildTree (const AABB& abBounds)
{
	// create an initial leaf
	CLeafNode* pInitLeaf = new CLeafNode ();

	// set the bounds of the scene
	m_pMainTree->m_abSceneBounds = abBounds;
	// set the initial leafs bounds
	pInitLeaf->SetBounds ( abBounds );

	// start building the main tree
	m_pMainTree->Build ( m_pMainTree, pInitLeaf );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build():	Takes in a CKdTree pointer and a CLeafNode pointer (_pTree, _pLeaf). This function will build the
//			Kd Tree based on the scene bounds, this function will try to construct each leaf based on the size
//			of the largest object (or any size we decided). All leafs created are leafs that fit within the scene bounds.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::Build ( CKdTree* pTree, CLeafNode* pLeaf )
{
	// this follows the ED2 Build Function
	unsigned char ucLargestAxis = 0;

	vec3f vAxis = (pLeaf->GetBounds ().GetMaxPoint3D() - pLeaf->GetBounds ().GetMinPoint3D());

	float fBestLength = 0.0f;
	for (size_t unPlane = 0; unPlane < 3; ++unPlane)
	{
		float fLength = vAxis.v[unPlane];
		if (fLength > fBestLength)
		{
			fBestLength = fLength;
			ucLargestAxis = unPlane;
		}
	}

	if (vAxis.v[ucLargestAxis] < 200 * 2)// this needs to be the size of our largest object
	{
		pTree->GetLeaves ().push_back (pLeaf);

		if (!pLeaf->GetParent ())
		{
			pTree->SetTreeRoot (pLeaf);
		}
		else
		{
			if (!pLeaf->GetParent ()->GetLeftChild ())
			{
				pLeaf->GetParent ()->SetLeftChild (pLeaf);
			}
			else if (!pLeaf->GetParent ()->GetRightChild ())
			{
				pLeaf->GetParent ()->SetRightChild (pLeaf);
			}
		}

		return;
	}

	CInternalNode* pInternalLeaf = new CInternalNode ();
	pInternalLeaf->SetParent (pLeaf->GetParent ());
	pInternalLeaf->SetAxis (ucLargestAxis);
	pInternalLeaf->SetPlaneOffset (pLeaf->GetBounds ().GetMinPoint3D().v [ucLargestAxis] + vAxis.v [ucLargestAxis] * 0.5f);
	
	if (!pInternalLeaf->GetParent ())
	{
		pTree->SetTreeRoot (pInternalLeaf);
	}
	else
	{
		if (!pInternalLeaf->GetParent ()->GetLeftChild ())
		{
			pInternalLeaf->GetParent ()->SetLeftChild (pInternalLeaf);
		}
		else if (!pInternalLeaf->GetParent ()->GetRightChild ())
		{
			pInternalLeaf->GetParent ()->SetRightChild (pInternalLeaf);
		}
	}

	CLeafNode* pNewLeaf = new CLeafNode ();
	AABB abFirstHalf;
	AABB abSecondHalf;

	abFirstHalf = pLeaf->GetBounds ();
	abSecondHalf = pLeaf->GetBounds ();
	vec3f firstHalfAxis = abFirstHalf.GetMaxPoint3D();
	vec3f secondHalfAxis = abSecondHalf.GetMinPoint3D();

	firstHalfAxis.v [ucLargestAxis] = pInternalLeaf->GetPlaneOffset ();
	secondHalfAxis.v [ucLargestAxis] = pInternalLeaf->GetPlaneOffset ();
	abFirstHalf.SetMaxPoint (firstHalfAxis);
	abSecondHalf.SetMinPoint (secondHalfAxis);

	pNewLeaf->SetBounds (abSecondHalf);
	pNewLeaf->SetParent (pInternalLeaf);

	pLeaf->SetBounds (abFirstHalf);
	pLeaf->SetParent (pInternalLeaf);

	Build (pTree, pLeaf);
	Build (pTree, pNewLeaf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateObject():	Takes in a SceneObject pointer (_pObject). This function updates the object by removing it from the
//					Kd Tree and then placing it back in the Kd Tree.  This function is not cheap and should not be used
//					on objects that dont move.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::UpdateObject ( CSceneObject* pObject )
{
	// check if the object still has a bounding volumne
	//if (!pObject->GetCollidableObject())
	//{
	//	MasterObjRemove (pObject);
	//	return;
	//}

	// make sure the object is contained and is not in the return vector
	pObject->SetInReturnVec (false);
	//if (pObject->IsContained ())
	//{
	//	return;
	//}

	// remove the object from all its leafs
	pObject->RemoveFromLeaves ();

	tTraverseData data;

	// setup the data that will be needed to traverse the tree and place the object back in the tree
	data.m_LeafFunc = InsertObjInLeaf; 
	switch (pObject->GetCollidableObject()->GetVolumeType())
	{
	case VMT_SPHERE:
		data.m_SplitFunc = CKdTree::SphereLeafSplit;
		break;
	case VMT_CAPSULE:
		data.m_SplitFunc = CKdTree::CapsuleLeafSplit;
		break;
	case VMT_AABB:
		data.m_SplitFunc = CKdTree::AABBLeafSplit;
		break;
	case VMT_BOX:
		data.m_SplitFunc = CKdTree::BoxLeafSplit;
		break;
	default:
		return;
	}

	data.m_pVoidData = pObject;

	// traverse the tree
	Traverse (&data);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():	Takes in an AABB (_abBounds).  Creates the kd tree based on the passed 
//			in bounbing box.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::Init ( const AABB& abBounds )
{
	// init and start building the tree
	m_pMainTree = new CKdTree(); 
	m_pMainTree->BuildTree (abBounds);
	m_pMainTree->ObjManager = CObjectManager::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CKdTree():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKdTree::CKdTree () : m_pTreeRoot (NULL), m_usObjIndex (0)
{
	m_vObjects.resize (100, NULL);
	m_usObjIndex = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	Destories the kd tree and cleans up all memory
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::Shutdown ()
{
	// destory the tree and delete all leaves in the tree
	if (!m_pMainTree)
	{
		return;
	}

	m_pMainTree->m_lsLeafs.clear();;
	m_pMainTree->m_vObjects.clear();
	m_pMainTree->m_usObjIndex = 0;

	delete m_pMainTree->m_pTreeRoot;
	delete m_pMainTree;

	m_pMainTree = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	Updates all objects inside the kd tree so they are in the correct 
//				leaves.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::Update ()
{
	// update every object in the object manager
	std::list< IBaseObject* > objects = CObjectManager::GetInstance()->GetAllObjects();
	for (std::list< IBaseObject* >::iterator itr = objects.begin(); itr != objects.end(); itr++)
	{
		if ((*itr)->GetCollidableObject() && (*itr)->GetCollidableObject()->GetBodyType() != BT_STATIC)
		{
			m_pMainTree->UpdateObject (*itr);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Traverse():	THIS IS A PRIVATE FUNCTION AND YOU SHOULD NOT CALL IT. This function is the starting point for 
//				traverseing through the tree
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::Traverse( tTraverseData* pData )
{
	// traverse the tree
	if (m_pTreeRoot)
	{
		m_pTreeRoot->Traverse (pData);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Traversal():	THIS IS A PRIVATE FUNCTION AND YOU SHOULD NOT CALL IT. This function traverses through the kd tree
//				and fills out the necessary information.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::Traversal ( CTreeNode* pNode, tTraverseData* pData )
{
	// this is where the magic happenes

	// if the tree node is an internal leaf then we need to split down one of or both sides of the tree
	if (pNode->GetNodeType() == TREE_INTERNAL)
	{
		if (pData->m_SplitFunc != NULL)
			return pData->m_SplitFunc ((CInternalNode*)pNode, pData);
	}

	// if the tree node is a leaf then we need to do stuff to that leaf
	if (pNode->GetNodeType() == TREE_LEAF)
	{
		// is there a leaf func pointer if so call it and let it do its thing
		CLeafNode* LeafNode = (CLeafNode*)(pNode);
		if (!pData->m_LeafFunc || pData->m_LeafFunc (LeafNode, pData))
		{
			// if we get past the leaf func pointer then we need make sure we have an add object func to add the object and 
			// that the object vector is not all ready full
			if (pData->m_AddObjectFunc && m_pMainTree->m_usObjIndex < 400)
			{
				// this is a debug vector for seeing all the leafs that object is standing in
				// this needs to be removed before turn in
				// BE WARNED IF YOU UNCOMMENT THIS MAKE SURE YOU CLEAR THIS EVERY FRAME SOME WHERE OR ELSE BAD THINGS WILL HAPPEN
				// nah it just crashs the game after a while because the vector grows really big
				m_pMainTree->debugLeafTest.push_back (LeafNode);

				// loop through all objects that over lap this leaf
				for (std::list< CSceneObject* >::iterator itr = LeafNode->GetOverLappingObjects ().begin(); itr != LeafNode->GetOverLappingObjects ().end(); itr++)
				{	
					// make sure the object still has a bounding volume
					if (!(*itr)->GetCollidableObject())
					{
						continue;
					}

					IBaseObject* pObject = ((IBaseObject*)(*itr));
					// get the data from the Return Flags pointer
					int sReturnFlagData = (*(int*)pData->m_pVoidReturnFlags);
					int sBodyFlagData = (*(int*)pData->m_pVoidBodyFlags);
					int sObjectFlagData = (*(int*)pData->m_pVoidObjectFlags);

					if (sBodyFlagData)
					{
						if ( !(sBodyFlagData & (1<<(*itr)->GetCollidableObject()->GetBodyType())) )
						{
							//pData->m_AddObjectFunc (*itr);
							continue;
						}
					}

					int type = pObject->GetType();

					if (sObjectFlagData)
					{
						if ( !(sObjectFlagData & (1<<type)) )
						{
							//pData->m_AddObjectFunc (*itr);
							continue;
						}
					}

					// see what kind of CollisionVolumeType they want
					if (sReturnFlagData)
					{
						if ( (sReturnFlagData & (1<<RFLAG_SPHERE)) )
						{
							if ((*itr)->GetCollidableObject()->GetVolumeType() == VMT_SPHERE)
							{
								pData->m_AddObjectFunc (*itr);
							}
						}

						if ( (sReturnFlagData & (1<<RFLAG_CAPSULE)) )
						{
							if ((*itr)->GetCollidableObject()->GetVolumeType() == VMT_CAPSULE)
							{
								pData->m_AddObjectFunc (*itr);
							}
						}

						if ( (sReturnFlagData & (1<<RFLAG_AABB)) )
						{
							if ((*itr)->GetCollidableObject()->GetVolumeType() == VMT_AABB)
							{
								pData->m_AddObjectFunc (*itr);
							}
						}

						if ( (sReturnFlagData & (1<<RFLAG_BOX)) )
						{
							if ((*itr)->GetCollidableObject()->GetVolumeType() == VMT_BOX)
							{
								pData->m_AddObjectFunc (*itr);
							}
						}
					}
					else
					{
						pData->m_AddObjectFunc (*itr);
					}
				}
			}
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MasterObjInsert():	Takes in a SceneObject (pObject). This function add the object to the
//			master list of object inside the kd tree. This may get removed because
//			there does not need to be multiple lists of all objects.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::MasterObjInsert ( CSceneObject* pObject )
{
	// make sure the object has a bounding volume
	if (!pObject || !pObject->GetCollidableObject() || 
		pObject->GetCollidableObject()->GetBodyType() == BT_NULL ||
		pObject->GetCollidableObject()->GetVolumeType() == VMT_NULL)
	{
		return;
	}

	// remove object from master list if it is already in there
	MasterObjRemove (pObject);

	// if the object already has a tree dont add it to this one
	if (pObject->GetTree())
	{
		return;
	}

	// set the objects tree to this tree
	pObject->SetTree (m_pMainTree);

	// update the object so it get put in the leafs
	m_pMainTree->UpdateObject (pObject);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MasterObjRemove():	Takes in a SceneObject (pObject). This function removes the object from
//						the master list and any leaves it is overlapping. This function may 
//						be removed or replaced with a different function because
//						there does not need to be multiple lists of all objects.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::MasterObjRemove ( CSceneObject* pObject )
{
	// if the objects tree is not equal to this tree then dont remove it
	if (pObject->GetTree() != m_pMainTree)
	{
		return;
	}

	// remove the object from all its overlapping leafs
	pObject->RemoveFromLeaves ();
	// set the objects tree to null
	pObject->SetTree (NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ReturnObjInsert():	Takes in a SceneObject pointer (_pObject). This function inserts the given object inside the 
//						return vector
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::ReturnObjInsert ( CSceneObject* pObject )
{
	// if the return object list of this tree is already at max capcity then just return
	if (pObject->GetTree()->m_usObjIndex >= 100)
	{
		return;
	}
	// check to make sure the object is not in the return vector
	if (!pObject->GetInReturnVec())
	{
		// add the object to the return vector
		pObject->GetTree()->m_vObjects[pObject->GetTree()->m_usObjIndex] = (pObject);
		++pObject->GetTree()->m_usObjIndex;
		pObject->SetInReturnVec (true);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ReturnObjClear():	This function clears out the return vector.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::ReturnObjClear ()
{
	// clear the return vector and resize it to the max number of objects
	m_vObjects.clear ();
	m_vObjects.resize (100, NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InsertObjInLeaf():	Takes in a LeafNode pointer and TraverseData pointer (_pLeaf, _pData). This function
//						adds the object inside the TraverseData to the leafs object list and adds the leaf
//						to the objects leaf list.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CKdTree::InsertObjInLeaf ( CLeafNode* pLeaf, tTraverseData* pData )
{
	CSceneObject* object = (CSceneObject*)pData->m_pVoidData;

	// insert the object into the leafs over lapping objects
	pLeaf->GetOverLappingObjects().push_back (object);

	if ( object->GetCollidableObject()->GetBodyType() == BT_STATIC )
	{
		return true;
	}

	// make sure we and not adding leafs to triangles
	// loop through all the over lapping leafs for this object
	for (int i = 0; i < MAX_OVERLAPPED_LEAVES; i++)
	{
		// if a leaf is null place the passed in leaf in the objects overlapping leaf
		if (!object->GetOverLappedLeaves()[i])
		{
			object->GetOverLappedLeaves()[i] = pLeaf;
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the Sphere that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::SphereLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pSphere = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::SphereToAAPlane( *(Sphere*)(pSphere->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CapsuleLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the Capsule that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::CapsuleLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pCapsule = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::CapsuleToAAPlane( *(Capsule*)(pCapsule->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PointLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the Point that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::PointLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pPoint = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::PointToAAPlane( *(CCollisionPoint*)(pPoint->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the Line that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::LineLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pLine = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::LineToAAPlane( *(Line*)(pLine->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AABBLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the AABB that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::AABBLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pAABB = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::AABBToAAPlane( *(AABB*)(pAABB->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BoxLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the Box that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::BoxLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pBox = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::BoxToAAPlane( *(Box*)(pBox->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FrustumLeafSplit():	Takes in an InternalNode pointer and TraverseData pointer (_pNode, _pData). This function
//						splits the InternalNode based on the Frustums AABB that is in the TraverseData.
//
// Returns:		unsigned int
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int CKdTree::FrustumLeafSplit ( CInternalNode* pNode, tTraverseData* pData )
{
	CSceneObject* pFrustumAABB = (CSceneObject*)pData->m_pVoidData;
	return CollisionLib::AABBToAAPlane( *(AABB*)(pFrustumAABB->GetCollidableObject()), pNode->GetAxis(), pNode->GetPlaneOffset() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetNearObjects():	Takes in a CSceneObject pointer, and two flag paramaters (pPassedShape, emFlags,
//						sReturnFlags), and a Output Vector Pramater (vReturnVector).
//						This function goes through the kd tree and finds all object that
//						match the ReturnFlags type ( which is a bit flag to the object types).
//						It will return all found objects in the vReturnVector.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKdTree::GetNearObjects ( CSceneObject* pPassedShape, emPassedFlags emFlags, int sReturnFlags, std::vector < CSceneObject* >& vReturnVector, int sReturnBody, int sReturnObject )
{
	// make sure the tree is still there
	if (!m_pMainTree)
	{
		return;
	}

	// setup the data needed to traverse the tree
	tTraverseData traversalData;
	traversalData.m_pVoidReturnFlags = &sReturnFlags;
	traversalData.m_pVoidBodyFlags = &sReturnBody;
	traversalData.m_pVoidObjectFlags = &sReturnObject;
	traversalData.m_pVoidData = pPassedShape;

	switch (emFlags)
	{
	case PSFLAG_SPHERE:
		{
		traversalData.m_SplitFunc = CKdTree::SphereLeafSplit;
		}
		break;
	case PSFLAG_CAPSULE:
		{
		traversalData.m_SplitFunc = CKdTree::CapsuleLeafSplit;
		}
		break;
	case PSFLAG_POINT:
		{
		traversalData.m_SplitFunc = CKdTree::PointLeafSplit;
		}
		break;
	case PSFLAG_LINE:
		{
		traversalData.m_SplitFunc = CKdTree::LineLeafSplit;
		}
		break;
	case PSFLAG_FRUSTUM:
		{
		traversalData.m_SplitFunc = CKdTree::FrustumLeafSplit;
		}
		break;
	case PSFLAG_AABB:
		{
		traversalData.m_SplitFunc = CKdTree::AABBLeafSplit;
		}
		break;
	case PSFLAG_BOX:
		{
		traversalData.m_SplitFunc = CKdTree::BoxLeafSplit;
		}
		break;
	}

	traversalData.m_AddObjectFunc = ReturnObjInsert;
	
	// traverse the tree and find all the objects that they want
	m_pMainTree->Traverse( &traversalData );

	// place all the objects found inside the return vector
	vReturnVector.assign (m_pMainTree->m_vObjects.begin(), m_pMainTree->m_vObjects.begin() + m_pMainTree->m_usObjIndex);

	// reset the bool saying that the object is in the return vector to false so the next call can get that object if needed
	for (unsigned int ReturnIndex = 0; ReturnIndex < vReturnVector.size(); ReturnIndex++)
	{
		vReturnVector[ReturnIndex]->SetInReturnVec (false);
	}

	m_pMainTree->m_usObjIndex = 0;
	m_pMainTree->ReturnObjClear();
}

void CKdTree::FlushKDTree( void )
{
	for( list< CLeafNode* >::iterator LeafItr =  m_pMainTree->m_lsLeafs.begin(); LeafItr != m_pMainTree->m_lsLeafs.end(); LeafItr++)
	{
		CLeafNode* LeafNode = (*LeafItr);

		// loop through all objects that over lap this leaf
		for (std::list< CSceneObject* >::iterator itr = LeafNode->GetOverLappingObjects ().begin(); itr != LeafNode->GetOverLappingObjects ().end();)
		{	
			// make sure the object still has a bounding volume
			if (!(*itr)->GetCollidableObject() || (*itr)->GetTree() != m_pMainTree )
			{
				itr = LeafNode->GetOverLappingObjects().erase( itr );
				//continue;
			}
			else
			{
				itr++;
			}
		}
	}
}
