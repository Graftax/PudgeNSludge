#ifndef BIGHURTEFFECTSTATE_H_
#define BIGHURTEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class BigHurtEffectState : public State
{
private:
	BigHurtEffectState(void){}
	BigHurtEffectState( const BigHurtEffectState& ) {}
	const BigHurtEffectState& operator = (const BigHurtEffectState& ) {}
public:
	~BigHurtEffectState(void);

	static BigHurtEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif