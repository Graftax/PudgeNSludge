#ifndef SLUDGESSHIPSTEAMATTACKSTATE_H_
#define SLUDGESSHIPSTEAMATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipSteamAttackState : public State
{
private:
	SludgesShipSteamAttackState(void){}
	SludgesShipSteamAttackState( const SludgesShipSteamAttackState& ) {}
	const SludgesShipSteamAttackState& operator = (const SludgesShipSteamAttackState& ) {}
public:
	~SludgesShipSteamAttackState(void);

	static SludgesShipSteamAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif