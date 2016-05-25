#ifndef SLIMEMONSTERIDLESTATE_H_
#define SLIMEMONSTERIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterIdleState : public State
{
private:
	SlimeMonsterIdleState(void){}
	SlimeMonsterIdleState( const SlimeMonsterIdleState& ) {}
	const SlimeMonsterIdleState& operator = (const SlimeMonsterIdleState& ) {}
public:
	~SlimeMonsterIdleState(void);

	static SlimeMonsterIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif