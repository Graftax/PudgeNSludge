#ifndef SLUDGESHIPSPINATTACKSTATE_H_
#define SLUDGESHIPSPINATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipSpinAttackState : public State
{
private:
	SludgesShipSpinAttackState(void){}
	SludgesShipSpinAttackState( const SludgesShipSpinAttackState& ) {}
	const SludgesShipSpinAttackState& operator = (const SludgesShipSpinAttackState& ) {}
public:
	~SludgesShipSpinAttackState(void);

	static SludgesShipSpinAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif