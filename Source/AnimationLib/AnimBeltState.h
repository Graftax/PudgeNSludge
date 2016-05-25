#ifndef ANIMBELTIDLE_H_
#define ANIMBELTIDLE_H_

#include "State.h"

__declspec(align(32))
class AnimBeltState : public State
{
private:
	AnimBeltState(void){}
	AnimBeltState( const AnimBeltState& ) {}
	const AnimBeltState& operator = (const AnimBeltState& ) {}
public:
	~AnimBeltState(void);

	static AnimBeltState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif