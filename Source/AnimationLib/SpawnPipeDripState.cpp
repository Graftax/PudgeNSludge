#include "AnimationIncludes.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

SpawnPipeDripState::~SpawnPipeDripState(void)
{
}

// Access the state to set it in the state machine for each component.
SpawnPipeDripState* SpawnPipeDripState::GetState(void)
{
	static SpawnPipeDripState state;
	return &state;
}

// This will set all variables needed for this animation.
void SpawnPipeDripState::Enter( CComponent_Animation* pComponent )
{
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("SpawnPipeDrip_Drip") );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;
	pComponent->GetInterpolator().SetTime( 0.0f );

	m_fPlaybackSpeed = 2.00f;
}

// The actual processing of Animation KeyFrames.
void SpawnPipeDripState::Execute( CComponent_Animation* pComponent, float fTime )
{
	float Elapsedtime = fTime * m_fPlaybackSpeed;

	m_fTimeInState += Elapsedtime;
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( Elapsedtime );

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

	if( pComponent->GetInterpolator().IsAnimationDone() )
		CObjectManager::GetInstance()->AddObjectToRemove( pComponent->GetParent() );
}

// Clean up of the Animation.
void SpawnPipeDripState::Exit( CComponent_Animation* pComponent )
{

}