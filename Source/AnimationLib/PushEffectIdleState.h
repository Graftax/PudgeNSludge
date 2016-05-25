#ifndef PUSHEFFECTIDLESTATE_H_
#define PUSHEFFECTIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class PushEffectIdleState : public State
{
private:
	PushEffectIdleState(void){}
	PushEffectIdleState( const PushEffectIdleState& ) {}
	const PushEffectIdleState& operator = (const PushEffectIdleState& ) {}
public:
	~PushEffectIdleState(void);

	static PushEffectIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif