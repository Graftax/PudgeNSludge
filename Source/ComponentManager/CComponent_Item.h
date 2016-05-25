#ifndef CCOMPONENT_ITEMH
#define CCOMPONENT_ITEMH

#include "IComponent.h"

__declspec(align(32))
class CComponent_Item : public IComponent
{
	IBaseObject* m_pTarget;
	bool m_bAttached;
	float m_fSpawnIgnoreTimer;

public:

	CComponent_Item() : IComponent( ECOMP_ITEM ), m_pTarget( nullptr ) { m_bAttached = false; }

	bool Initialize(void);
	void Update( float fDT );

	///////////////
	// accessors //
	///////////////
	float GetSpawnIgnoreTime() { return m_fSpawnIgnoreTimer; }
	bool GetAttached(void) { return m_bAttached; }
	IBaseObject* GetTarget() { return m_pTarget; }

	//////////////
	// mutators //
	//////////////
	void SetSpawnIgnoreTime(float _ignoreTime) { m_fSpawnIgnoreTimer = _ignoreTime; }
	void SetTarget( IBaseObject* _pTarget ) { m_pTarget = _pTarget; }
	void SetAttached( bool _value ) { m_bAttached = _value; }
};

#endif // CCOMPONENT_ITEMH