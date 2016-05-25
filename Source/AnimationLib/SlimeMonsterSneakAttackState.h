#ifndef SLIMEMONSTERSNEAKATTACKSTATE_H_
#define SLIMEMONSTERSNEAKATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterSneakAttackState : public State
{
private:
	SlimeMonsterSneakAttackState(void){}
	SlimeMonsterSneakAttackState( const SlimeMonsterSneakAttackState& ) {}
	const SlimeMonsterSneakAttackState& operator = (const SlimeMonsterSneakAttackState& ) {}
public:
	~SlimeMonsterSneakAttackState(void);

	static SlimeMonsterSneakAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif