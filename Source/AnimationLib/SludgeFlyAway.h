#ifndef SLUDGEFLYAWAY_H_
#define SLUDGEFLYAWAY_H_

#include "State.h"

__declspec(align(32))
class SludgeFlyAway : public State
{
private:
	SludgeFlyAway(void){}
	SludgeFlyAway( const SludgeFlyAway& ) {}
	const SludgeFlyAway& operator = (const SludgeFlyAway& ) {}
public:
	~SludgeFlyAway(void);

	static SludgeFlyAway* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif