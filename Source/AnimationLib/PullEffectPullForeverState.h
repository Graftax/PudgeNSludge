#ifndef PULLEFFECTPULLFOREVERSTATE_H_
#define PULLEFFECTPULLFOREVERSTATE_H_

#include "State.h"

__declspec(align(32))
class PullEffectPullForeverState : public State
{
private:
	PullEffectPullForeverState(void){}
	PullEffectPullForeverState( const PullEffectPullForeverState& ) {}
	const PullEffectPullForeverState& operator = (const PullEffectPullForeverState& ) {}
public:
	~PullEffectPullForeverState(void);

	static PullEffectPullForeverState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif