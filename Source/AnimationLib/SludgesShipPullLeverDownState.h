#ifndef SLUDGESSHIPPULLLEVERDOWNSTATE_H_
#define SLUDGESSHIPPULLLEVERDOWNSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipPullLeverDownState : public State
{
private:
	SludgesShipPullLeverDownState(void){}
	SludgesShipPullLeverDownState( const SludgesShipPullLeverDownState& ) {}
	const SludgesShipPullLeverDownState& operator = (const SludgesShipPullLeverDownState& ) {}
public:
	~SludgesShipPullLeverDownState(void);

	static SludgesShipPullLeverDownState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif