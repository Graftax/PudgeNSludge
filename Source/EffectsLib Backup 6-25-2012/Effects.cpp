#include "Effects.h"
#include "ParticleManager.h"
#include "Emitter.h"
#include "../Camera/Camera.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEffects::CEffects()
{
	m_nNumEmitters = 0;
	m_nParticleIndex = 0;
	m_vEmitters.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEffects::CEffects(  CEffects& var )
{
	m_bActive = var.m_bActive;
	this->m_vEmitters.clear();
	for (unsigned int i = 0; i < var.m_vEmitters.size(); i++)
	{
		this->m_vEmitters.push_back(var.m_vEmitters[i]);
	}
// 	if(!m_vEmitters.size())
// 	{
// // 		m_vEmitters.resize (var.m_vEmitters.size());
// // 		memcpy(&this->m_vEmitters[0], &var.m_vEmitters[0], sizeof(m_vEmitters[0]) * m_vEmitters.size());
// 	}

// 	for(unsigned int i = 0; i < var.m_vEmitters.size(); i++)
// 	{
// 
// 		if(!m_vEmitters[i].GetParticles()->size())
// 		{
// 			this->m_vEmitters[i].GetParticles()->clear();
// 			m_vEmitters[i].GetParticles()->resize (var.m_vEmitters[i].GetParticles()->size());
// 			memcpy(&m_vEmitters[i].GetParticles()[0], &var.m_vEmitters[i].GetParticles()[0], sizeof(m_vEmitters[i].GetParticles()[0]) * m_vEmitters[i].GetParticles()->size());
// 		}
// 	}
	
	m_nParticleIndex = var.m_nParticleIndex;
	m_nNumEmitters = var.m_nNumEmitters;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEffects::~CEffects()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update: Updates the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEffects::Update( float dt)
{
	for (unsigned int i = 0; i < m_vEmitters.size(); i++)
	{
		m_vEmitters[i].Update(dt);
	}
}
void CEffects::UpdatePointToDirection( float dt)
{
	for (unsigned int i = 0; i < m_vEmitters.size(); i++)
	{
		m_vEmitters[i].UpdatePointToDirection(dt);
	}
}

void CEffects::UpdatePointToPoint( float dt, vec3f start, vec3f end )
{
 	for (unsigned int i = 0; i < m_vEmitters.size(); i++)
 	{
		m_vEmitters[i].UpdatePointToPoint(start, end, dt);
 	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render: Renders the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEffects::Render()
{
  	for (unsigned int i = 0; i < m_vEmitters.size(); i++)
  	{
   		if(m_vEmitters[i].GetEmitterActive() || m_vEmitters[i].GetCurDead() < m_vEmitters[i].GetNumParticles())
   			m_vEmitters[i].Render();

// 	if(m_vEmitters[i].GetEmitterActive() && m_vEmitters[i].GetCurDead() <= m_vEmitters[i].GetNumParticles())
//   			m_vEmitters[i].Render();
  	}
//  	if(m_vEmitters[0].GetEmitterActive() || m_vEmitters[0].GetCurDead() < m_vEmitters[0].GetNumParticles())
//  		m_vEmitters[0].Render();

}

