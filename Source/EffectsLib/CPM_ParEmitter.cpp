#include "CPM_ParEmitter.h"

#include "ParticleManager.h"

CPM_ParEmitter::CPM_ParEmitter()
{
	uTexHandle = -1;
	m_uAlphaRampHandle = -1;
	m_uColorRampHandle = -1;
	m_uScaleRampHandle = -1;
	m_uRotationRampHandle = -1;

	m_bBurst = false;
	m_bGravityWell = false;
	m_bLocalSpace = false;
	m_bInterpolate = false;
	m_bScreenSpace = false;

	m_nEmitterType = -1;
	m_nMaxCount = -1;
	m_nRenderType = 0;
	m_nSpriteNum = 0;
	m_nParticleRate = 0;
	m_fMinDistance = 0.0f;
	m_fMaxDistance = 0.0f;
	m_fSpread = 0.0f;
	m_fSpeed = 0.0f;
	m_fSpeedRandom = 0.0f;
	m_fLifespan = 0.0f;
	m_fLifespanRandom = 0.0f;
	m_fSpriteTwist = 0.0f;

	m_fRampScaleMin = 0.0f;
	m_fRampScaleMax = 0.0f;
	
	m_fRampRotationMin = 0.0f;
	m_fRampRotationMax = 0.0f;

	m_fLeftoverTime = 0.0f;

	m_fSpriteScale = vec2f( 0.0f, 0.0f );
	m_fDirection = vec3f( 0.0f, 0.0f, 0.0f );
	m_fPosition = vec3f( 0.0f, 0.0f, 0.0f );
	m_fGravity = vec3f( 0.0f, 0.0f, 0.0f );
	m_fTargetPosition = vec3f( 0.0f, 0.0f, 0.0f );
	m_fHalfExtents = vec3f( 0.0f, 0.0f, 0.0f );
}

unsigned int CPM_ParEmitter::GetActiveParticleCount()
{
	return m_pListParticles.size();
}

void CPM_ParEmitter::EntangleParticle( CPM_Particle* _toAdd )
{
	m_pListParticles.push_back( _toAdd );
	_toAdd->m_pEmitterParent = this;
}

void CPM_ParEmitter::DisentangleParticle( CPM_Particle* _toRemove )
{
	m_pListParticles.remove( _toRemove );

	// If there are no particles left.
	if( m_pListParticles.size() < 1 )
	{
		// If this is one shot.
		if( m_bBurst )
		{
			// Then remove it from the list of active particle emitters.
			CParticleManager::GetInstance()->StoreEmitter( this );
		}
	}
}

void CPM_ParEmitter::Emit( float fDT )
{
	CParticleManager::GetInstance()->Emit( this, fDT );
}