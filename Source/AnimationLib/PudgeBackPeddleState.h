#ifndef PUDGEBACKPEDDLESTATE_H_
#define PUDGEBACKPEDDLESTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeBackPeddleState : public State
{
private:
	PudgeBackPeddleState(void){}
	PudgeBackPeddleState( const PudgeBackPeddleState& ) {}
	const PudgeBackPeddleState& operator = (const PudgeBackPeddleState& ) {}
public:
	~PudgeBackPeddleState(void);

	static PudgeBackPeddleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif