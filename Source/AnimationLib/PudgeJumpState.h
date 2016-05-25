#ifndef PUDGEJUMPSTATE_H_
#define PUDGEJUMPSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeJumpState : public State
{
private:
	PudgeJumpState(void){}
	PudgeJumpState( const PudgeJumpState& ) {}
	const PudgeJumpState& operator = (const PudgeJumpState& ) {}
public:
	~PudgeJumpState(void);

	static PudgeJumpState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif