#include "DoorOpenState.h"
#include "AnimationIncludes.h"
#include "../ComponentManager/CComponent_Sound.h"

DoorOpenState::~DoorOpenState(void)
{
}

// Access the state to set it in the state machine for each component.
DoorOpenState* DoorOpenState::GetState(void)
{
	static DoorOpenState state;
	return &state;
}

// This will set all variables needed for this animation.
void DoorOpenState::Enter( CComponent_Animation* pComponent )
{
	// Set the Animation Play backwards to false.
	pComponent->SetAnimationBackwards( false );

	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation( "Door_Open" ) );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;
	pComponent->GetInterpolator().SetTime( 0.0f );
	m_fPlaybackSpeed = 1.5f;
	m_bPlayed = false;
}

// The actual processing of Animation KeyFrames.
void DoorOpenState::Execute( CComponent_Animation* pComponent, float fTime )
{
	// The Elapsed Time to update the Animation.
	float ElapsedTime = (m_fPlaybackSpeed * fTime);

	if( !pComponent->IsAnimationBackwards() )
	{
		// Switch to idle if this animation is done
		if( pComponent->GetInterpolator().IsAnimationDone() )
		{
			CComponent_Sound* pSoundCmp = (CComponent_Sound*)pComponent->GetParent()->GetComponent(ECOMP_SOUND);
			if(pSoundCmp && !m_bPlayed)
			{
				pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_DOOR_CLOSE);
				m_bPlayed = true;
			}
			return;
		}

	}
	else
	{
		pComponent->GetInterpolator().SetIsAnimationDone(false);
		m_bPlayed = false;
		if( pComponent->GetInterpolator().GetTime() <= ElapsedTime )
		{
			CComponent_Sound* pSoundCmp = (CComponent_Sound*)pComponent->GetParent()->GetComponent(ECOMP_SOUND);
			if(pSoundCmp)
			{
				pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_DOOR_CLOSE);
			}
			//((CComponent_Sound*)pComponent->GetParent()->GetComponent(ECOMP_SOUND))->PlaySound(AK::EVENTS::PLAY_SFX_DOOR_OPEN);
 			pComponent->GetAnimStateMachine()->ChangeState( DoorIdleState::GetState() );
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
	// Resize the Frame to store all of the interpolated frames.
	// Multiply by the inverse Bind Pose.
	pComponent->GetAnimationFrames().resize( pComponent->GetInterpolator().GetCurrentBoneFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix) * 
		pComponent->GetInterpolator().GetCurrentBoneFrames()[i].GetWorldMat();

}

// Clean up of the Animation.
void DoorOpenState::Exit( CComponent_Animation* pComponent )
{

}