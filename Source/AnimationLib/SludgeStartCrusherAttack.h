#ifndef SLUDGESTARTCRUSHERATTACKSTATE_H_
#define SLUDGESTARTCRUSHERATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgeStartCrusherAttackState : public State
{
private:
	SludgeStartCrusherAttackState(void){}
	SludgeStartCrusherAttackState( const SludgeStartCrusherAttackState& ) {}
	const SludgeStartCrusherAttackState& operator = (const SludgeStartCrusherAttackState& ) {}
public:
	~SludgeStartCrusherAttackState(void);

	static SludgeStartCrusherAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif