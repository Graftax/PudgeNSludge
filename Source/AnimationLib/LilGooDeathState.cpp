#include "AnimationIncludes.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

LilGooDeathState::~LilGooDeathState(void)
{
}

// Access the state to set it in the state machine for each component.
LilGooDeathState* LilGooDeathState::GetState(void)
{
	static LilGooDeathState state;
	return &state;
}

// This will set all variables needed for this animation.
void LilGooDeathState::Enter( CComponent_Animation* pComponent )
{
	pComponent->GetInterpolator().SetAnimation( AnimationManager::GetInstance()->GetAnimation("LilGoo_Death") );
	pComponent->SetBindPose( const_cast<Animation*>(pComponent->GetInterpolator().GetAnimation())->GetBindPose() );

	m_fTimeInState = 0.0f;
	pComponent->GetInterpolator().SetTime( 0.0f );
	m_fPlaybackSpeed = 0.5f;

}

// The actual processing of Animation KeyFrames.
void LilGooDeathState::Execute( CComponent_Animation* pComponent, float fTime )
{
	float ElapsedTime = (fTime * m_fPlaybackSpeed);
	m_fTimeInState += ElapsedTime;
	// Update the time of the Interpolator.
	pComponent->GetInterpolator().AddTime( ElapsedTime );
	// Process The Animation
	pComponent->GetInterpolator().Process();
	// Resize the Frame to store all of the intepolated frames.
	// Multiply by the inverse Bind Pose.
	pComponent->GetAnimationFrames().resize( pComponent->GetInterpolator().GetCurrentBoneFrames().size() );
	for( unsigned int i = 0; i < pComponent->GetAnimationFrames().size(); ++i)
		pComponent->GetAnimationFrames()[i].GetWorldMat() = (*(matrix4f*)&pComponent->GetBindPose()->GetHierarchyList()[i]->m_InverseLocalMatrix) * 
		pComponent->GetInterpolator().GetCurrentBoneFrames()[i].GetWorldMat();

	if( pComponent->GetInterpolator().IsAnimationDone() )
	{
		// Add this object to the list of objects to delete
		CObjectManager::GetInstance()->AddObjectToRemove(pComponent->GetParent());
	}
}

// Clean up of the Animation.
void LilGooDeathState::Exit( CComponent_Animation* pComponent )
{

}