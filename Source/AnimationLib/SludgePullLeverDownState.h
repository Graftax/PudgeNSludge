#ifndef SLUDGEPULLLEVERDOWNSTATE_H_
#define SLUDGEPULLLEVERDOWNSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgePullLeverDownState : public State
{
private:
	SludgePullLeverDownState(void){}
	SludgePullLeverDownState( const SludgePullLeverDownState& ) {}
	const SludgePullLeverDownState& operator = (const SludgePullLeverDownState& ) {}
public:
	~SludgePullLeverDownState(void);

	static SludgePullLeverDownState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif