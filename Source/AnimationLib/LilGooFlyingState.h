#ifndef LILGOOFLYINGSTATE_H_
#define LILGOOFLYINGSTATE_H_

#include "State.h"

__declspec(align(32))
class LilGooFlyingState : public State
{
private:
	LilGooFlyingState(void){}
	LilGooFlyingState( const LilGooFlyingState& ) {}
	const LilGooFlyingState& operator = (const LilGooFlyingState& ) {}
public:
	~LilGooFlyingState(void);

	static LilGooFlyingState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif