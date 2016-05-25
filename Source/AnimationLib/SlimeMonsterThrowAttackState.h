#ifndef SLIMEMONSTERTHROWATTACKSTATE_H_
#define SLIMEMONSTERTHROWATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterThrowAttackState : public State
{
private:
	SlimeMonsterThrowAttackState(void){}
	SlimeMonsterThrowAttackState( const SlimeMonsterThrowAttackState& ) {}
	const SlimeMonsterThrowAttackState& operator = (const SlimeMonsterThrowAttackState& ) {}
public:
	~SlimeMonsterThrowAttackState(void);

	static SlimeMonsterThrowAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif