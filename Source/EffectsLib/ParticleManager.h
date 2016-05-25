//////////////////////////////////////////////////////////////////////////////////////
// Filename: CParticleManager.h
// Author: Ethan Pendergraft
// Date: 6/26/12
// Purpose: The manager will load in all particles from files
//////////////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLEMANAGER_H_
#define PARTICLEMANAGER_H_
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <list>

#include "../PS/config.h"
#include "../Utilities/MathLib/vec3.h"
#include "../Camera/Camera.h"
#include "CPM_Particle.h"
#include "CPM_ParEmitter.h"

#define _MAX_PARTICLES (65535)
#define _MAX_EMITTERS (512)
#define _PARTICLE_FILE_PATH "./Resources/Particles/"

struct Particle_Vertex
{
	D3DXVECTOR3 m_fPos;
	D3DXVECTOR2 uv;
	float m_fSize;
	D3DCOLOR col;
	float m_fPercentage;
};

class CPM_ParSystem;

class CParticleManager
{
private:

	// Static Instance Pointer
	static CParticleManager* m_pInstance;

	// The pointer to the device handle
	LPDIRECT3DDEVICE9 m_drxDevice;
	
	// The vertex declaration for particles
	LPDIRECT3DVERTEXDECLARATION9 m_drxParticleDecl;

	// The buffer used for rendering the particles.
	LPDIRECT3DVERTEXBUFFER9 m_drxParticleVerts;

	// The effect used to render the particles.
	LPD3DXEFFECT m_drxParticleEffect;

	// The Library of loaded particles.
	std::map< std::string, CPM_ParEmitter > m_mapEffectLibrary;

	// The allocation of particles.
	CPM_Particle m_Particles[ _MAX_PARTICLES ];

	CPM_Particle* m_pActiveHead;
	CPM_Particle* m_pDeadHead;
	unsigned int m_unActiveParticles;

	CPM_ParEmitter m_Emitters[ _MAX_EMITTERS ];
	std::list< CPM_ParEmitter* > m_plistDeadEmitters;
	std::list< CPM_ParEmitter* > m_plistActiveEmitters;

	// The default emitter.
	//CPM_ParEmitter* m_pEmitterDefault;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Constructors: Default constructor, destructor
	// Returns: void
	// Mod. Name: EP
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CParticleManager(void);
	~CParticleManager(void);
	CParticleManager( const CParticleManager& );
	CParticleManager& operator = ( const CParticleManager& );

	void KillParticle( CPM_Particle* _pToKill );
	CPM_Particle* GetNextDeadParticle( CPM_ParEmitter* _parentEmitter );

	CPM_ParEmitter* GetEffect( std::string _szName );
	
	void Emit( CPM_ParEmitter* _toFire, float fDT );

	CPM_ParEmitter* GetNextEmitter();
	void StoreEmitter( CPM_ParEmitter* _toStore );

	void RenderEmitter( CPM_ParEmitter* _toDraw, CCamera* _cam  );

public:
	
	LPD3DXEFFECT GetEfect() { return m_drxParticleEffect; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: GetInstance
	// Purpose: Returns the instance
	// Original Author: EP
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	static CParticleManager* GetInstance();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: DeleteInstance
	// Purpose: Deletes the instance
	// Original Author: EP
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DeleteInstance ();

	void Initialize( LPDIRECT3DDEVICE9 _device );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update: Updates the particles
	// Returns: void
	// Mod. Name: EP
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float elapsedTime);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render: Renders the particles
	// Returns: void
	// Mod. Name: EP
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render( CCamera* _cam );

	void RenderEmitter( CPM_ParEmitter* _toDraw );

	void Shutdown();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadParticles: Loads a particle effect from file.
	// Returns: void
	// Mod. Name: EP
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadParticleEffect( std::string szName, std::string szFilename );

	void CreateEffectOneShot( std::string szName, vec3f _pos );
	void CreateEffectOneShot( std::string szName, vec3f _pos, vec3f _dir );

	CPM_ParEmitter* CreateEffect( std::string szName, vec3f _pos );

	void DestroyEffect( CPM_ParEmitter* _toDestroy );

	friend class CPM_ParEmitter;

};


#endif