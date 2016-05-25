#ifndef SELECTIONMESHSPAWNSTATE_H_
#define SELECTIONMESHSPAWNSTATE_H_

#include "State.h"

__declspec(align(32))
class SelectionMeshSpawnState : public State
{
private:
	SelectionMeshSpawnState(void){}
	SelectionMeshSpawnState( const SelectionMeshSpawnState& ) {}
	const SelectionMeshSpawnState& operator = (const SelectionMeshSpawnState& ) {}
public:
	~SelectionMeshSpawnState(void);

	static SelectionMeshSpawnState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif