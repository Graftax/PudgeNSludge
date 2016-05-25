#ifndef SLUDGESSHIPIDLESTATE_H_
#define SLUDGESSHIPIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipIdleState : public State
{
private:
	SludgesShipIdleState(void){}
	SludgesShipIdleState( const SludgesShipIdleState& ) {}
	const SludgesShipIdleState& operator = (const SludgesShipIdleState& ) {}
public:
	~SludgesShipIdleState(void);

	static SludgesShipIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif