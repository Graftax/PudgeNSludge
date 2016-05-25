#ifndef PUDGEFALLINGSTATE_H_
#define PUDGEFALLINGSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeFallingState : public State
{
private:
	PudgeFallingState(void){}
	PudgeFallingState( const PudgeFallingState& ) {}
	const PudgeFallingState& operator = (const PudgeFallingState& ) {}
public:
	~PudgeFallingState(void);

	static PudgeFallingState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif