//////////////////////////////////////////////////////////////////////////////////////
// Filename:	KdTreeNodes.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This files holds all the different Nodes that are needed for the Kd Tree
//////////////////////////////////////////////////////////////////////////////////////
#define _BROKEN_AUDIO
#include "KdTreeNodes.h"
#include "KdTree.h"
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Traverse():	virtual Traverse function used to traverse the Tree
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInternalNode::~CInternalNode ()
{
	delete m_pLeftChild;
	delete m_pRightChild;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Traverse():	Takes in a TraverseData pointer (pData). This function traverse though the Kd Tree and splits it accordingly
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInternalNode::Traverse ( tTraverseData* pData )
{
	// call the traversal function to find out which way we need to split the tree
	size_t unResults = CKdTree::Traversal (this, pData);

	// decide which way to split the tree
	switch (unResults)
	{
	case 0:
		m_pLeftChild->Traverse (pData);
		break;

	case 1:
		m_pRightChild->Traverse (pData);
		break;

	case 2:
		m_pLeftChild->Traverse (pData);
		m_pRightChild->Traverse (pData);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Traverse():	Takes in a TraverseData pointer (pData). This function calls the Kd Trees traverse function passing in
//				this leaf and the passed in TraverseData.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLeafNode::Traverse ( tTraverseData* pData )
{
	// traverse the tree
	CKdTree::Traversal (this, pData);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSceneObject():	Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSceneObject::CSceneObject () : m_pOwnerTree (NULL), pCollisionVolume (NULL)
{
	bAlreadyAdded = false;
	memset ( m_pOverLappedLeaves, 0, sizeof (m_pOverLappedLeaves) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CSceneObject():	Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSceneObject::~CSceneObject ()
{
	if (pCollisionVolume)
	{
		delete pCollisionVolume;
		pCollisionVolume = NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveFromLeaves():	This function removes the this SceneObject from all its over lapping leafs.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSceneObject::RemoveFromLeaves ()
{
	// loop through the leaves and remove the object from them
	for (size_t unLeafIndex = 0; unLeafIndex < MAX_OVERLAPPED_LEAVES; ++unLeafIndex)
	{
		if (m_pOverLappedLeaves [unLeafIndex])
		{
			m_pOverLappedLeaves [unLeafIndex]->GetOverLappingObjects ().remove (this);
			m_pOverLappedLeaves [unLeafIndex] = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsContained():	Checks if the collision volume of this SceneObject is contained within the objects over lapped leafs
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSceneObject::IsContained ()
{
	// decided how to check if the object is contained by looking at the objects bounding volume type
	switch (pCollisionVolume->GetVolumeType())
	{
	case VMT_SPHERE:
		{
			Sphere temp = *(Sphere*) (pCollisionVolume);
			return IsSphereContained (temp);
		}
		break;
		
	case VMT_CAPSULE:
		{
			Capsule temp = *(Capsule*) (pCollisionVolume);
			return IsCapsuleContained (temp);
		}
		break;

	case VMT_AABB:
		{
			AABB temp = *(AABB*) (pCollisionVolume);
			return IsAABBContained (temp);
		}
		break;

	case VMT_BOX:
		{
			Box temp = *(Box*) (pCollisionVolume);
			return IsBoxContained (temp);
		}
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsSphereContained():	Checks if the Sphere is contained within its over lapping leafs
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSceneObject::IsSphereContained(Sphere temp)
{
	// check to make sure the sphere tis contained 

	int Out[3] = { 0,0,0};

	int NullLeaves = 0;

	for (int i = 0; i < MAX_OVERLAPPED_LEAVES; i++)
	{
		if (!m_pOverLappedLeaves[i])
		{
			NullLeaves++;
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			if (temp.GetCenter3D().v[j] - temp.GetRadius() > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetCenter3D().v[j] + temp.GetRadius() < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
		}
	}

	if (NullLeaves == MAX_OVERLAPPED_LEAVES)
	{
		return false;
	}

	if (Out[0] == 0 && Out[1] == 0 && Out[2] == 0)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsAABBContained():	Checks if the AABB is contained within its over lapping leafs.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSceneObject::IsAABBContained (AABB temp)
{
	// check to make sure the sphere tis contained 

	int Out[3] = { 0,0,0};

	int NullLeaves = 0;

	for (int i = 0; i < MAX_OVERLAPPED_LEAVES; i++)
	{
		if (!m_pOverLappedLeaves[i])
		{
			NullLeaves++;
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			if (temp.GetMax3DTransformed().v[j] > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetMin3DTransformed().v[j] < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
		}
	}

	if (NullLeaves == MAX_OVERLAPPED_LEAVES)
	{
		return false;
	}

	if (Out[0] == 0 && Out[1] == 0 && Out[2] == 0)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsBoxContained():	Checks if the Box is contained within its over lapping leafs.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSceneObject::IsBoxContained (Box temp)
{
	// check to make sure the sphere tis contained 

	int Out[3] = { 0,0,0};

	int NullLeaves = 0;

	for (int i = 0; i < MAX_OVERLAPPED_LEAVES; i++)
	{
		if (!m_pOverLappedLeaves[i])
		{
			NullLeaves++;
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			if (temp.GetVertices()[0].Get3DCentorid().v[j] > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetVertices()[0].Get3DCentorid().v[j] < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
			if (temp.GetVertices()[1].Get3DCentorid().v[j] > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetVertices()[1].Get3DCentorid().v[j] < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
			if (temp.GetVertices()[2].Get3DCentorid().v[j] > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetVertices()[2].Get3DCentorid().v[j] < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
			if (temp.GetVertices()[3].Get3DCentorid().v[j] > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetVertices()[3].Get3DCentorid().v[j] < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
		}
	}

	if (NullLeaves == MAX_OVERLAPPED_LEAVES)
	{
		return false;
	}

	if (Out[0] == 0 && Out[1] == 0 && Out[2] == 0)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsCapsuleContained():	Checks if the Capsule is contained within its over lapping leafs
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CSceneObject::IsCapsuleContained(Capsule temp)
{
	// check to make sure the capsule is contatined
	int Out[3] = { 0, 0, 0 };

	int NullLeaves = 0;

	for (int i = 0; i < MAX_OVERLAPPED_LEAVES; i++)
	{
		if (!m_pOverLappedLeaves[i])
		{
			NullLeaves++;
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			if (temp.GetCenterLineStart3D().v[j] - temp.GetRadius() > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetCenterLineStart3D().v[j] + temp.GetRadius() < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}

			if (temp.GetCenterLineEnd3D().v[j] - temp.GetRadius() > m_pOverLappedLeaves[i]->GetBounds ().GetMinPoint3D().v[j])
			{
				Out[j]++;
			}
			if (temp.GetCenterLineEnd3D().v[j] + temp.GetRadius() < m_pOverLappedLeaves[i]->GetBounds ().GetMaxPoint3D().v[j])
			{
				Out[j]--;
			}
		}
	}

	if (NullLeaves == MAX_OVERLAPPED_LEAVES)
	{
		return false;
	}

	if (Out[0] == 0 && Out[1] == 0 && Out[2] == 0)
	{
		return true;
	}

	return false;
}

/////////////////////////////
//  Mutators
/////////////////////////////
void CSceneObject::SetCollidableObject ( CCollisionVolume* _cvVolume )
{
	if (_cvVolume)
	{
		switch (_cvVolume->GetVolumeType ())
		{
			case VMT_POINT:
				break;

			case VMT_PLANE:
				break;

			case VMT_LINE:
				pCollisionVolume = _cvVolume;
				break;

			case VMT_CIRCLE:
				break;
				
			case VMT_SPHERE:
				pCollisionVolume = new Sphere (*(Sphere*)_cvVolume);
				break;

			case VMT_CAPSULE:
				break;

			case VMT_BOX:
				pCollisionVolume = new Box (*(Box*)_cvVolume);
				break;

			case VMT_CUBE:
				break;

			case VMT_AABB:
				pCollisionVolume = new AABB (*(AABB*)_cvVolume);
			break;
		}
	}
	else
	{
		pCollisionVolume = NULL;
	}
}