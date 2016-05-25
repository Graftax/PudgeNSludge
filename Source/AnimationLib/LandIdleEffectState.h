#ifndef LANDIDLEEFFECTSTATE_H_
#define LANDIDLEEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class LandIdleEffectState : public State
{
private:
	LandIdleEffectState(void){}
	LandIdleEffectState( const LandIdleEffectState& ) {}
	const LandIdleEffectState& operator = (const LandIdleEffectState& ) {}
public:
	~LandIdleEffectState(void);

	static LandIdleEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif