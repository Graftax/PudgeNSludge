#ifndef PUDGEIDLETOJUMPSTATE_H_
#define PUDGEIDLETOJUMPSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeIdleToJumpState : public State
{
private:
	PudgeIdleToJumpState(void){}
	PudgeIdleToJumpState( const PudgeIdleToJumpState& ) {}
	const PudgeIdleToJumpState& operator = (const PudgeIdleToJumpState& ) {}
public:
	~PudgeIdleToJumpState(void);

	static PudgeIdleToJumpState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif