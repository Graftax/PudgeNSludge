#ifndef SLUDGESTEAMATTACKSTATE_H_
#define SLUDGESTEAMATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgeSteamAttackState : public State
{
private:
	SludgeSteamAttackState(void){}
	SludgeSteamAttackState( const SludgeSteamAttackState& ) {}
	const SludgeSteamAttackState& operator = (const SludgeSteamAttackState& ) {}
public:
	~SludgeSteamAttackState(void);

	static SludgeSteamAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif