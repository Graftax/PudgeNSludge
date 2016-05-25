#ifndef PUDGERUNTOIDLESTATE_H_
#define PUDGERUNTOIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeRunToIdleState : public State
{
private:
	PudgeRunToIdleState(void){}
	PudgeRunToIdleState( const PudgeRunToIdleState& ) {}
	const PudgeRunToIdleState& operator = (const PudgeRunToIdleState& ) {}
public:
	~PudgeRunToIdleState(void);

	static PudgeRunToIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif