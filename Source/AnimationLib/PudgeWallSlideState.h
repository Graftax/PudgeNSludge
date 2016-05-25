#ifndef PUDGEWALLSLIDESTATE_H_
#define PUDGEWALLSLIDESTATE_H_

#include "State.h"

__declspec(align(32))
class PudgeWallSlideState : public State
{
private:
	PudgeWallSlideState(void){}
	PudgeWallSlideState( const PudgeWallSlideState& ) {}
	const PudgeWallSlideState& operator = (const PudgeWallSlideState& ) {}
public:
	~PudgeWallSlideState(void);

	static PudgeWallSlideState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif