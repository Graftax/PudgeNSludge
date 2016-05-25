#ifndef BRIDGEHALFRETRACTIDLESTATESTATE_H_
#define BRIDGEHALFRETRACTIDLESTATESTATE_H_
#include "State.h"

__declspec(align(32))
class BridgeHalfRetractIdleState : public State
{
private:
	BridgeHalfRetractIdleState(void){}
	BridgeHalfRetractIdleState( const BridgeHalfRetractIdleState& ) {}
	const BridgeHalfRetractIdleState& operator = (const BridgeHalfRetractIdleState& ) {}
public:
	~BridgeHalfRetractIdleState(void);

	static BridgeHalfRetractIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};
#endif