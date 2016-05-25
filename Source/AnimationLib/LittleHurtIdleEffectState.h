#ifndef LITTLEHURTIDLEEFFECTSTATE_H_
#define LITTLEHURTIDLEEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class LittleHurtIdleEffectState : public State
{
private:
	LittleHurtIdleEffectState(void){}
	LittleHurtIdleEffectState( const LittleHurtIdleEffectState& ) {}
	const LittleHurtIdleEffectState& operator = (const LittleHurtIdleEffectState& ) {}
public:
	~LittleHurtIdleEffectState(void);

	static LittleHurtIdleEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif