#ifndef CCOMPONENT_MENUSYSTEM
#define CCOMPONENT_MENUSYSTEM

#include "IComponent.h"

__declspec( align(32))
class CComponent_MenuSystem : public IComponent
{

private:

public:

	CComponent_MenuSystem();

	bool Initialize( void );

	void Update( float fDT );

	void Shutdown();
};

#endif