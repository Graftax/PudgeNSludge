#ifndef ANIMSTATEMACHINE_H_
#define ANIMSTATEMACHINE_H_

// Include all states that will be 
// used in the State Machine.
#include "State.h"

__declspec(align(32))
class AnimationStateMachine
{
private:
	CComponent_Animation*		m_pComponent;
	State*						m_pCurrentState;
	State*						m_pPreviousState;
	float						fTimeInCurrentState;

public:
	AnimationStateMachine(void);
	AnimationStateMachine(CComponent_Animation* pComponent);

	// Destructor
	~AnimationStateMachine(void);


	void SetCurrentState(State* pState) { m_pCurrentState = pState; }
	void SetPreviousState(State* pState){ m_pPreviousState = pState;}

	//////////////////////////////////////////////////////////////////
	//	Update():	Updates the Current State of the Components 
	//				State Machine.
	//
	//	Returns:	void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	void Update(float fTime);

	//////////////////////////////////////////////////////////////////
	//	ChangeState():	Changes the state of the State Machine and 
	//					shuts down the previous state.
	//
	//	Returns:	void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	void ChangeState(State* pNewState);

	//////////////////////////////////////////////////////////////////
	//	RevertToPreviousState():	Changes the state of the State Machine
	//								back to the previous state.
	//
	//	Returns:	void
	//
	//	Modified By:
	//	Modified Date:	
	//////////////////////////////////////////////////////////////////
	void RevertToPreviousState();

	const State* GetCurrentState(){ return m_pCurrentState; }
	const State* GetPreviousState(){ return m_pPreviousState; }
};

#endif