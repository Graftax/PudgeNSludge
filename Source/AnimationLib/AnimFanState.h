#ifndef ANIMFANSTATE_H_
#define ANIMFANSTATE_H_

#include "State.h"

__declspec(align(32))
class AnimFanState : public State
{
private:
	AnimFanState(void){}
	AnimFanState( const AnimFanState& ) {}
	const AnimFanState& operator = (const AnimFanState& ) {}
public:
	~AnimFanState(void);

	static AnimFanState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif