#ifndef BUTTONCLOSINGSTATE_H_
#define BUTTONCLOSINGSTATE_H_

#include "State.h"

__declspec(align(32))
class ButtonClosingState : public State
{
private:
	ButtonClosingState(void){}
	ButtonClosingState( const ButtonClosingState& ) {}
	const ButtonClosingState& operator = (const ButtonClosingState& ) {}
public:
	~ButtonClosingState(void);

	static ButtonClosingState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif