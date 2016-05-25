#ifndef PUDGEWALLJUMPSTATE_H_
#define PUDGEWALLJUMPSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeWallJumpState : public State
{
private:
	PudgeWallJumpState(void){}
	PudgeWallJumpState( const PudgeWallJumpState& ) {}
	const PudgeWallJumpState& operator = (const PudgeWallJumpState& ) {}
public:
	~PudgeWallJumpState(void);

	static PudgeWallJumpState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif