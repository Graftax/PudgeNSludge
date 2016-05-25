#ifndef ANIMCRATECLAWSTATE_H_
#define ANIMCRATECLAWSTATE_H_

#include "State.h"

__declspec(align(32))
class AnimCrateClawState : public State
{
private:
	AnimCrateClawState(void){}
	AnimCrateClawState( const AnimCrateClawState& ) {}
	const AnimCrateClawState& operator = (const AnimCrateClawState& ) {}
public:
	~AnimCrateClawState(void);

	static AnimCrateClawState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif