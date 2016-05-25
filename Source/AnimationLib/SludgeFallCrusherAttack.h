#ifndef SLUDGEFALLCRUSHERATTACKSTATE_H_
#define SLUDGEFALLCRUSHERATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgeFallCrusherAttackState : public State
{
private:
	SludgeFallCrusherAttackState(void){}
	SludgeFallCrusherAttackState( const SludgeFallCrusherAttackState& ) {}
	const SludgeFallCrusherAttackState& operator = (const SludgeFallCrusherAttackState& ) {}
public:
	~SludgeFallCrusherAttackState(void);

	static SludgeFallCrusherAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif