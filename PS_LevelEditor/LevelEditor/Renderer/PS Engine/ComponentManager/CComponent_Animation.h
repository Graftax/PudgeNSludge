#ifndef CCOMPONENT_ANIMATION_H_
#define CCOMPONENT_ANIMATION_H_

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Animation.h
// Author:              Andy Madruga
// Date:                5/14/12
// Purpose:             Defines the Animation data needed to animate a mesh.
//////////////////////////////////////////////////////////////////////////////////////

#include "IComponent.h"
#include "../AnimationLib/AnimationSystem.h"

class AnimationStateMachine;

__declspec(align(32))
class CComponent_Animation : public IComponent
{
private:
	AnimationStateMachine* m_pAnimStateMachine;	// A pointer to the state machine.

	// Interpolates from one keyframe to the next.
	Interpolator m_Interpolator;
	// Interpolates from one keyframe to the next for the next animation.
	Interpolator m_BlendToInterpolator;
	// Used to render the model Animated.
	vector<Frame> m_vBlendedFrames; // The frames to be passed into the shader for hardware skinning.

	BindPose* m_pPose;// The bind pose used for this mesh.

	// Determine whether it should blend backwords.
	bool m_bBlendStateBackward;

public:
	CComponent_Animation();
	~CComponent_Animation();

	//////////////////////////////////////////////////////////////////
	//	Initialize():	Initializes the Component and sets all values
	//					to default values to avoid crashes/bugs.
	//
	//	Returns:		bool - returns true if everything was initialized.
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	bool Initialize(void);

	//////////////////////////////////////////////////////////////////
	//	Update():	Calls Update on the State Machine and updates the
	//				current State of the Animation.
	//
	//	Returns:	void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	void Update( float fDT );

	//////////////////////////////////////////////////////////////////
	//	Initialize():	Shutsdown the component and its state machine.
	//
	//	Returns:		void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	void Shutdown(void);

	// Accessors and Mutators
	AnimationStateMachine* GetAnimStateMachine() { return m_pAnimStateMachine; }

	int GetNumberOfFrames(void) { return m_vBlendedFrames.size(); }
	vector<Frame>& GetAnimationFrames(void) { return m_vBlendedFrames; }

	Interpolator& GetInterpolator(void) { return m_Interpolator; }
	Interpolator& GetBlendInterpolator(void) { return m_BlendToInterpolator; }

	BindPose* GetBindPose(void) { return m_pPose; }
	void SetBindPose( BindPose* pose) { m_pPose = pose; }

	void SetBlendBackwards( bool val ) { m_bBlendStateBackward = val; }
	bool IsBlendBackwards(void) { return m_bBlendStateBackward; }
};

#endif