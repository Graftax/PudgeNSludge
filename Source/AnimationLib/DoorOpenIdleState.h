#ifndef DOOROPENIDLESTATE_H_
#define DOOROPENIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class DoorOpenIdleState : public State
{
private:
	DoorOpenIdleState(void){}
	DoorOpenIdleState( const DoorOpenIdleState& ) {}
	const DoorOpenIdleState& operator = (const DoorOpenIdleState& ) {}
public:
	~DoorOpenIdleState(void);

	static DoorOpenIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif