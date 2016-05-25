#ifndef DOORIDLE_H_
#define DOORIDLE_H_

#include "State.h"

__declspec(align(32))
class DoorIdleState : public State
{
private:
	DoorIdleState(void){}
	DoorIdleState( const DoorIdleState& ) {}
	const DoorIdleState& operator = (const DoorIdleState& ) {}
public:
	~DoorIdleState(void);

	static DoorIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif