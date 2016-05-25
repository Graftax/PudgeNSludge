#ifndef BRIDGEIDLE_H_
#define BRIDGEIDLE_H_

#include "State.h"

__declspec(align(32))
class BridgeIdleState : public State
{
private:
	BridgeIdleState(void){}
	BridgeIdleState( const BridgeIdleState& ) {}
	const BridgeIdleState& operator = (const BridgeIdleState& ) {}
public:
	~BridgeIdleState(void);

	static BridgeIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif