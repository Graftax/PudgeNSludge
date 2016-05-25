#ifndef STATE_H_
#define STATE_H_

#include "AnimationBlender.h"
#include "../ComponentManager/CComponent_Animation.h"

__declspec(align(32))
class State
{
protected:
	float				m_fTimeInState;				// The time in the Animation State.
	float				m_fPlaybackSpeed;			// The Playback Speed of the Animation.
	AnimationBlender	m_Blender;					// The Blender used to blend between animations, not always used.
public:
	State(void) : m_fPlaybackSpeed(1.0f){}

	int m_nInterpolatorsCurrentFrame;
	int m_nBlendInterpsCurrentFrame;

	virtual void Enter(CComponent_Animation* pComponent) = 0;
	virtual void Execute(CComponent_Animation* pComponent, float fTime) = 0;
	virtual void Exit(CComponent_Animation* pComponent) = 0;

	~State(void){}

	// Accessors
	float GetTimeInState(void) { return m_fTimeInState; }

	// Mutators
	// This function will reset the animation by setting the interpolators time to 0.0f
	void ResetAnimation ( CComponent_Animation* pComponent ) { pComponent->GetInterpolator ().SetTime (0.0f); pComponent->GetInterpolator().SetIsAnimationDone (false); }
};

#endif