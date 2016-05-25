#ifndef SLIMEMONSTERDEATHSTATE_H_
#define SLIMEMONSTERDEATHSTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterDeathState : public State
{
private:
	SlimeMonsterDeathState(void){}
	SlimeMonsterDeathState( const SlimeMonsterDeathState& ) {}
	const SlimeMonsterDeathState& operator = (const SlimeMonsterDeathState& ) {}
public:
	~SlimeMonsterDeathState(void);

	static SlimeMonsterDeathState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif