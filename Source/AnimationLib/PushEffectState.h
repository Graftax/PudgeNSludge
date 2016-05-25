#ifndef PUSHEFFECTSTATE_H_
#define PUSHEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class PushEffectState : public State
{
private:
	PushEffectState(void){}
	PushEffectState( const PushEffectState& ) {}
	const PushEffectState& operator = (const PushEffectState& ) {}
public:
	~PushEffectState(void);

	static PushEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif