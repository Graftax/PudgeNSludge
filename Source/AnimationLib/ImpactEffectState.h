#ifndef IMPACTEFFECTSTATE_H_
#define IMPACTEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class ImpactEffectState : public State
{
private:
	ImpactEffectState(void){}
	ImpactEffectState( const ImpactEffectState& ) {}
	const ImpactEffectState& operator = (const ImpactEffectState& ) {}
public:
	~ImpactEffectState(void);

	static ImpactEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif