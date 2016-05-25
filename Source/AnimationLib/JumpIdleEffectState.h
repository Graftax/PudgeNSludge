#ifndef JUMPIDLEEFFECTSTATE_H_
#define JUMPIDLEEFFECTSTATE_H_

#include "State.h"

__declspec(align(32))
class JumpIdleEffectState : public State
{
private:
	JumpIdleEffectState(void){}
	JumpIdleEffectState( const JumpIdleEffectState& ) {}
	const JumpIdleEffectState& operator = (const JumpIdleEffectState& ) {}
public:
	~JumpIdleEffectState(void);

	static JumpIdleEffectState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif