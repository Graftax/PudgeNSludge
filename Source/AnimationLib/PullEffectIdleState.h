#ifndef PULLEFFECTIDLESTATE_H_
#define PULLEFFECTIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class PullEffectIdleState : public State
{
private:
	PullEffectIdleState(void){}
	PullEffectIdleState( const PullEffectIdleState& ) {}
	const PullEffectIdleState& operator = (const PullEffectIdleState& ) {}
public:
	~PullEffectIdleState(void);

	static PullEffectIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif