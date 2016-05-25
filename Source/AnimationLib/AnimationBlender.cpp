#include "AnimationBlender.h"

AnimationBlender::AnimationBlender(void)
{
	m_fTransitionTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_pInterpolatorOne = NULL;
	m_pInterpolatorTwo = NULL;
	m_bIsDoneInterpolating = false;
}

AnimationBlender::~AnimationBlender(void)
{
}

bool AnimationBlender::SetInterpolators(Interpolator* pInterp1, Interpolator* pInterp2)
{
	m_pInterpolatorOne = pInterp1;
	m_pInterpolatorTwo = pInterp2;

	return true;
}

bool AnimationBlender::Dissolve()
{
	// Clear first to make sure blended frames is empty.
	m_vBlendedFrames.clear();

	// This will be the factor of how much to interpolate by over time.
	float Lambda = GetDissolveFactor();

	if( Lambda >= 1.0f )
		SetIsDoneInterp(true);

	int NumOfBones = m_pInterpolatorOne->GetCurrentBoneFrames().size();

	for( int i = 0; i < NumOfBones; i++)
	{
		// Interpolate from InterpolatorOne world Matrix to InterpolatorTwo world Matrix over Lambda
		// Store the new matrix in Blended Frames
		D3DXMATRIX blendedMatrix = Interpolate( (*(D3DXMATRIX*)&m_pInterpolatorOne->GetCurrentBoneFrames()[i].GetWorldMat()), (*(D3DXMATRIX*)&m_pInterpolatorTwo->GetCurrentBoneFrames()[i].GetWorldMat()), Lambda );
		m_vBlendedFrames.push_back( blendedMatrix );
	}

	// Dissolve was successful.
	return true;
}