#ifndef DEATHCLOUDPUDGEDEATHSTATE_H_
#define DEATHCLOUDPUDGEDEATHSTATE_H_

#include "State.h"

__declspec(align(32))
class DeathCloudPudgeDeathState : public State
{
private:
	DeathCloudPudgeDeathState(void){}
	DeathCloudPudgeDeathState( const DeathCloudPudgeDeathState& ) {}
	const DeathCloudPudgeDeathState& operator = (const DeathCloudPudgeDeathState& ) {}
public:
	~DeathCloudPudgeDeathState(void);

	static DeathCloudPudgeDeathState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif