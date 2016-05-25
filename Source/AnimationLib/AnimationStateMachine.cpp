#include "AnimationStateMachine.h"
#include "../ComponentManager/CComponent_Animation.h"
#include "../Utilities/Logger/Logger.h"

AnimationStateMachine::AnimationStateMachine(void)
{
	m_pComponent = NULL;
	m_pCurrentState = m_pPreviousState = NULL;
	fTimeInCurrentState = 0.0f;
}

AnimationStateMachine::AnimationStateMachine(CComponent_Animation* pComponent)
{
	m_pComponent = pComponent;
	m_pCurrentState = m_pPreviousState = NULL;
	fTimeInCurrentState = 0.0f;
}

AnimationStateMachine::~AnimationStateMachine(void)
{
}

//////////////////////////////////////////////////////////////////
//	Update():	Updates the Current State of the Components 
//				State Machine.
//
//	Returns:	void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void AnimationStateMachine::Update(float fTime)
{
	static bool DidMessagePrint = false;
	if( !m_pCurrentState )
	{
		if( !DidMessagePrint )
		{
			LOG(LOG_ERROR, "AnimationStateMachine::Update", "Current State not set. Could Potentially Crash Game.");
			DidMessagePrint = true;
		}
		return;

	}

	fTimeInCurrentState += fTime;
	m_pCurrentState->Execute( m_pComponent, fTime);
}

//////////////////////////////////////////////////////////////////
//	ChangeState():	Changes the state of the State Machine and 
//					shuts down the previous state.
//
//	Returns:	void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void AnimationStateMachine::ChangeState(State* pNewState)
{
	if( pNewState == m_pCurrentState )
		return;

	m_pPreviousState = m_pCurrentState;
	if( m_pCurrentState )
		m_pCurrentState->Exit(m_pComponent);
	m_pCurrentState = pNewState;
	fTimeInCurrentState = 0.0f;
	m_pCurrentState->Enter( m_pComponent );
}

//////////////////////////////////////////////////////////////////
//	RevertToPreviousState():	Changes the state of the State Machine
//								back to the previous state.
//
//	Returns:	void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void AnimationStateMachine::RevertToPreviousState()
{
	ChangeState(m_pPreviousState);
}