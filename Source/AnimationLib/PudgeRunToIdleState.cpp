#include "AnimationIncludes.h"

PudgeRunToIdleState::~PudgeRunToIdleState(void)
{
}

// Access the state to set it in the state machine for each component.
PudgeRunToIdleState* PudgeRunToIdleState::GetState(void)
{
	static PudgeRunToIdleState state;
	return &state;
}

// This will set all variables needed for this animation.
void PudgeRunToIdleState::Enter( CComponent_Animation* pComponent )
{
	m_fTimeInState = 0.0f;
	
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("Pudge_Run") );
	pComponent->GetBlendInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("Pudge_Idle") );
	

	State* prevstate = const_cast<State*>(pComponent->GetAnimStateMachine()->GetPreviousState());
	if( prevstate )
	{
		pComponent->GetInterpolator().SetTime( prevstate->GetTimeInState() );
		pComponent->GetBlendInterpolator().SetTime( 0.0f );
	}
	else
	{
		pComponent->GetInterpolator().SetTime( 0.0f );
		pComponent->GetBlendInterpolator().SetTime( 0.0f );
	}

	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_Blender.SetCurrentTime( 0.0f );
	m_Blender.SetIsDoneInterp( false );
	m_Blender.SetTransitionTime( 0.2f );
	m_Blender.SetInterpolators( &pComponent->GetInterpolator(), &pComponent->GetBlendInterpolator() );
}

// The actual processing of Animation KeyFrames.
void PudgeRunToIdleState::Execute( CComponent_Animation* pComponent, float fTime )
{
	m_fTimeInState += fTime;

	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( fTime );
	// For animation Blending Process the Animation wish to blend to as well.
	pComponent->GetBlendInterpolator().AddTime( fTime );

	// Determine if object is inside the Frustum otherwise dont process anything no need to skin.
	if( !pComponent->IsObjectInFrustum() )
		return;

	// Process The Animation
	pComponent->GetInterpolator().Process();
	// Process the Animation to Blend
	pComponent->GetBlendInterpolator().Process();

	m_Blender.AddCurrentTime( fTime );
	m_Blender.Dissolve();

	// Resize the Frame to store all of the intepolated frames.
	// Multiply by the inverse Bind Pose.
	pComponent->GetAnimationFrames().resize( m_Blender.GetFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&(pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix * m_Blender.GetFrames()[i]));

	if( m_Blender.isDoneInterp() )
	{
		pComponent->GetAnimStateMachine()->ChangeState( PudgeIdleState::GetState() );
	}
}

// Clean up of the Animation.
void PudgeRunToIdleState::Exit( CComponent_Animation* pComponent )
{

}