#ifndef SLIMEMONSTERSTUNSTATE_H_
#define SLIMEMONSTERSTUNSTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterStunState : public State
{
private:
	SlimeMonsterStunState(void){}
	SlimeMonsterStunState( const SlimeMonsterStunState& ) {}
	const SlimeMonsterStunState& operator = (const SlimeMonsterStunState& ) {}
public:
	~SlimeMonsterStunState(void);

	static SlimeMonsterStunState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif