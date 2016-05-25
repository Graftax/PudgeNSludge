#ifndef BRIDGEHALFRETRACT_H_
#define BRIDGEHALFRETRACT_H_

#include "State.h"

__declspec(align(32))
class BridgeHalfRetractState : public State
{
private:
	BridgeHalfRetractState(void){}
	BridgeHalfRetractState( const BridgeHalfRetractState& ) {}
	const BridgeHalfRetractState& operator = (const BridgeHalfRetractState& ) {}
public:
	~BridgeHalfRetractState(void);

	static BridgeHalfRetractState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif