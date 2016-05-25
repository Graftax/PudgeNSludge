#ifndef LILGOOATTACKSTATE_H_
#define LILGOOATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class LilGooAttackState : public State
{
private:
	LilGooAttackState(void){}
	LilGooAttackState( const LilGooAttackState& ) {}
	const LilGooAttackState& operator = (const LilGooAttackState& ) {}
public:
	~LilGooAttackState(void);

	static LilGooAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif