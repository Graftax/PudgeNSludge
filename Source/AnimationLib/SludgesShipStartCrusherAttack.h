#ifndef SLUDGESSHIPSTARTCRUSHERATTACKSTATE_H_
#define SLUDGESSHIPSTARTCRUSHERATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipStartCrusherAttackState : public State
{
private:
	SludgesShipStartCrusherAttackState(void){}
	SludgesShipStartCrusherAttackState( const SludgesShipStartCrusherAttackState& ) {}
	const SludgesShipStartCrusherAttackState& operator = (const SludgesShipStartCrusherAttackState& ) {}
public:
	~SludgesShipStartCrusherAttackState(void);

	static SludgesShipStartCrusherAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif