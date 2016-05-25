#ifndef SLUDGESSHIPFALLCRUSHERATTACKSTATE_H_
#define SLUDGESSHIPFALLCRUSHERATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipFallCrusherAttackState : public State
{
private:
	SludgesShipFallCrusherAttackState(void){}
	SludgesShipFallCrusherAttackState( const SludgesShipFallCrusherAttackState& ) {}
	const SludgesShipFallCrusherAttackState& operator = (const SludgesShipFallCrusherAttackState& ) {}
public:
	~SludgesShipFallCrusherAttackState(void);

	static SludgesShipFallCrusherAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif