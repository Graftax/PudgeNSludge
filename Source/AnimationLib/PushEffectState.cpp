#include "AnimationIncludes.h"

PushEffectState::~PushEffectState(void)
{
}

// Access the state to set it in the state machine for each component.
PushEffectState* PushEffectState::GetState(void)
{
	static PushEffectState state;
	return &state;
}

// This will set all variables needed for this animation.
void PushEffectState::Enter( CComponent_Animation* pComponent )
{
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("PushMesh_Push") );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;

	pComponent->GetInterpolator().SetTime( 0.0f );

	m_fPlaybackSpeed = 3.0f;

}

// The actual processing of Animation KeyFrames.
void PushEffectState::Execute( CComponent_Animation* pComponent, float fTime )
{
	float fElapsedTime = (fTime * m_fPlaybackSpeed);
	m_fTimeInState += fElapsedTime;
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( fElapsedTime );

	// Determine if object is inside the Frustum otherwise dont process anything no need to skin.
	if( !pComponent->IsObjectInFrustum() )
		return;

	// Process The Animation
	pComponent->GetInterpolator().Process();
	// Resize the Frame to store all of the interpolated frames.
	pComponent->GetAnimationFrames().resize( pComponent->GetInterpolator().GetCurrentBoneFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix) *
		pComponent->GetInterpolator().GetCurrentBoneFrames()[i].GetWorldMat();
}

// Clean up of the Animation.
void PushEffectState::Exit( CComponent_Animation* pComponent )
{

}