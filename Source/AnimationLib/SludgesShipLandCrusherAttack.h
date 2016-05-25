#ifndef SLUDGESSHIPLANDCRUSHERATTACKSTATE_H_
#define SLUDGESSHIPLANDCRUSHERATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipLandCrusherAttackState : public State
{
private:
	SludgesShipLandCrusherAttackState(void){}
	SludgesShipLandCrusherAttackState( const SludgesShipLandCrusherAttackState& ) {}
	const SludgesShipLandCrusherAttackState& operator = (const SludgesShipLandCrusherAttackState& ) {}
public:
	~SludgesShipLandCrusherAttackState(void);

	static SludgesShipLandCrusherAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif