#ifndef SLUDGEIDLESTATE_H_
#define SLUDGEIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class SludgeIdleState : public State
{
private:
	SludgeIdleState(void){}
	SludgeIdleState( const SludgeIdleState& ) {}
	const SludgeIdleState& operator = (const SludgeIdleState& ) {}
public:
	~SludgeIdleState(void);

	static SludgeIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif