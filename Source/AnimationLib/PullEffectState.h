#ifndef PULLEFFECTSTATE_H_
#define PULLEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class PullEffectState : public State
{
private:
	PullEffectState(void){}
	PullEffectState( const PullEffectState& ) {}
	const PullEffectState& operator = (const PullEffectState& ) {}
public:
	~PullEffectState(void);

	static PullEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif