#include "AnimationManager.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

AnimationManager* AnimationManager::_highlander = NULL;

AnimationManager::AnimationManager(void)
{
}

AnimationManager::~AnimationManager(void)
{
	ShutDown();
}

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
AnimationManager* AnimationManager::GetInstance(void)
{
	if( _highlander == NULL )
		_highlander = new AnimationManager();
	return _highlander;
}

//////////////////////////////////////////////////////////////////
//	DeleteInstance():	Deletes the instance of the Manager.
//
//	Returns:			void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void AnimationManager::DeleteInstance()
{
	if( _highlander != NULL)
		delete _highlander;
}

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
bool AnimationManager::AddAnimation(Animation* pAnimationToAdd)
{
	for( unsigned int iAnim = 0; iAnim < m_vAnimations.size(); iAnim++)
	{
		// Animation is already in the list do not add.
		if( pAnimationToAdd->m_szName == m_vAnimations[iAnim]->m_szName)
			return false;
	}

	// Animation was succesfully added.
	m_vAnimations.push_back(pAnimationToAdd);
	return true;
}

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
Animation* AnimationManager::RemoveAnimation( string szName)
{
	for( unsigned int iAnim = 0; iAnim < m_vAnimations.size(); iAnim++)
	{
		if( szName == m_vAnimations[iAnim]->m_szName)
		{
			// Animation was found and removed succesfully, return the animation.
			Animation* pAnim = m_vAnimations[iAnim];
			m_vAnimations.erase(m_vAnimations.begin() + iAnim );
			return pAnim;
		}
	}
	// Animation was not found so was not removed.
	return NULL;
}

//////////////////////////////////////////////////////////////////
//	GetAnimation():	Checks to see if the string being passed in
//					matches any of the animations being loaded up.
//
//	Returns:		Animation* - return the pointer to the Animation
//					based
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
Animation* AnimationManager::GetAnimation( string szName)
{
	for( unsigned int iAnim = 0; iAnim < m_vAnimations.size(); iAnim++)
	{
		// If animation was found return the animation
		if( szName == m_vAnimations[iAnim]->m_szName)
			return m_vAnimations[iAnim];
	}
	// else Animation was not found so return NULL
	return NULL;
}

//////////////////////////////////////////////////////////////////
//	GetAnimation():	Utilizes the index passed in to grab the animation.
//
//	Returns:		Animation* - return the pointer to the Animation
//					based
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
Animation* AnimationManager::GetAnimation( unsigned int iIndex)
{
	// If it is a valid index return that animation in that index.
	if( iIndex >= 0 || iIndex < m_vAnimations.size() )
		return m_vAnimations[iIndex];
	// else return NULL
	return NULL;
}

//////////////////////////////////////////////////////////////////
//	ShutDown():	Shuts down the manager and deletes all the 
//				Animations.
//
//	Returns:		void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void AnimationManager::ShutDown(void)
{
	// Delete all the animations loaded up :D.
	for( unsigned int iAnim = 0; iAnim < m_vAnimations.size(); iAnim++)
		delete m_vAnimations[iAnim];
	m_vAnimations.clear();

	for( unsigned int iJoint = 0; iJoint < m_vBindPoses.size(); iJoint++)
		delete m_vBindPoses[iJoint];
	m_vBindPoses.clear();
}

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
BindPose* AnimationManager::IsBindPoseLoaded(const char* szBindName)
{
	for( unsigned int i = 0; i < m_vBindPoses.size(); i++)
	{
		if( _strcmpi( m_vBindPoses[i]->GetBindPoseName(), szBindName ) == 0)
			return m_vBindPoses[i];
	}

	// If we reached this section then Bind Pose was not found.
	// Load up Bind Pose and return the pointer in this case.
	int PoseIndex = LoadBindPose( szBindName );
	return m_vBindPoses[PoseIndex];
}

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
int AnimationManager::LoadBindPose( const char* szBindName )
{
	BindPose* PoseToAddOn = new BindPose();
	PoseToAddOn->SetBindPoseName( szBindName );

	// Add on directory before loading
	int len = (strlen(szBindName)+1) + ANIM_DIR_LEN + JOINT_FILE_EXT_LEN;
	char* BoneFileName = new char[len];
	sprintf_s( BoneFileName, len, "%s%s%s", ANIM_DIR, szBindName, JOINT_FILE_EXT );

	fstream PoseStream;
	PoseStream.open( BoneFileName, ios_base::in | ios_base::binary );

	if( PoseStream.is_open() )
	{
		// Read in Bind Pose
		PoseStream.seekg(ios_base::beg);

		// Read in Number Of Bones
		int NumOfBones;
		PoseStream.read( (char*)&NumOfBones, 4);
		PoseToAddOn->SetNumberOfBones( NumOfBones );
		
		for( int boneIndex = 0; boneIndex < NumOfBones; boneIndex++)
		{
			tJoint* jointToAddOn = new tJoint();

			// Read in Joint Index
			PoseStream.read( (char*)&jointToAddOn->nJointIndex, 4);

			// Read in Name
			int nameLength = 0;
			PoseStream.read( (char*)&nameLength, 4);
			jointToAddOn->m_JointName = new char[nameLength];
			PoseStream.read( jointToAddOn->m_JointName, nameLength );

			// Read in Parent Index and Inverse Local Matrix.
			PoseStream.read( (char*)&jointToAddOn->nParentIndex, 4 );
			PoseStream.read( (char*)&jointToAddOn->m_InverseLocalMatrix, sizeof(jointToAddOn->m_InverseLocalMatrix) );	// Size of D3DXMatrix

			// Load Up its children.
			int NumberOfChildren;
			PoseStream.read( (char*)&NumberOfChildren, 4 );

			for( int childIndex = 0; childIndex < NumberOfChildren; childIndex++ )
			{
				int childIndexToRead;
				PoseStream.read( (char*)&childIndexToRead, 4 );
				jointToAddOn->m_ChildrenIndex.push_back(childIndexToRead);
			}

			// Add on the Joint
			PoseToAddOn->GetHierarchyList().push_back(jointToAddOn);
		}

	}
	PoseStream.close();

	int returnIndex = (int)m_vBindPoses.size();
	m_vBindPoses.push_back( PoseToAddOn );

	delete[] BoneFileName;

	return returnIndex;
}


