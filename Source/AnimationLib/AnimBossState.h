#ifndef ANIMBOSSIDLE_H_
#define ANIMBOSSIDLE_H_

#include "State.h"

__declspec(align(32))
class AnimBossState : public State
{
private:
	AnimBossState(void){}
	AnimBossState( const AnimBossState& ) {}
	const AnimBossState& operator = (const AnimBossState& ) {}
public:
	~AnimBossState(void);

	static AnimBossState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif