#include "AnimationIncludes.h"

ButtonClosingState::~ButtonClosingState(void)
{

}

// Access the state to set it in the state machine for each component.
ButtonClosingState* ButtonClosingState::GetState(void)
{
	static ButtonClosingState state;
	return &state;
}

// This will set all variables needed for this animation.
void ButtonClosingState::Enter( CComponent_Animation* pComponent )
{
	// Set the Animation Play backwards to false.
	pComponent->SetAnimationBackwards( false );

	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation( "Button_GoingDown" ) );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;
	pComponent->GetInterpolator().SetTime( 0.0f );
	m_fPlaybackSpeed = 3.0f;
}

// The actual processing of Animation KeyFrames.
void ButtonClosingState::Execute( CComponent_Animation* pComponent, float fTime )
{
	// The Elapsed Time to update the Animation.
	float ElapsedTime = (m_fPlaybackSpeed * fTime);

	if( !pComponent->IsAnimationBackwards() )
	{
		// Switch to idle if this animation is done
		if( pComponent->GetInterpolator().IsAnimationDone() )
		{
			return;
		}

	}
	else
	{
		pComponent->GetInterpolator().SetIsAnimationDone(false);
		if( pComponent->GetInterpolator().GetTime() <= ElapsedTime )
		{
			pComponent->GetAnimStateMachine()->ChangeState( ButtonIdleState::GetState() );
		}
		// If the Animation is Set to play backwards, Set the Elapsed time to a Negative Value.
		ElapsedTime *= -1;
	}

	m_fTimeInState += ElapsedTime;
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( ElapsedTime );

	// Determine if object is inside the Frustum otherwise dont process anything no need to skin.
	if( !pComponent->IsObjectInFrustum() )
		return;

	// Process The Animation
	pComponent->GetInterpolator().Process();
	// Resize the Frame to store all of the intepolated frames.
	// Multiply by the inverse Bind Pose.
	pComponent->GetAnimationFrames().resize( pComponent->GetInterpolator().GetCurrentBoneFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix) * 
		pComponent->GetInterpolator().GetCurrentBoneFrames()[i].GetWorldMat();

}

// Clean up of the Animation.
void ButtonClosingState::Exit( CComponent_Animation* pComponent )
{

}