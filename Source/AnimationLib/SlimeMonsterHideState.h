#ifndef SLIMEMONSTERHIDESTATE_H_
#define SLIMEMONSTERHIDESTATE_H_

#include "State.h"

__declspec(align(32))
class SlimeMonsterHideState : public State
{
private:
	SlimeMonsterHideState(void){}
	SlimeMonsterHideState( const SlimeMonsterHideState& ) {}
	const SlimeMonsterHideState& operator = (const SlimeMonsterHideState& ) {}
public:
	~SlimeMonsterHideState(void);

	static SlimeMonsterHideState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif //SLIMEMONSTERHIDESTATE_H_