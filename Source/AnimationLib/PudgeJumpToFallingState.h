#ifndef PUDGEJUMPTOFALLINGSTATE_H_
#define PUDGEJUMPTOFALLINGSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeJumpToFallingState : public State
{
private:
	PudgeJumpToFallingState(void){}
	PudgeJumpToFallingState( const PudgeJumpToFallingState& ) {}
	const PudgeJumpToFallingState& operator = (const PudgeJumpToFallingState& ) {}
public:
	~PudgeJumpToFallingState(void);

	static PudgeJumpToFallingState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif