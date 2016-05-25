//////////////////////////////////////////////////////////////////
//	Date: 5/1/12												//
//	Author: Andy M.												//
//	Purpose: Purpose is to create a hierarchy can be useful for //
//			 flattening the hierarchy.							//
//////////////////////////////////////////////////////////////////

#ifndef NODE_H_
#define NODE_H_

#include <vector>
using namespace std;

class Node
{
//Private

	// Parent pointer of this object.
	Node* m_pParent;

	// Child pointer of this object, can be more than one.
	vector< Node* > m_vChildren;

public:

	// Constructor
	Node(void) : m_pParent(0) {}
	// Destructor
	virtual ~Node(void) {}

	// Helper Functions
	Node* GetParent(void) { return m_pParent; }
	vector< Node* >& GetChildren(void) { return m_vChildren; }

	// Useful functions

/*****************************************************************/
	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: Node* Child									//
	//															//
	//	Purpose: This function will add the Node passed in to	//
	//			 the list of children of the invoking object.	//
	//															//
	//	Notes: If returns NULL it failed.						//
	//////////////////////////////////////////////////////////////
	Node* AddChild( Node* pChildToAdd )
	{
		// If this occurs chances are its already
		// in the hierarchy or it is in another hierarchy.
		// Either way it will mess up the flow of this list
		// so don't add it on and return NULL, which means
		// failure.
		if( pChildToAdd->GetParent() != NULL )
			return NULL;

		// Otherwise keep the list flowing setting the parent
		// aswell as adding this node onto the parents list of 
		// children. Return the child added.
		pChildToAdd->m_pParent = this;
		m_vChildren.push_back(pChildToAdd);
		return pChildToAdd;
	}

	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: Node* Child									//
	//															//
	//	Purpose: This function will remove the Node passed in	//
	//			 from the list of children of the				//
	//			 invoking object.								//
	//															//
	//	Notes: If returns NULL it failed.						//
	//////////////////////////////////////////////////////////////
	Node* RemoveChild( Node* pChildToRemove )
	{
		// This child does not belong to this parents children
		// We must be at the wrong parents house.
		// Get out before they call the cops.
		if( pChildToRemove->GetParent() != this )
			return NULL;

		// Loop through the list of Childrens.
		for( unsigned int i = 0; i < m_vChildren.size(); i++)
		{
			// If the child is in the house at this point,
			// disown him. Don't want him staying with his
			// parents forever.
			if( m_vChildren[i] == pChildToRemove )
			{
				m_vChildren.erase(m_vChildren.begin() + i);
				pChildToRemove->m_pParent = NULL;
				return pChildToRemove;
			}
		}

		// If we arrive at this point then the child was probably in the list
		// but he did not recognize his own parents so return NULL for now.
		// If crashes occur we must find out where this child forgot who his
		// parents were.
		return NULL;
	}

/*******************************************************************/

};

#endif