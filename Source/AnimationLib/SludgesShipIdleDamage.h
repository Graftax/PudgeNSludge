#ifndef SLUDGESSHIPIDLEDAMAGE_H_
#define SLUDGESSHIPIDLEDAMAGE_H_

#include "State.h"

__declspec(align(32))
class SludgesShipIdleDamage : public State
{
private:
	SludgesShipIdleDamage(void){}
	SludgesShipIdleDamage( const SludgesShipIdleDamage& ) {}
	const SludgesShipIdleDamage& operator = (const SludgesShipIdleDamage& ) {}
public:
	~SludgesShipIdleDamage(void);

	static SludgesShipIdleDamage* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif