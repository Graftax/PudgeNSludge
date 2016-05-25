#ifndef ANIMATIONSYSTEM_H_
#define ANIMATIONSYSTEM_H_

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            AnimationSystem.h
// Author:              Andy Madruga
// Date:                5/24/12
// Purpose:             Defines all the classes used for animations.
//////////////////////////////////////////////////////////////////////////////////////

#include "BoneFrame.h"
#include "../Utilities/Interpolate.h"

__declspec(align(16))
struct tJoint
{
	int			nJointIndex;			// The index of this joint.
	int			nParentIndex;			// The index of the parent, if -1 then this is the root.
	char*		m_JointName;			// The name of the Joint.
	D3DXMATRIX	m_InverseLocalMatrix;	// The Local Matrix already Flattened and Pre Inverted.
	vector<int>	m_ChildrenIndex;		// A list of children Indices.

	~tJoint()
	{
		if( m_JointName )
			delete[] m_JointName;
	}
};

__declspec(align(32))
class BindPose
{
private:
	char*		szBindPoseName;				// The name of this Bind Pose.

	unsigned int uiNumberOfBones;			// The number of bones in this Bind Pose.
	vector<tJoint*> m_HierarchyList;		// A Hierarchy List of the Bind Pose.

public:

	void SetNumberOfBones(unsigned int value) { uiNumberOfBones = value; }
	unsigned int GetNumberOfBones(void) { return uiNumberOfBones; }
	char* GetBindPoseName(void) { return szBindPoseName; }
	void SetBindPoseName( const char* name)
	{
		int len = strlen(name) + 1;
		szBindPoseName = new char[len];
		memcpy( szBindPoseName, name, len);
	}
	tJoint* GetJoint( const char* jointName )
	{
		for( unsigned int i = 0; i < uiNumberOfBones; i++ )
		{
			if( _strcmpi( jointName, m_HierarchyList[i]->m_JointName) == 0 )
				return m_HierarchyList[i];
		}

		// If it was not found then return NULL.
		return NULL;
	}

	vector<tJoint*>& GetHierarchyList(void) { return m_HierarchyList; }

	~BindPose()
	{
		if( szBindPoseName )
		{
			delete[] szBindPoseName;
		}

		for( unsigned int iBone = 0; iBone < m_HierarchyList.size(); iBone++)
			delete m_HierarchyList[iBone];
		m_HierarchyList.clear();
	}
};

struct KeyFrame
{
	// The index for this keyframe.
	int		m_nKeyIndex;
	// The time the keyframe occurs.
	float	m_fKeyTime;

	// The transform of this keyframe
	BoneFrame m_TransformFrame;
};

struct AnimatedBone
{
	// The index of the animated bone
	int BoneIndex;
	KeyFrame& operator[] (int x) {
          return *(pKeyFrames + x);
      }
	unsigned int unNumKeyFrames;
	// A vector of this bones keyframes.
	KeyFrame* pKeyFrames;
	AnimatedBone() { pKeyFrames = nullptr; }
	~AnimatedBone() { /*if( pKeyFrames ) delete[] pKeyFrames;*/ }
};

__declspec(align(32))
class Animation
{
	friend class AnimationManager;
	friend class Interpolator;
private: 
	// The duration of the Animation
	float m_fDuration;
	// The name of the Animation
	char* m_szName;

	// Pointer to the JointHierarchy.
	BindPose*			m_BindPose;

	// All of our AnimatedBones each has a vector of transforms 
	// and times they take place.
	// Actual animation data is in here.
	vector< AnimatedBone > m_AnimationBoneData; 

public:
	Animation(void) : m_szName(0){ m_fDuration = 0.0f; }
	~Animation(void)
	{
		if( m_szName )
			delete[] m_szName;

		for( unsigned int i = 0u; i < m_AnimationBoneData.size(); ++i )
			if( m_AnimationBoneData[i].pKeyFrames )
				delete[] m_AnimationBoneData[i].pKeyFrames;
	}

	bool LoadFromFile(string szFileName);

	vector<AnimatedBone>&	GetAnimatedBones(void){ return m_AnimationBoneData; }
	float					GetDuration(void){ return m_fDuration; }

	BindPose*				GetBindPose(void){ return m_BindPose; }
};

__declspec(align(32))
class Interpolator
{
private:
	// A pointer to the Animation data being used.
	const Animation*	m_pAnimation;
	// A vector of inteprolated bones frames
	vector<BoneFrame>	m_vBoneFrames;
	// The current time in the Animation.
	float m_fCurrentTime;
	bool m_bIsAnimationDonePlaying;
public:
	Interpolator(void) : m_pAnimation(0){}
	~Interpolator(void) { m_vBoneFrames.clear(); }

	void SetAnimation( const Animation* inAnimation);
	const Animation* GetAnimation() { return m_pAnimation; }
	void AddTime(float time);
	void SetTime( float time);
	void Process(void);

	vector<BoneFrame>&		GetCurrentBoneFrames(){ return m_vBoneFrames; }

	bool IsAnimationDone(void) { return m_bIsAnimationDonePlaying; }
	void SetIsAnimationDone(bool value) { m_bIsAnimationDonePlaying = value; }

	float GetTime( void ) { return m_fCurrentTime; }

};

#endif