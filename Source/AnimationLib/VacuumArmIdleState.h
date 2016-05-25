#ifndef VACUUMARMIDLESTATE_H_
#define VACUUMARMIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class VacuumArmIdleState : public State
{
private:
	VacuumArmIdleState(void){}
	VacuumArmIdleState( const VacuumArmIdleState& ) {}
	const VacuumArmIdleState& operator = (const VacuumArmIdleState& ) {}
public:
	~VacuumArmIdleState(void);

	static VacuumArmIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif