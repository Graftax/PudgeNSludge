#ifndef PUDGERUNSTATE_H_
#define PUDGERUNSTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeRunState : public State
{
private:
	PudgeRunState(void) {}
	PudgeRunState( const PudgeRunState &) {}
	const PudgeRunState& operator = (const PudgeRunState& ) {}
public:
	~PudgeRunState(void);

	static PudgeRunState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif

