#ifndef CPM_PARTICLE_H
#define CPM_PARTICLE_H

#include "../Utilities/MathLib/vec3.h"

class CPM_ParEmitter;
class CPM_Particle
{
private:

public:

	bool m_bAlive;
	float m_fEnergy;
	float m_fDuration;
	float m_fScale;
	vec3f m_fPos;
	vec3f m_fVel;
	vec3f m_fPrevPos;
	CPM_ParEmitter* m_pEmitterParent;

	CPM_Particle();

	CPM_Particle* m_pNext;
	CPM_Particle* m_pPrev;

	CPM_Particle* m_pParentNext;
	CPM_Particle* m_pParentPrev;


};

#endif // CPM_PARTICLE_H