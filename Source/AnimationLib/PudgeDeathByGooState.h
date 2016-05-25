#ifndef PUDGEDEATHBYGOOSTATE_H_
#define PUDGEDEATHBYGOOSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeDeathByGooState : public State
{
private:
	PudgeDeathByGooState(void){}
	PudgeDeathByGooState( const PudgeDeathByGooState& ) {}
	const PudgeDeathByGooState& operator = (const PudgeDeathByGooState& ) {}
public:
	~PudgeDeathByGooState(void);

	static PudgeDeathByGooState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif