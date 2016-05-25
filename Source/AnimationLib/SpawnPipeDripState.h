#ifndef SPAWNPIPEDRIPSTATE_H_
#define SPAWNPIPEDRIPSTATE_H_

#include "State.h"

__declspec(align(32))
class SpawnPipeDripState : public State
{
private:
	SpawnPipeDripState(void){}
	SpawnPipeDripState( const SpawnPipeDripState& ) {}
	const SpawnPipeDripState& operator = (const SpawnPipeDripState& ) {}
public:
	~SpawnPipeDripState(void);

	static SpawnPipeDripState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif