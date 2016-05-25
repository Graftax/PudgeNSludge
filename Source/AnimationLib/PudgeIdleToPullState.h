#ifndef PUDGEIDLETOPULLSTATE_H_
#define PUDGEIDLETOPULLSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeIdleToPullState : public State
{
private:
	PudgeIdleToPullState(void){}
	PudgeIdleToPullState( const PudgeIdleToPullState& ) {}
	const PudgeIdleToPullState& operator = (const PudgeIdleToPullState& ) {}
public:
	~PudgeIdleToPullState(void);

	static PudgeIdleToPullState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif