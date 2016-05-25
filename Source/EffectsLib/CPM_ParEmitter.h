#ifndef CPM_PAREMITTER_H
#define CPM_PAREMITTER_H

#include <list>
#include "../Utilities/MathLib/vec2.h"
#include "../Utilities/MathLib/vec3.h"

enum eEmissionType{ ETYPE_DIRECTION, ETYPE_OMNI, ETYPE_BOX };

class CPM_Particle;
class CPM_ParEmitter
{
private:

	std::list< CPM_Particle* > m_pListParticles;

	void EntangleParticle( CPM_Particle* _toAdd );
	void DisentangleParticle( CPM_Particle* _toRemove );

public:

	unsigned int uTexHandle;
	unsigned int m_uAlphaRampHandle;
	unsigned int m_uColorRampHandle;
	unsigned int m_uScaleRampHandle;
	unsigned int m_uRotationRampHandle;

	bool m_bBurst;
	bool m_bGravityWell;
	bool m_bLocalSpace;
	bool m_bInterpolate;
	bool m_bScreenSpace;

	int m_nEmitterType;
	int m_nMaxCount;
	int m_nRenderType;
	int m_nSpriteNum;
	int m_nParticleRate;
	float m_fMinDistance;
	float m_fMaxDistance;
	float m_fSpread;
	float m_fSpeed;
	float m_fSpeedRandom;
	float m_fLifespan;
	float m_fLifespanRandom;
	float m_fSpriteTwist;
	float m_fScale;

	float m_fRampScaleMin;
	float m_fRampScaleMax;

	float m_fRampRotationMin;
	float m_fRampRotationMax;

	float m_fLeftoverTime;

	vec2f m_fSpriteScale;
	vec3f m_fDirection;
	vec3f m_fPosition;
	vec3f m_fGravity;
	vec3f m_fTargetPosition;
	vec3f m_fHalfExtents;

	CPM_ParEmitter();

	unsigned int GetActiveParticleCount();

	void Emit( float fDT );

	friend class CParticleManager;
};

#endif