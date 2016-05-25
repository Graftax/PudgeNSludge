#ifndef PUDGESMALLDAMAGESTATE_H_
#define PUDGESMALLDAMAGESTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeSmallDamageState : public State
{
private:
	PudgeSmallDamageState(void){}
	PudgeSmallDamageState( const PudgeSmallDamageState& ) {}
	const PudgeSmallDamageState& operator = (const PudgeSmallDamageState& ) {}
public:
	~PudgeSmallDamageState(void);

	static PudgeSmallDamageState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif