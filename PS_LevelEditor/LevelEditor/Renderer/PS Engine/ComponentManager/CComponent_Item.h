#ifndef CCOMPONENT_ITEMH
#define CCOMPONENT_ITEMH

#include "IComponent.h"

__declspec(align(32))
class CComponent_Item : public IComponent
{
	IBaseObject* m_pTarget;
	bool m_bAttached;

public:

	CComponent_Item() : IComponent( ECOMP_ITEM ), m_pTarget( nullptr ) { m_bAttached = false; }

	IBaseObject* GetTarget() { return m_pTarget; }
	void SetTarget( IBaseObject* _pTarget ) { m_pTarget = _pTarget; }
	void SetAttached( bool _value ) { m_bAttached = _value; }

	bool Initialize(void);
	void Update( float fDT );

	bool GetAttached(void) { return m_bAttached; }
};

#endif // CCOMPONENT_ITEMH