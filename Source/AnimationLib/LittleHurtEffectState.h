#ifndef LITTLEHURTEFFECTSTATE_H_
#define LITTLEHURTEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class LittleHurtEffectState : public State
{
private:
	LittleHurtEffectState(void){}
	LittleHurtEffectState( const LittleHurtEffectState& ) {}
	const LittleHurtEffectState& operator = (const LittleHurtEffectState& ) {}
public:
	~LittleHurtEffectState(void);

	static LittleHurtEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif