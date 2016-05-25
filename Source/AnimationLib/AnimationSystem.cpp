#include "AnimationSystem.h"
#include "AnimationManager.h"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

#include <fstream>
using namespace std;

#define ANIM_FILE_EXT ".InfiniteHPAnim"
#define ANIM_FILE_EXT_LEN strlen(ANIM_FILE_EXT)

bool Animation::LoadFromFile(string szFileName)
{
	int index = szFileName.find_last_of( "_" );
	char* FileName = new char[index+1];
	memcpy( FileName, szFileName.c_str(), index);
	FileName[index] = '\0';
	// Load Joint before the Animation.
	m_BindPose = AnimationManager::GetInstance()->IsBindPoseLoaded( FileName );

	// Add on directory before loading
	int len = (strlen(szFileName.c_str())+1) + ANIM_DIR_LEN + ANIM_FILE_EXT_LEN;
	char* AnimationFileName = new char[len];
	sprintf_s( AnimationFileName, len, "%s%s%s", ANIM_DIR, szFileName.c_str(), ANIM_FILE_EXT );

	// Delete allocated memory.
	delete[] FileName;


	fstream AnimationStream;
	// Open Animation File
	AnimationStream.open( AnimationFileName, ios_base::in | ios_base::binary );

	// If it is open read in the data.
	if( AnimationStream.is_open() )
	{
		// Seek to beggining of file
		AnimationStream.seekg( ios_base::beg );

		// Read in Animation Name
		int nameLength;
		AnimationStream.read( (char*)&nameLength, 4);
		// +1 is for Null Terminator which isn't written to the file. If there is no Plus One it causes Heap Corruption.
		m_szName = new char[nameLength+1];
		AnimationStream.read( m_szName, nameLength);
		m_szName[nameLength] = '\0';

		// Read in Animation Duration
		AnimationStream.read( (char*)&m_fDuration, 4);

		// Loop through number of bones from bind pose.
		for( unsigned int i = 0; i < m_BindPose->GetNumberOfBones(); i++ )
		{
			AnimatedBone BoneToAddOn;// = new AnimatedBone();

			// Read in current Bone Index.
			AnimationStream.read( (char*)&BoneToAddOn.BoneIndex, 4 );

			// Read in Number of KeyFrames for that bones.
			int NumOfKeyFrames;
			AnimationStream.read( (char*)&NumOfKeyFrames, 4 );

			BoneToAddOn.unNumKeyFrames = (unsigned int)NumOfKeyFrames;

			BoneToAddOn.pKeyFrames = new KeyFrame[NumOfKeyFrames];

			// Loop through reading in each keyframe for this bone.
			for( int KeyIndex = 0; KeyIndex < NumOfKeyFrames; KeyIndex++ )
			{
				KeyFrame frameToAddOn;

				// Read in Key Index.
				AnimationStream.read( (char*)&frameToAddOn.m_nKeyIndex, 4 );
				// Read in Key Time.
				AnimationStream.read( (char*)&frameToAddOn.m_fKeyTime, 4 );
				// Read in Local Matrix.
				AnimationStream.read( (char*)&frameToAddOn.m_TransformFrame.GetLocalMat(), 64 );
				// Read in World Matrix.
				AnimationStream.read( (char*)&frameToAddOn.m_TransformFrame.GetWorldMat(), 64 );

				// Add on the KeyFrame
				//BoneToAddOn.vKeyFrames.push_back( frameToAddOn );
				*(BoneToAddOn.pKeyFrames + KeyIndex) = frameToAddOn;
			}

			m_AnimationBoneData.push_back( BoneToAddOn );
		}

	}
	else
		return false;

	// Delete allocated memory.
	delete[] AnimationFileName;

	AnimationStream.close();

	return true;
}


void Interpolator::SetAnimation( const Animation* inAnimation)
{
	m_pAnimation = inAnimation;
	m_vBoneFrames.resize(m_pAnimation->m_AnimationBoneData.size());
	for( unsigned int nFrame = 0; nFrame < m_vBoneFrames.size(); nFrame++)
	{
		m_vBoneFrames[nFrame] = m_pAnimation->m_AnimationBoneData[nFrame].pKeyFrames->m_TransformFrame;
	}
	m_bIsAnimationDonePlaying = false;
}

void Interpolator::AddTime(float time)
{
	m_fCurrentTime += time;

	float AnimationDuration = const_cast<Animation*>(m_pAnimation)->m_fDuration;

	if( m_fCurrentTime < 0.0f )
		m_fCurrentTime += AnimationDuration;
	else if( m_fCurrentTime >= AnimationDuration )
		m_fCurrentTime -= AnimationDuration;
}

void Interpolator::SetTime( float time)
{
	m_fCurrentTime = time;

	if( m_pAnimation )
	{
		float AnimationDuration = const_cast<Animation*>(m_pAnimation)->m_fDuration;

		if( m_fCurrentTime < 0.0f )
			m_fCurrentTime += AnimationDuration;
		while( m_fCurrentTime >= AnimationDuration )
		{
			m_fCurrentTime -= AnimationDuration;
		}
	}
}

void Interpolator::Process(void)
{
	vector<AnimatedBone> bones = const_cast<Animation*>(m_pAnimation)->GetAnimatedBones();

	float fTweenTime;
	int currentFrame = 0, nextFrame = 0;
	float percentageOverTime;
	
	float AnimationDuration = const_cast<Animation*>(m_pAnimation)->m_fDuration;


		for( unsigned int i = 0; i < bones.size(); ++i)
		{
			KeyFrame* frames = bones[i].pKeyFrames;
			for( unsigned int keyIndex = 0; keyIndex < bones[i].unNumKeyFrames; keyIndex++)
			{
				if( keyIndex + 1 < bones[i].unNumKeyFrames )
					fTweenTime = frames[keyIndex + 1].m_fKeyTime - frames[keyIndex].m_fKeyTime;
				else
					fTweenTime = AnimationDuration - frames[keyIndex].m_fKeyTime;

				float valueToCompare = frames[keyIndex].m_fKeyTime + fTweenTime;
			if( this->m_fCurrentTime <= valueToCompare )
				{
					currentFrame = keyIndex;
					break;
				}
			}

			nextFrame = currentFrame + 1;
			if( nextFrame >= (int)bones[i].unNumKeyFrames/*vKeyFrames.size()*/ )
				nextFrame = 0;

			percentageOverTime = ((m_fCurrentTime - bones[i].pKeyFrames[currentFrame].m_fKeyTime) / fTweenTime);

			// Interpolate between both matrices
			// Set the world matrix =  to the interpolated data.
			m_vBoneFrames[i].GetWorldMat() = (*(matrix4f*)&Interpolate( (*(D3DXMATRIX*)&bones[i].pKeyFrames[currentFrame].m_TransformFrame.GetWorldMat()) , (*(D3DXMATRIX*)&bones[i].pKeyFrames[nextFrame].m_TransformFrame.GetWorldMat()), percentageOverTime));
	}

	if( nextFrame >= (int)(bones[0].unNumKeyFrames /*vKeyFrames.size()*/ -1) )
		m_bIsAnimationDonePlaying = true;
}