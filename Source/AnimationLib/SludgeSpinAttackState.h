#ifndef SLUDGESPINATTACKSTATE_H_
#define SLUDGESPINATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgeSpinAttackState : public State
{
private:
	SludgeSpinAttackState(void){}
	SludgeSpinAttackState( const SludgeSpinAttackState& ) {}
	const SludgeSpinAttackState& operator = (const SludgeSpinAttackState& ) {}
public:
	~SludgeSpinAttackState(void);

	static SludgeSpinAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif