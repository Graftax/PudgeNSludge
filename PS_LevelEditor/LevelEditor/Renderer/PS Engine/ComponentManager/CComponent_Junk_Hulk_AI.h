#ifndef CCOMPONENT_JUNK_HULK_H
#define CCOMPONENT_JUNK_HULK_H

#include "CComponent_AI.h"

class CComponent_Junk_Hulk_AI : public CComponent_AI
{
private:
	bool m_bStuned;

	void Attack ();
	void Move ();
	void Move (IBaseObject* _pTarget);
	void Avoide (IBaseObject* _pHazard);
	void Damaged ();

public:
	CComponent_Junk_Hulk_AI ();
	~CComponent_Junk_Hulk_AI ();

	bool Initialize ();
	void Shutdown ();

	void Update ( float _fElapsedTime );
};
#endif