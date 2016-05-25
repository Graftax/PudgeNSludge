#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            AnimationManager.h
// Author:              Andy Madruga
// Date:                5/24/12
// Purpose:             This manager will handle loading up all animations
//						along with the bind pose corresponding for each animation. It
//						also take care of all memory allocated.
//////////////////////////////////////////////////////////////////////////////////////

#include "AnimationSystem.h"

#define ANIM_DIR "./Resources/Animations/"
#define ANIM_DIR_LEN strlen(ANIM_DIR)
#define JOINT_FILE_EXT ".InfiniteHPJoint"
#define JOINT_FILE_EXT_LEN strlen(JOINT_FILE_EXT)

__declspec(align(32))
class AnimationManager
{
private:
	// Constructor
	AnimationManager(void);
	// Copy Constructor
	AnimationManager(const AnimationManager& ){}
	// Assignment Operator
	const AnimationManager& operator=( const AnimationManager&){}

	static AnimationManager* _highlander;
	// Master List of BindPoses.
	vector<BindPose*> m_vBindPoses;

	//////////////////////////////////////////////////////////////////
	//	LoadBindPose():	Loads the Bind pose based on the string
	//					passed in. Does not load again if it always
	//					already loaded.
	//
	//	Returns:		int - the index into the master bindpose list.
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	int LoadBindPose( const char* szBindName );

public:
	vector<Animation*> m_vAnimations;

	// Destructor
	~AnimationManager(void);

	//////////////////////////////////////////////////////////////////
	//	GetInstance():	Grabs the Instance of the AnimationManager
	//					and returns a pointer to the manager.
	//
	//	Returns:		AnimationManager* - Returns a pointer 
	//					to the manager.
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	static AnimationManager* GetInstance();

	//////////////////////////////////////////////////////////////////
	//	DeleteInstance():	Deletes the instance of the Manager.
	//
	//	Returns:			void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	static void DeleteInstance();

	//////////////////////////////////////////////////////////////////
	//	AddAnimation():	If the animation passed in is not yet in the 
	//					list then the manager will go ahead and add
	//					the animation to the list.
	//
	//	Returns:		bool - returns true if the animation was
	//					added succesfully to the list.
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	bool AddAnimation( Animation* pAnimationToAdd);

	//////////////////////////////////////////////////////////////////
	//	RemoveAnimation():	If the animation passed in is in the list
	//						then we can go ahead and remove it from the
	//						list.
	//
	//	Returns:		Animation* - return the pointer to the Animation
	//					so the memory allocated for it can be deleted.
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	Animation* RemoveAnimation( string szName);

	//////////////////////////////////////////////////////////////////
	//	GetAnimation():	Checks to see if the string being passed in
	//					matches any of the animations being loaded up.
	//
	//	Returns:		Animation* - return the pointer to the Animation
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	Animation* GetAnimation( string szName);

	//////////////////////////////////////////////////////////////////
	//	GetAnimation():	Utilizes the index passed in to grab the animation.
	//
	//	Returns:		Animation* - return the pointer to the Animation
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	Animation* GetAnimation( unsigned int iIndex);

	//////////////////////////////////////////////////////////////////
	//	IsBindPoseLoaded():	Checks to see if the bind pose has been 
	//						loaded up already, if not it goes ahead
	//						and loads up the pose and returns a pointer
	//						to the pose.
	//
	//	Returns:		BindPose* - return the pointer to the BindPose
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	BindPose* IsBindPoseLoaded(const char* szBindName);

	//////////////////////////////////////////////////////////////////
	//	ShutDown():	Shuts down the manager and deletes all the 
	//				Animations.
	//
	//	Returns:		void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	void ShutDown();
};

#endif