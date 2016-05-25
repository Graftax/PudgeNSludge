#ifndef JUMPEFFECTSTATE_H_
#define JUMPEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class JumpEffectState : public State
{
private:
	JumpEffectState(void){}
	JumpEffectState( const JumpEffectState& ) {}
	const JumpEffectState& operator = (const JumpEffectState& ) {}
public:
	~JumpEffectState(void);

	static JumpEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif