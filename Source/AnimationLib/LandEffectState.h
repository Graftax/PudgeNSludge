#ifndef LANDEFFECTSTATE_H_
#define LANDEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class LandEffectState : public State
{
private:
	LandEffectState(void){}
	LandEffectState( const LandEffectState& ) {}
	const LandEffectState& operator = (const LandEffectState& ) {}
public:
	~LandEffectState(void);

	static LandEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif