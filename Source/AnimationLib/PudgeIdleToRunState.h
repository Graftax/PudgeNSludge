#ifndef PUDGEIDLETORUNSTATE_H_
#define PUDGEIDLETORUNSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeIdleToRunState : public State
{
private:
	PudgeIdleToRunState(void){}
	PudgeIdleToRunState( const PudgeIdleToRunState& ) {}
	const PudgeIdleToRunState& operator = (const PudgeIdleToRunState& ) {}
public:
	~PudgeIdleToRunState(void);

	static PudgeIdleToRunState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif