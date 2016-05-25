#ifndef PUDGEPULLSTATE_H_
#define PUDGEPULLSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgePullState : public State
{
private:
	PudgePullState(void){}
	PudgePullState( const PudgePullState& ) {}
	const PudgePullState& operator = (const PudgePullState& ) {}
public:
	~PudgePullState(void);

	static PudgePullState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif