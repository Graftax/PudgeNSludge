#include "AnimationIncludes.h"

SludgesShipStartCrusherAttackState::~SludgesShipStartCrusherAttackState(void)
{
}

// Access the state to set it in the state machine for each component.
SludgesShipStartCrusherAttackState* SludgesShipStartCrusherAttackState::GetState(void)
{
	static SludgesShipStartCrusherAttackState state;
	return &state;
}

// This will set all variables needed for this animation.
void SludgesShipStartCrusherAttackState::Enter( CComponent_Animation* pComponent )
{
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("SludgesShip_StartCrusherAttack") );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;
	pComponent->GetInterpolator().SetTime( 0.0f );
	m_fPlaybackSpeed = 3.0;
}

// The actual processing of Animation KeyFrames.
void SludgesShipStartCrusherAttackState::Execute( CComponent_Animation* pComponent, float fTime )
{
	m_fTimeInState += (fTime);
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( fTime * m_fPlaybackSpeed );

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
	// Multiply by the inverse Bind Pose.
}

// Clean up of the Animation.
void SludgesShipStartCrusherAttackState::Exit( CComponent_Animation* pComponent )
{

}