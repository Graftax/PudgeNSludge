#include "PudgePushState.h"
#include "AnimationIncludes.h"

PudgePushState::~PudgePushState(void)
{
}

// Access the state to set it in the state machine for each component.
PudgePushState* PudgePushState::GetState(void)
{
	static PudgePushState state;
	return &state;
}

// This will set all variables needed for this animation.
void PudgePushState::Enter( CComponent_Animation* pComponent )
{
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("Pudge_Push") );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

		pComponent->GetInterpolator().SetTime( 0.0f );
}

// The actual processing of Animation KeyFrames.
void PudgePushState::Execute( CComponent_Animation* pComponent, float fTime )
{
	if( pComponent->GetInterpolator().IsAnimationDone() )
		pComponent->GetAnimStateMachine()->ChangeState( PudgeIdleState::GetState() );

	m_fTimeInState += fTime;
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( fTime );

	// Determine if object is inside the Frustum otherwise dont process anything no need to skin.
	if( !pComponent->IsObjectInFrustum() )
		return;

	// Process The Animation
	pComponent->GetInterpolator().Process();
	// Resize the Frame to store all of the intepolated frames.
	pComponent->GetAnimationFrames().resize( pComponent->GetInterpolator().GetCurrentBoneFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix) *
		pComponent->GetInterpolator().GetCurrentBoneFrames()[i].GetWorldMat();

	
}

// Clean up of the Animation.
void PudgePushState::Exit( CComponent_Animation* pComponent )
{

}