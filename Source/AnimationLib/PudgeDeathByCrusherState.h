#ifndef PUDGEDEATHBYCRUSHERSTATE_H_
#define PUDGEDEATHBYCRUSHERSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeDeathByCrusherState : public State
{
private:
	PudgeDeathByCrusherState(void){}
	PudgeDeathByCrusherState( const PudgeDeathByCrusherState& ) {}
	const PudgeDeathByCrusherState& operator = (const PudgeDeathByCrusherState& ) {}
public:
	~PudgeDeathByCrusherState(void);

	static PudgeDeathByCrusherState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif