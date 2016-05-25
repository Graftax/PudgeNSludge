#include "CPM_Particle.h"

CPM_Particle::CPM_Particle()
{
	m_bAlive = false;
	m_fEnergy = 0.0f;
	m_fDuration = 0.0f;
	m_fPos = vec3f( 0.0f, 0.0f, 0.0f );
	m_fPrevPos = vec3f( 0.0f, 0.0f, 0.0f );

	m_pEmitterParent = nullptr;
	m_pNext = nullptr;
	m_pPrev = nullptr;
}