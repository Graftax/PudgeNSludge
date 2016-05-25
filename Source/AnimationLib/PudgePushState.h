#ifndef PUDGEPUSHSTATE_H_
#define PUDGEPUSHSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgePushState : public State
{
private:
	PudgePushState(void){}
	PudgePushState( const PudgePushState& ) {}
	const PudgePushState& operator = (const PudgePushState& ) {}
public:
	~PudgePushState(void);

	static PudgePushState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif