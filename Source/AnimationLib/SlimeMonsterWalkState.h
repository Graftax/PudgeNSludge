#ifndef SLIMEMONSTERWALKSTATE_H_
#define SLIMEMONSTERWALKSTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterWalkState : public State
{
private:
	SlimeMonsterWalkState(void){}
	SlimeMonsterWalkState( const SlimeMonsterWalkState& ) {}
	const SlimeMonsterWalkState& operator = (const SlimeMonsterWalkState& ) {}
public:
	~SlimeMonsterWalkState(void);

	static SlimeMonsterWalkState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif