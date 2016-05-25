#ifndef ANIMCLAWSTATE_H_
#define ANIMCLAWSTATE_H_

#include "State.h"

__declspec(align(32))
class AnimClawState : public State
{
private:
	AnimClawState(void){}
	AnimClawState( const AnimClawState& ) {}
	const AnimClawState& operator = (const AnimClawState& ) {}
public:
	~AnimClawState(void);

	static AnimClawState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif