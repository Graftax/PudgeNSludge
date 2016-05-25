#ifndef PUDGEBIGDAMAGESTATE_H_
#define PUDGEBIGDAMAGESTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeBigDamageState : public State
{
private:
	PudgeBigDamageState(void){}
	PudgeBigDamageState( const PudgeBigDamageState& ) {}
	const PudgeBigDamageState& operator = (const PudgeBigDamageState& ) {}
public:
	~PudgeBigDamageState(void);

	static PudgeBigDamageState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif