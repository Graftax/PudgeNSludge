#ifndef SELECTIONMESHROTATINGSTATE_H_
#define SELECTIONMESHROTATINGSTATE_H_

#include "State.h"

__declspec(align(32))
class SelectionMeshRotatingState : public State
{
private:
	SelectionMeshRotatingState(void){}
	SelectionMeshRotatingState( const SelectionMeshRotatingState& ) {}
	const SelectionMeshRotatingState& operator = (const SelectionMeshRotatingState& ) {}
public:
	~SelectionMeshRotatingState(void);

	static SelectionMeshRotatingState* GetState();

	void Enter(CComponent_Animation* pComponent);
	void Execute(CComponent_Animation* pComponent, float fTime);
	void Exit(CComponent_Animation* pComponent);
};

#endif