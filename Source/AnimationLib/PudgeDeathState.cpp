#include "AnimationIncludes.h"
#include "../ComponentManager/CComponent_Collision.h"

PudgeDeathState::~PudgeDeathState(void)
{
}

// Access the state to set it in the state machine for each component.
PudgeDeathState* PudgeDeathState::GetState(void)
{
	static PudgeDeathState state;
	return &state;
}

// This will set all variables needed for this animation.
void PudgeDeathState::Enter( CComponent_Animation* pComponent )
{
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("Pudge_Death") );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;

		pComponent->GetInterpolator().SetTime( 0.0f );


}

// The actual processing of Animation KeyFrames.
void PudgeDeathState::Execute( CComponent_Animation* pComponent, float fTime )
{
	m_fTimeInState += fTime;
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( fTime );
	// Process The Animation
	pComponent->GetInterpolator().Process();
	// Resize the Frame to store all of the intepolated frames.
	pComponent->GetAnimationFrames().resize( pComponent->GetInterpolator().GetCurrentBoneFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix) * 
		pComponent->GetInterpolator().GetCurrentBoneFrames()[i].GetWorldMat();

// 	if( pComponent->GetInterpolator().IsAnimationDone() )
// 		//pComponent->GetAnimStateMachine()->ChangeState( PudgeJumpToFallingState::GetState() );
// 		pComponent->GetAnimStateMachine()->ChangeState( PudgeFallingState::GetState() );
}

// Clean up of the Animation.
void PudgeDeathState::Exit( CComponent_Animation* pComponent )
{

}