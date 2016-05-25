#ifndef LILGOODEATHSTATE_H_
#define LILGOODEATHSTATE_H_

#include "State.h"

__declspec(align(32))
class LilGooDeathState : public State
{
private:
	LilGooDeathState(void){}
	LilGooDeathState( const LilGooDeathState& ) {}
	const LilGooDeathState& operator = (const LilGooDeathState& ) {}
public:
	~LilGooDeathState(void);

	static LilGooDeathState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif