#ifndef PUDGEIDLESTATE_H_
#define PUDGEIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeIdleState : public State
{
private:
	PudgeIdleState(void){}
	PudgeIdleState( const PudgeIdleState& ) {}
	const PudgeIdleState& operator = (const PudgeIdleState& ) {}
public:
	~PudgeIdleState(void);

	static PudgeIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif