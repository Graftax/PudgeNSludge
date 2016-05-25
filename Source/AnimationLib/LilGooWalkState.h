#ifndef LILGOOWALKSTATE_H_
#define LILGOOWALKSTATE_H_

#include "State.h"

__declspec(align(32))
class LilGooWalkState : public State
{
private:
	LilGooWalkState(void){}
	LilGooWalkState( const LilGooWalkState& ) {}
	const LilGooWalkState& operator = (const LilGooWalkState& ) {}
public:
	~LilGooWalkState(void);

	static LilGooWalkState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif