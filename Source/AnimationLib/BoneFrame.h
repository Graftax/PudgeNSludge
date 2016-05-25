//////////////////////////////////////////////////////////////////
//	Date: 5/1/12												//
//	Author: Andy M.												//
//	Purpose: Base class to define what a BoneFrame contains.	//
//////////////////////////////////////////////////////////////////

#ifndef BONEFRAME_H_
#define BONEFRAME_H_

#include "../Utilities/Frame/Frame.h"
#include <string>
using namespace std;

class BoneFrame : public Frame
{
	// Index into a bone list
	int m_iBoneIndex;

	// The name of the bone
	string m_szBoneName;
public:
	// Constructor
	BoneFrame(void){ m_iBoneIndex = -1; }

	// Helper Functions

/**********************************************************************************/

	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: Get the bone index of this frame.				//
	//															//
	//	Notes: None.											//
	//////////////////////////////////////////////////////////////
	int GetBoneIndex(void){ return m_iBoneIndex; }
	
	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: Get the bone name of this frame.				//
	//															//
	//	Notes: None.											//
	//////////////////////////////////////////////////////////////
	string GetBoneName(void){ return m_szBoneName; }

	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: Set the bone index of this frame.				//
	//															//
	//	Notes: None.											//
	//////////////////////////////////////////////////////////////
	void SetBoneIndex(int index){ m_iBoneIndex = index; }
	
	//////////////////////////////////////////////////////////////
	//	Author: Andy M.											//
	//															//
	//	Parameters: void										//
	//															//
	//	Purpose: Set the bone name of this frame.				//
	//															//
	//	Notes: None.											//
	//////////////////////////////////////////////////////////////
	void SetBoneName(string szName){ m_szBoneName = szName; }

/**********************************************************************************/

};

#endif