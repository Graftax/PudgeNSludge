//////////////////////////////////////////////////////////////////
//	Date: 5/1/12												//
//	Author: Andy M.												//
//	Purpose: Use to create a matrix transform/hierarchy.		//
//////////////////////////////////////////////////////////////////

#ifndef FRAME_H_
#define FRAME_H_


#include "MathLib\matrix4.h"
#include "Node.h"
#include <d3dx9.h>

// This class allows creation of a matrix hierarchy which can be
// useful for a flattening hierarchy system. *WINK*
// A Hierarchy consist of multiple Frames each with its own object/local
//space matrix and its own global/world space matrix.
//
// Object/Local space matrix (m_LocalMat) stores transformation applied only to this frame.
// The position and orientation of the local matrix is relative to the position/orientation 
// of its parent.
//
// The global/world space matrix (m_WorldMat) stores the concatenation (multiplication) of the local space
// matrix with the parent frame's world space matrix. The position and orientation of the world space matrix
// is in absolute world-space. It is where the object would/could be rendered.

class Frame : public Node
{
	// The Local/Object Space matrix of the frame.
	matrix4f m_LocalMat;

	// The World/Global Space matrix of the frame.
	matrix4f m_WorldMat;

	// Flag used to identify whether the frames needs to be updated.
	bool Dirty;

public:

	// Constructor
	Frame(void) : Dirty(0) 
	{
		m_LocalMat.make_identity();
		m_WorldMat.make_identity();
	}
	// Destructor
	virtual ~Frame(void){}

	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: This function will "Dirty" a frame and its		//
	//			 children. The worldspace matrix (m_WorldMat)	//
	//			 of "Dirty" frames is "cleaned" when			//
	//			 GetWorldMat() is called.						//
	//															//
	//	Notes: Used for Flattening Hierarchy.					//
	//////////////////////////////////////////////////////////////
	void Update(void)
	{
		// If Frame is not dirty.
		if( !Dirty )
		{
			// Make this sucker Dirty.
			Dirty = !Dirty;

			// Loop through the children, upcast the pointer, Dirty those Frames.
			for( unsigned int i = 0; i < GetChildren().size(); ++i)
				((Frame*)GetChildren()[i])->Update();
		}
	}

	// Helper Functions

/**********************************************************************************/
	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: Get the Local Space matrix of the frame.		//
	//															//
	//	Notes: None.											//
	//////////////////////////////////////////////////////////////
	matrix4f& GetLocalMat(void){ return m_LocalMat; }
	
	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: Get the World Space matrix of the frame.		//
	//															//
	//	Notes: If the frame is "Dirty" the world matrix is		//
	//		   first recalculated and the frame is cleaned.		//
	//		   DO NOT edit the matrix directly, any changes done//
	//		   will be lost on the next update, because			//
	//		   m_WorldMat = m_LocalMat or						//
	//		   m_WorldMat = m_LocalMat * pParent->m_WorldMat;	//
	//////////////////////////////////////////////////////////////
	matrix4f&	GetWorldMat(void)
	{
		// If the Frame is Dirty.
		if( Dirty )
		{
			// Safe check make sure we are not the root.
			if( GetParent() != NULL)
				m_WorldMat = m_LocalMat * ((Frame*)GetParent())->GetWorldMat();
			// We are the root so just set to the local matrix.
			else
				m_WorldMat = m_LocalMat;

			// The Frame is now clean. Remember to let the Customer know it's done.
			Dirty = !Dirty;
		}
		return m_WorldMat;
	}

	
/**********************************************************************************/
};

#endif