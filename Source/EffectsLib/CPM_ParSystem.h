#ifndef CPM_PARSYSTEM_H
#define CPM_PARSYSTEM_H

#include <vector>

#include "CPM_ParEmitter.h"

class CPM_ParSystem
{

private:

	std::vector< CPM_ParEmitter > m_vecEmitters;

public:

	// Add more particles into the system, if possible.
	void Inject();

};

#endif