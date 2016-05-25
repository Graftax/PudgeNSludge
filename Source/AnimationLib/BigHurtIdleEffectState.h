#ifndef BIGHURTIDLEEFFECTSTATE_H_
#define BIGHURTIDLEEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class BigHurtIdleEffectState : public State
{
private:
	BigHurtIdleEffectState(void){}
	BigHurtIdleEffectState( const BigHurtIdleEffectState& ) {}
	const BigHurtIdleEffectState& operator = (const BigHurtIdleEffectState& ) {}
public:
	~BigHurtIdleEffectState(void);

	static BigHurtIdleEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif