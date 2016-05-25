#ifndef ANINBOXESSTATE_H_
#define ANINBOXESSTATE_H_

#include "State.h"

__declspec(align(32))
class AnimBoxesState : public State
{
private:
	AnimBoxesState(void){}
	AnimBoxesState( const AnimBoxesState& ) {}
	const AnimBoxesState& operator = (const AnimBoxesState& ) {}
public:
	~AnimBoxesState(void);

	static AnimBoxesState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif