#ifndef BUTTONIDLESTATE_H_
#define BUTTONIDLESTATE_H_

#include "State.h"

__declspec(align(32))
class ButtonIdleState : public State
{
private:
	ButtonIdleState(void){}
	ButtonIdleState( const ButtonIdleState& ) {}
	const ButtonIdleState& operator = (const ButtonIdleState& ) {}
public:
	~ButtonIdleState(void);

	static ButtonIdleState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif