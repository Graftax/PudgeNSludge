#ifndef DOOROPEN_H_
#define DOOROPEN_H_

#include "State.h"

__declspec(align(32))
class DoorOpenState : public State
{
private:
	DoorOpenState(void){}
	DoorOpenState( const DoorOpenState& ) {}
	const DoorOpenState& operator = (const DoorOpenState& ) {}
	bool m_bPlayed;
public:
	~DoorOpenState(void);

	static DoorOpenState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif