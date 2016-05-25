#ifndef SLUDGELANDCRUSHERATTACKSTATE_H_
#define SLUDGELANDCRUSHERATTACKSTATE_H_

#include "State.h"

__declspec(align(32))
class SludgeLandCrusherAttackState : public State
{
private:
	SludgeLandCrusherAttackState(void){}
	SludgeLandCrusherAttackState( const SludgeLandCrusherAttackState& ) {}
	const SludgeLandCrusherAttackState& operator = (const SludgeLandCrusherAttackState& ) {}
public:
	~SludgeLandCrusherAttackState(void);

	static SludgeLandCrusherAttackState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif