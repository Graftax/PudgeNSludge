#ifndef CCOMPONENT_FLASHER_H
#define CCOMPONENT_FLASHER_H

#include "IComponent.h"

__declspec( align(32))
class CComponent_Flasher : public IComponent
{

private:

	float m_fTimeLeft;
	float m_fFlashTimer;
	bool m_bActive;

public:

	CComponent_Flasher();

	bool Initialize( void );

	void Update( float fDT );

	void Shutdown();

	void SetTimeLeft( float _time );

	void SetActive(bool _active) { m_bActive = _active; }
};

#endif // CCOMPONENT_FLASHER_H