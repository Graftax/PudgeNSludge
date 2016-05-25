#ifndef PUDGEDEATHSTATE_H_
#define PUDGEDEATHSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeDeathState : public State
{
private:
	PudgeDeathState(void){}
	PudgeDeathState( const PudgeDeathState& ) {}
	const PudgeDeathState& operator = (const PudgeDeathState& ) {}
public:
	~PudgeDeathState(void);

	static PudgeDeathState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif