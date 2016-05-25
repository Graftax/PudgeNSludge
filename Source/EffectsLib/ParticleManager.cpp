#include "ParticleManager.h"
#include "../Camera/Camera.h"
#include "../RenderEngine/RenderEngine.h"
#include "../RenderEngine/TextureManager.h"
#include "CPM_ParEmitter.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "TinyXML\tinyxml.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#define NEXT(x) (x=x->m_pNext)

CParticleManager* CParticleManager::m_pInstance = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager::CParticleManager(void)
{
	m_pInstance = nullptr;
	m_drxDevice = nullptr;
	m_drxParticleDecl = nullptr;
	m_drxParticleVerts = nullptr;
	m_drxParticleEffect = nullptr;

	m_pActiveHead = nullptr;
	m_pDeadHead = nullptr;
	m_unActiveParticles = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager::~CParticleManager(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns the instance
// Original Author: KC
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager* CParticleManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CParticleManager;
	}
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: DeleteInstance
// Purpose: Deletes the instance
// Original Author: KC
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::DeleteInstance ()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CParticleManager::Initialize( LPDIRECT3DDEVICE9 _device )
{
	// Get at the device.
	m_drxDevice = _device;

	//Setup the decl for the particle.
	D3DVERTEXELEMENT9 decl[] = 
	{ 
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE, 0},
		{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};
	m_drxDevice->CreateVertexDeclaration(decl, &m_drxParticleDecl);

	//Create the buffer for particles. This is as large as it could ever possibly be.
	m_drxDevice->CreateVertexBuffer(_MAX_PARTICLES * sizeof(Particle_Vertex), 
		D3DUSAGE_POINTS, 
		0, D3DPOOL_MANAGED, 
		&m_drxParticleVerts, NULL );

	// Load up the effect for rendering particles.
	ID3DXBuffer *error;
	HRESULT hr = D3DXCreateEffectFromFile(m_drxDevice, (LPCSTR)"./Resources/Shaders/Particle.fx", 0, 0, 0, 0, &m_drxParticleEffect, &error);
	if (error)
	{
		MessageBox(0, (LPCSTR)error->GetBufferPointer(),0,0);
		PostQuitMessage( 1337 );
	}

	// Setup the doubly linked list.
	for( unsigned int i = 0; i < _MAX_PARTICLES; ++i )
	{
		if( i == 0 )
			m_Particles[i].m_pPrev = nullptr;
		else
			m_Particles[i].m_pPrev = &m_Particles[i-1];

		if( i == _MAX_PARTICLES - 1 )
			m_Particles[i].m_pNext = nullptr;
		else
			m_Particles[i].m_pNext =  &m_Particles[i+1];
	}

	//Link the dead pointer to the list.
	m_pDeadHead = &m_Particles[0];

	// Setup the list of emitters.
	for( unsigned int i = 0; i < _MAX_EMITTERS; ++i )
	{
		m_plistDeadEmitters.push_back( &m_Emitters[i] );
	}

	//LoadParticleEffect( string("test"), string("Resources/Particles/test.xml") );
}

void CParticleManager::KillParticle( CPM_Particle* _pToKill )
{
	// Disentangle the particle from the active list.
	if( _pToKill->m_pPrev )
		_pToKill->m_pPrev->m_pNext = _pToKill->m_pNext;
	else
	{
		if( _pToKill == m_pActiveHead )
			m_pActiveHead = _pToKill->m_pNext;
	}

	if( _pToKill->m_pNext )
		_pToKill->m_pNext->m_pPrev = _pToKill->m_pPrev;

	// Remove it from its parents list.
	_pToKill->m_pEmitterParent->DisentangleParticle( _pToKill );

	// Clear it out.
	(*_pToKill) = CPM_Particle();

	// Add it to the dead list.
	if( m_pDeadHead )
	{
		m_pDeadHead->m_pPrev = _pToKill;
		_pToKill->m_pNext = m_pDeadHead;
		m_pDeadHead = _pToKill;
	}
	else
		m_pDeadHead = _pToKill;

}

CPM_Particle* CParticleManager::GetNextDeadParticle( CPM_ParEmitter* _parentEmitter )
{
	if( m_pDeadHead )
	{
		CPM_Particle* pToReturn = m_pDeadHead;
		m_pDeadHead = m_pDeadHead->m_pNext;

		if( m_pDeadHead )
			m_pDeadHead->m_pPrev = nullptr;

		(*pToReturn) = CPM_Particle();

		if( m_pActiveHead )
		{
			m_pActiveHead->m_pPrev = pToReturn;
			pToReturn->m_pNext = m_pActiveHead;
			m_pActiveHead = pToReturn;
		}
		else
			m_pActiveHead = pToReturn;

		pToReturn->m_bAlive = true;
		pToReturn->m_fEnergy = 1.0f;
		pToReturn->m_pEmitterParent = _parentEmitter;
		pToReturn->m_pEmitterParent->EntangleParticle( pToReturn );
		//float percentage = rand() % 10000 / 10000.0f;
		//float fMaxLife = _parentEmitter->m_fLifespan + _parentEmitter->m_fLifespanRandom;
		//float fMinLife = _parentEmitter->m_fLifespan - _parentEmitter->m_fLifespanRandom;
		//pToReturn->m_fDuration = _parentEmitter->m_fLifespan + percentage * ( fMaxLife - fMinLife );


		return pToReturn;
	}
	else
		return nullptr;
}

CPM_ParEmitter* CParticleManager::GetEffect( std::string _szName )
{
	if( m_mapEffectLibrary.count( _szName ) == 1 )
	{
		return &m_mapEffectLibrary[ _szName ];
	}
	else
		return nullptr;
}

void CParticleManager::Emit( CPM_ParEmitter* _toFire, float fDT )
{
	if( !_toFire )
		return;

	float timestep = _toFire->m_nParticleRate * fDT;
	timestep += _toFire->m_fLeftoverTime;

	unsigned int unToEmit = 0;

	while( timestep > 1.0f )
	{
		unToEmit++;
		timestep -= 1.0f;
	}

	_toFire->m_fLeftoverTime = timestep;

	if( _toFire->m_bBurst )
		unToEmit = _toFire->m_nParticleRate;

	if( _toFire->GetActiveParticleCount() + unToEmit > (unsigned int)_toFire->m_nMaxCount )
	{
		unToEmit -= ( _toFire->GetActiveParticleCount() + unToEmit - _toFire->m_nMaxCount );
	}


	if( ( _toFire->m_fPosition - CObjectManager::GetInstance()->GetPlayer()->GetWorldPos() ).magnitude() > 2000.0f )
	{
		if( !_toFire->m_bScreenSpace )
		{
			if( _toFire->m_bBurst )
				unToEmit = 1;
			else
				unToEmit = 0;
		}
	}

	for( unsigned int i = 0; i < unToEmit; ++i )
	{
		CPM_Particle* pPart = GetNextDeadParticle( _toFire );
		if(pPart)
		{
			// Start off with the basic postion.
			if( _toFire->m_bLocalSpace )
				pPart->m_fPos = vec3f( 0.0f, 0.0f, 0.0f );
			else
				pPart->m_fPos = _toFire->m_fPosition;

			// Setup the particle's lifespan.
			float percentage = rand() % 10000 / 10000.0f;
			float fMaxLife = _toFire->m_fLifespan + _toFire->m_fLifespanRandom;
			float fMinLife = _toFire->m_fLifespan - _toFire->m_fLifespanRandom;
			pPart->m_fDuration = fMinLife + percentage * ( fMaxLife - fMinLife );

			// Decide the particle's speed.
			percentage = rand() % 10000 / 10000.0f;
			float fMaxSpeed = _toFire->m_fSpeed + _toFire->m_fSpeedRandom;
			float fMinSpeed = _toFire->m_fSpeed - _toFire->m_fSpeedRandom;
			float fFinalSpeed = fMinSpeed + percentage * ( fMaxSpeed - fMinSpeed );

			vec3f fFinalDirection;

			percentage = rand() % 10000 / 10000.0f;
			float fFinalDistance = _toFire->m_fMinDistance + percentage * ( _toFire->m_fMaxDistance - _toFire->m_fMinDistance );

			// A switch statement for deciding the velocity.
			switch( _toFire->m_nEmitterType )
			{
			case ETYPE_DIRECTION:
				{
					// Setup the preliminary vectors.
					D3DXVECTOR3 d3dvecUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
					D3DXVECTOR3 d3dvecOriginal = D3DXVECTOR3( _toFire->m_fDirection.x, _toFire->m_fDirection.y, _toFire->m_fDirection.z );
					D3DXVECTOR4 d3dvecFinal = D3DXVECTOR4( d3dvecOriginal, 1.0f );

					if( abs( D3DXVec3Dot( &d3dvecOriginal, &d3dvecUp ) ) > 0.9f )
					{
						d3dvecUp = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
					}

					// Find the first rotational axis.
					D3DXVECTOR3 d3dvecRotAxisOne;
					D3DXVec3Cross( &d3dvecRotAxisOne, &d3dvecOriginal, &d3dvecUp );

					//How much are we going to rotate around this axis.
					percentage = rand() % 10000 / 10000.0f;
					float rotationMin = -_toFire->m_fSpread * 1.6f;
					float rotationMax = _toFire->m_fSpread * 1.6f;
					float firstRotFinal = rotationMin + percentage * ( rotationMax - rotationMin );

					// Do the rotation.
					D3DXMATRIX d3dmatFirstRotation;
					D3DXMatrixRotationAxis( &d3dmatFirstRotation, &d3dvecRotAxisOne, firstRotFinal );
					D3DXVec3Transform( &d3dvecFinal, &d3dvecOriginal, &d3dmatFirstRotation );

					// Find the second rotational axis.
					D3DXVECTOR3 d3dvecRotAxisTwo;
					D3DXVec3Cross( &d3dvecRotAxisTwo, &d3dvecOriginal, &d3dvecRotAxisOne );

					//How much are we going to rotate around this axis.
					percentage = rand() % 10000 / 10000.0f;
					float secondRotFinal = rotationMin + percentage * ( rotationMax - rotationMin );

					// Do the rotation.
					D3DXVECTOR3 d3dvecTempFinal = D3DXVECTOR3( d3dvecFinal.x, d3dvecFinal.y, d3dvecFinal.z );
					D3DXMATRIX d3dmatSecondRotation;
					D3DXMatrixRotationAxis( &d3dmatSecondRotation, &d3dvecRotAxisTwo, secondRotFinal );
					D3DXVec3Transform( &d3dvecFinal, &d3dvecTempFinal, &d3dmatSecondRotation );

					fFinalDirection = vec3f( d3dvecFinal.x, d3dvecFinal.y, d3dvecFinal.z );

					// Now that we have the direction, set the speed.
					pPart->m_fVel = fFinalDirection * fFinalSpeed;
					pPart->m_fPos += fFinalDirection * fFinalDistance;

					break;
				}
			case ETYPE_OMNI:
				{
					percentage = rand() % 10000 / 10000.0f;
					fFinalDirection.x = ( percentage - 0.5f ) * 2.0f;
					percentage = rand() % 10000 / 10000.0f;
					fFinalDirection.y = ( percentage - 0.5f ) * 2.0f;
					percentage = rand() % 10000 / 10000.0f;
					fFinalDirection.z = ( percentage - 0.5f ) * 2.0f;
					fFinalDirection.normalize();

					// Now that we have the direction, set the speed.
					pPart->m_fVel = fFinalDirection * fFinalSpeed;
					pPart->m_fPos += fFinalDirection * fFinalDistance;

					break;
				}

			case ETYPE_BOX:
				{
					vec3f offset = vec3f( 0.0f, 0.0f, 0.0f );
					percentage = rand() % 10000 / 10000.0f;
					offset.x = -_toFire->m_fHalfExtents.x + percentage * ( _toFire->m_fHalfExtents.x * 2.0f );
					percentage = rand() % 10000 / 10000.0f;
					offset.y = -_toFire->m_fHalfExtents.y + percentage * ( _toFire->m_fHalfExtents.y * 2.0f );
					percentage = rand() % 10000 / 10000.0f;
					offset.z = -_toFire->m_fHalfExtents.z + percentage * ( _toFire->m_fHalfExtents.z * 2.0f );

					pPart->m_fPos += offset;

					// Setup the preliminary vectors.
					D3DXVECTOR3 d3dvecUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
					D3DXVECTOR3 d3dvecOriginal = D3DXVECTOR3( _toFire->m_fDirection.x, _toFire->m_fDirection.y, _toFire->m_fDirection.z );
					D3DXVECTOR4 d3dvecFinal = D3DXVECTOR4( d3dvecOriginal, 1.0f );

					if( abs( D3DXVec3Dot( &d3dvecOriginal, &d3dvecUp ) ) > 0.9f )
					{
						d3dvecUp = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
					}

					// Find the first rotational axis.
					D3DXVECTOR3 d3dvecRotAxisOne;
					D3DXVec3Cross( &d3dvecRotAxisOne, &d3dvecOriginal, &d3dvecUp );

					//How much are we going to rotate around this axis.
					percentage = rand() % 10000 / 10000.0f;
					float rotationMin = -_toFire->m_fSpread * 1.6f;
					float rotationMax = _toFire->m_fSpread * 1.6f;
					float firstRotFinal = rotationMin + percentage * ( rotationMax - rotationMin );

					// Do the rotation.
					D3DXMATRIX d3dmatFirstRotation;
					D3DXMatrixRotationAxis( &d3dmatFirstRotation, &d3dvecRotAxisOne, firstRotFinal );
					D3DXVec3Transform( &d3dvecFinal, &d3dvecOriginal, &d3dmatFirstRotation );

					// Find the second rotational axis.
					D3DXVECTOR3 d3dvecRotAxisTwo;
					D3DXVec3Cross( &d3dvecRotAxisTwo, &d3dvecOriginal, &d3dvecRotAxisOne );

					//How much are we going to rotate around this axis.
					percentage = rand() % 10000 / 10000.0f;
					float secondRotFinal = rotationMin + percentage * ( rotationMax - rotationMin );

					// Do the rotation.
					D3DXVECTOR3 d3dvecTempFinal = D3DXVECTOR3( d3dvecFinal.x, d3dvecFinal.y, d3dvecFinal.z );
					D3DXMATRIX d3dmatSecondRotation;
					D3DXMatrixRotationAxis( &d3dmatSecondRotation, &d3dvecRotAxisTwo, secondRotFinal );
					D3DXVec3Transform( &d3dvecFinal, &d3dvecTempFinal, &d3dmatSecondRotation );

					fFinalDirection = vec3f( d3dvecFinal.x, d3dvecFinal.y, d3dvecFinal.z );

					pPart->m_fVel = fFinalDirection * fFinalSpeed;

					break;
				}
			}


			if( _toFire->m_bInterpolate )
			{
				pPart->m_fVel = pPart->m_fPos - _toFire->m_fPosition;
			}

			//pTest->m_fVel = vec3f( rand() % 200 - 100, rand() % 200 - 100, rand() % 200 - 100 ).normalize() * 100.0f;

			pPart->m_fScale = _toFire->m_fScale;

			_toFire->EntangleParticle( pPart );
		}
	}
}

CPM_ParEmitter* CParticleManager::GetNextEmitter()
{
	if( m_plistDeadEmitters.size() )
	{
		CPM_ParEmitter* _toReturn = m_plistDeadEmitters.front();
		m_plistDeadEmitters.pop_front();
		m_plistActiveEmitters.push_back( _toReturn );
		return _toReturn;
	}
	else
		return nullptr;
}

void CParticleManager::StoreEmitter( CPM_ParEmitter* _toStore )
{
	if( _toStore )
	{
		*_toStore = CPM_ParEmitter();
		m_plistActiveEmitters.remove( _toStore );
		m_plistDeadEmitters.push_front( _toStore );
	}
}

void CParticleManager::RenderEmitter( CPM_ParEmitter* _toDraw, CCamera* _cam  )
{

	if( !_toDraw->m_pListParticles.size() )
	{
		return;
	}

	if( !_toDraw->m_bScreenSpace )
	{
		Particle_Vertex* _buffer;
		m_drxParticleVerts->Lock( 0, _toDraw->m_pListParticles.size() * sizeof( Particle_Vertex ), (void**)&_buffer, 0 );
		memset( (void*)_buffer, 0, _toDraw->m_pListParticles.size() * sizeof( Particle_Vertex ) );

		std::list< CPM_Particle* >::iterator itrParticle = _toDraw->m_pListParticles.begin();

		if( _toDraw->m_bLocalSpace )
		{
			vec3f offset = _toDraw->m_fPosition;

			while( itrParticle != _toDraw->m_pListParticles.end() )
			{
				//GPU			//CPU
				_buffer->m_fPos			= D3DXVECTOR3( (*itrParticle)->m_fPos.x + offset.x, (*itrParticle)->m_fPos.y + offset.y, (*itrParticle)->m_fPos.z + offset.z );
				_buffer->m_fSize		= (*itrParticle)->m_fScale * 1.25f;//20.0f + ( 1.0f - (*itrParticle)->m_fEnergy ) * ( 122.0f - 20.0f );
				_buffer->col			= D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
				_buffer->m_fPercentage	= (*itrParticle)->m_fEnergy;

				_buffer++;
				itrParticle++;
			}
		}
		else
		{
			while( itrParticle != _toDraw->m_pListParticles.end() )
			{
				//GPU			//CPU
				_buffer->m_fPos			= D3DXVECTOR3( (*itrParticle)->m_fPos.x, (*itrParticle)->m_fPos.y, (*itrParticle)->m_fPos.z );
				_buffer->m_fSize		= (*itrParticle)->m_fScale * 1.25f;//20.0f + ( 1.0f - (*itrParticle)->m_fEnergy ) * ( 122.0f - 20.0f );
				_buffer->col			= D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
				_buffer->m_fPercentage	= (*itrParticle)->m_fEnergy;

				_buffer++;
				itrParticle++;
			}
		}

		m_drxParticleVerts->Unlock();

		m_drxDevice->SetStreamSource( 0, m_drxParticleVerts, 0, sizeof( Particle_Vertex ) );
		m_drxDevice->SetVertexDeclaration( m_drxParticleDecl );

		if( m_drxParticleEffect )
		{
			m_drxParticleEffect->SetTechnique("Basic");
			UINT passes = 0;
			m_drxParticleEffect->Begin( &passes, 0 );
			{
				for( unsigned int i = 0; i < passes; ++i )
				{
					m_drxParticleEffect->BeginPass(i);
					m_drxParticleEffect->SetMatrix("gWVP", &(_cam->GetViewMatrix()* _cam->GetProjectionMatrix()));
					m_drxParticleEffect->SetTexture("Particle", TextureManager::GetInstance()->GetTexture(_toDraw->uTexHandle));
					m_drxParticleEffect->SetTexture("AlphaRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uAlphaRampHandle ) );
					m_drxParticleEffect->SetTexture("ColorRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uColorRampHandle ) );
					m_drxParticleEffect->SetTexture("ScaleRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uScaleRampHandle ) );
					m_drxParticleEffect->SetTexture("RotationRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uRotationRampHandle ) );
					m_drxParticleEffect->SetFloat( "fScaleMin", _toDraw->m_fRampScaleMin * 1.25f );
					m_drxParticleEffect->SetFloat( "fScaleMax", _toDraw->m_fRampScaleMax * 1.25f );
					m_drxParticleEffect->SetFloat( "fRotationMin", _toDraw->m_fRampRotationMin );
					m_drxParticleEffect->SetFloat( "fRotationMax", _toDraw->m_fRampRotationMax );
					m_drxParticleEffect->CommitChanges();
					HRESULT hr = m_drxDevice->DrawPrimitive( D3DPT_POINTLIST, 0, _toDraw->m_pListParticles.size() );
					m_drxParticleEffect->EndPass();
				}
			}
			m_drxParticleEffect->End();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update: Updates the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::Update(float elapsedTime)
{
	vec3f m_fPlayer = vec3f( 0.0f, 0.0f, 0.0f );
	if( CObjectManager::GetInstance()->GetPlayer() )
		m_fPlayer = CObjectManager::GetInstance()->GetPlayer()->GetWorldPos();

	m_unActiveParticles = 0;
	CPM_Particle* currParticle = m_pActiveHead;

	while( currParticle )
	{
		// Update the energy and chuck it if the particle is dead.
		currParticle->m_fEnergy -= elapsedTime/currParticle->m_fDuration;
		if( currParticle->m_fEnergy <= 0.0f )
		{
			currParticle->m_bAlive = false;
			CPM_Particle* pToKill = currParticle;
			NEXT( currParticle );
			KillParticle( pToKill );
			//pToKill->m_pEmitterParent->DisentangleParticle( pToKill );
			continue;
		}

		// Do update things.
		currParticle->m_fPrevPos = currParticle->m_fPos;
		currParticle->m_fPos += currParticle->m_fVel * elapsedTime;
		currParticle->m_fVel += currParticle->m_pEmitterParent->m_fGravity;

		if( currParticle->m_pEmitterParent->m_bGravityWell )
		{
			currParticle->m_fVel += ( currParticle->m_pEmitterParent->m_fTargetPosition - currParticle->m_fPos ).normalize() * 10.0f;
		}

		if( currParticle->m_pEmitterParent->m_bInterpolate )
		{
			vec3f m_fStart = currParticle->m_pEmitterParent->m_fPosition + currParticle->m_fVel;

			currParticle->m_fPos = ( m_fStart + ( currParticle->m_pEmitterParent->m_fTargetPosition - m_fStart ) * ( 1.0f - currParticle->m_fEnergy ) );
		}

		//Iterate
		m_unActiveParticles++;
		NEXT( currParticle );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render: Renders the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::Render( CCamera* _cam )
{
	if( !m_drxDevice )
		return;
	if( !_cam )
		return;

	std::list< CPM_ParEmitter* >::iterator itrEmitter = m_plistActiveEmitters.begin();

	while( itrEmitter != m_plistActiveEmitters.end() )
	{
		RenderEmitter( (*itrEmitter), _cam );
		itrEmitter++;
	}

}

void CParticleManager::RenderEmitter( CPM_ParEmitter* _toDraw )
{

	if( !_toDraw->m_bScreenSpace )
		return;

	Particle_Vertex* _buffer;
	m_drxParticleVerts->Lock( 0, _toDraw->m_pListParticles.size() * sizeof( Particle_Vertex ), (void**)&_buffer, 0 );
	memset( (void*)_buffer, 0, _toDraw->m_pListParticles.size() * sizeof( Particle_Vertex ) );

	std::list< CPM_Particle* >::iterator itrParticle = _toDraw->m_pListParticles.begin();

	while( itrParticle != _toDraw->m_pListParticles.end() )
	{
		//GPU			//CPU
		_buffer->m_fPos			= D3DXVECTOR3( (*itrParticle)->m_fPos.x/CGame::GetInstance()->GetScreenWidth(), (*itrParticle)->m_fPos.y/CGame::GetInstance()->GetScreenHeight(), 0.0f );
		_buffer->m_fSize		= (*itrParticle)->m_fScale * 1.25f;//20.0f + ( 1.0f - (*itrParticle)->m_fEnergy ) * ( 122.0f - 20.0f );
		_buffer->col			= D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f );
		_buffer->m_fPercentage	= (*itrParticle)->m_fEnergy;

		_buffer++;
		itrParticle++;
	}

	m_drxParticleVerts->Unlock();

	m_drxDevice->SetStreamSource( 0, m_drxParticleVerts, 0, sizeof( Particle_Vertex ) );
	m_drxDevice->SetVertexDeclaration( m_drxParticleDecl );

	if( m_drxParticleEffect )
	{
		m_drxParticleEffect->SetTechnique("BasicHUD");
		UINT passes = 0;
		m_drxParticleEffect->Begin( &passes, 0 );
		{
			for( unsigned int i = 0; i < passes; ++i )
			{
				m_drxParticleEffect->BeginPass(i);
				m_drxParticleEffect->SetTexture("Particle", TextureManager::GetInstance()->GetTexture(_toDraw->uTexHandle));
				m_drxParticleEffect->SetTexture("AlphaRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uAlphaRampHandle ) );
				m_drxParticleEffect->SetTexture("ColorRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uColorRampHandle ) );
				m_drxParticleEffect->SetTexture("ScaleRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uScaleRampHandle ) );
				m_drxParticleEffect->SetTexture("RotationRamp", TextureManager::GetInstance()->GetTexture( _toDraw->m_uRotationRampHandle ) );
				m_drxParticleEffect->SetFloat( "fScaleMin", _toDraw->m_fRampScaleMin * 1.25f );
				m_drxParticleEffect->SetFloat( "fScaleMax", _toDraw->m_fRampScaleMax * 1.25f );
				m_drxParticleEffect->SetFloat( "fRotationMin", _toDraw->m_fRampRotationMin );
				m_drxParticleEffect->SetFloat( "fRotationMax", _toDraw->m_fRampRotationMax );
				m_drxParticleEffect->CommitChanges();
				HRESULT hr = m_drxDevice->DrawPrimitive( D3DPT_POINTLIST, 0, _toDraw->m_pListParticles.size() );
				m_drxParticleEffect->EndPass();
			}
		}

		m_drxParticleEffect->End();
	}
}

void CParticleManager::Shutdown()
{
	SAFE_RELEASE(m_drxParticleDecl);
	SAFE_RELEASE(m_drxParticleEffect);
	SAFE_RELEASE(m_drxParticleVerts);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadParticles: Loads the particles from XML files
// Returns: Bool
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CParticleManager::LoadParticleEffect( std::string szName, std::string szFilename)
{
	// All of the values are going to be in Maya units, so for
	// our game we need to mmultiply them by GAME_UNIT.

	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile( szFilename.c_str() );

	TiXmlElement* xmlRoot = xmlDoc.RootElement();

	if( !xmlRoot )
		return false;

	double fValBuffer;
	TiXmlElement* xmlElement;

	// Load up the necessary texture.
	xmlElement = xmlRoot->FirstChildElement("fileTextureName");
	std::string szTextureName( xmlElement->Attribute("value") );
	// Get the filepath;
	std::string szTexturePath( _PARTICLE_FILE_PATH );
	szTexturePath += szTextureName;
	// Load the texture.
	m_mapEffectLibrary[szName].uTexHandle = TextureManager::GetInstance()->LoadTexture( szTexturePath.c_str() );

	//Grab the ramp texture
	xmlElement = xmlRoot->FirstChildElement("alphaRamp");
	std::string szRampName( xmlElement->Attribute("value") );

	std::string szAlphaPath( _PARTICLE_FILE_PATH );
	szAlphaPath += szRampName;
	m_mapEffectLibrary[szName].m_uAlphaRampHandle = TextureManager::GetInstance()->LoadTexture( szAlphaPath.c_str() );

	//Loadup the color ramp
	xmlElement = xmlRoot->FirstChildElement("colorRamp");
	std::string szColorRampName( xmlElement->Attribute("value") );

	std::string szColorPath( _PARTICLE_FILE_PATH );
	szColorPath += szColorRampName;
	m_mapEffectLibrary[szName].m_uColorRampHandle = TextureManager::GetInstance()->LoadTexture( szColorPath.c_str() );

	xmlElement = xmlRoot->FirstChildElement("type");
	xmlElement->Attribute("value", &m_mapEffectLibrary[szName].m_nEmitterType );

	xmlElement = xmlRoot->FirstChildElement("rate");
	xmlElement->Attribute("value", &m_mapEffectLibrary[szName].m_nParticleRate );

	xmlElement = xmlRoot->FirstChildElement("minDistance");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fMinDistance = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("maxDistance");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fMaxDistance = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("spread");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fSpread = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("speed");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fSpeed = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("speedRandom");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fSpeedRandom = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("maxCount");
	xmlElement->Attribute("value", &m_mapEffectLibrary[szName].m_nMaxCount );

	xmlElement = xmlRoot->FirstChildElement("lifespan");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fLifespan = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("lifespanRandom");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fLifespanRandom = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("direction");
	xmlElement->Attribute("X", &fValBuffer );
	m_mapEffectLibrary[szName].m_fDirection.x = (float)fValBuffer;
	xmlElement->Attribute("Y", &fValBuffer );
	m_mapEffectLibrary[szName].m_fDirection.y = (float)fValBuffer;
	xmlElement->Attribute("Z", &fValBuffer );
	m_mapEffectLibrary[szName].m_fDirection.z = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("particleRenderType");
	xmlElement->Attribute("value", &m_mapEffectLibrary[szName].m_nRenderType );

	xmlElement = xmlRoot->FirstChildElement("spriteNum");
	xmlElement->Attribute("value", &m_mapEffectLibrary[szName].m_nSpriteNum );

	xmlElement = xmlRoot->FirstChildElement("spriteScaleX");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fSpriteScale.x = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("spriteScaleY");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fSpriteScale.y = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("spriteTwist");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fSpriteTwist = (float)fValBuffer;

	xmlElement = xmlRoot->FirstChildElement("spriteScaleX");
	xmlElement->Attribute("value", &fValBuffer );
	m_mapEffectLibrary[szName].m_fScale = (float)fValBuffer;

	// ANYTHING BEYOND THIS POINT CAN NOT BE GURANTEED DUE TO BACKWARDS COMPATIBILITY.

	// Load up the scale ramp.
	xmlElement = xmlRoot->FirstChildElement("scaleRamp");
	std::string szScaleRampName;
	if( xmlElement )
		szScaleRampName = std::string( xmlElement->Attribute("value") );
	else
		szScaleRampName = std::string( "default.png" );

	std::string szScalePath( _PARTICLE_FILE_PATH );
	szScalePath += szScaleRampName;
	m_mapEffectLibrary[szName].m_uScaleRampHandle = TextureManager::GetInstance()->LoadTexture( szScalePath.c_str() );

	xmlElement = xmlRoot->FirstChildElement("ScaleX");
	if( xmlElement )
	{
		xmlElement->Attribute("min", &fValBuffer );
		m_mapEffectLibrary[szName].m_fRampScaleMin = (float)fValBuffer;
		xmlElement->Attribute("max", &fValBuffer );
		m_mapEffectLibrary[szName].m_fRampScaleMax = (float)fValBuffer;
	}
	else
	{
		m_mapEffectLibrary[szName].m_fRampScaleMin = m_mapEffectLibrary[szName].m_fScale;
		m_mapEffectLibrary[szName].m_fRampScaleMax = m_mapEffectLibrary[szName].m_fScale;
	}

	xmlElement = xmlRoot->FirstChildElement("rotationRamp");
	std::string szRotationRampName;
	if( xmlElement )
		szRotationRampName = std::string( xmlElement->Attribute("value") );
	else
		szRotationRampName = std::string( "default.png" );

	std::string szRotationPath( _PARTICLE_FILE_PATH );
	szRotationPath += szRotationRampName;
	m_mapEffectLibrary[szName].m_uRotationRampHandle = TextureManager::GetInstance()->LoadTexture( szRotationPath.c_str() );


	xmlElement = xmlRoot->FirstChildElement("Rotation");
	if( xmlElement )
	{
		xmlElement->Attribute("min", &fValBuffer );
		m_mapEffectLibrary[szName].m_fRampRotationMin = (float)fValBuffer;
		xmlElement->Attribute("max", &fValBuffer );
		m_mapEffectLibrary[szName].m_fRampRotationMax = (float)fValBuffer;
	}
	else
	{
		m_mapEffectLibrary[szName].m_fRampRotationMin = 0.0f;
		m_mapEffectLibrary[szName].m_fRampRotationMax = 0.0f;
	}


	xmlElement = xmlRoot->FirstChildElement("gravity");
	if( xmlElement )
	{
		xmlElement->Attribute("X", &fValBuffer );
		m_mapEffectLibrary[szName].m_fGravity.x = (float)fValBuffer;
		xmlElement->Attribute("Y", &fValBuffer );
		m_mapEffectLibrary[szName].m_fGravity.y = (float)fValBuffer;
		xmlElement->Attribute("Z", &fValBuffer );
		m_mapEffectLibrary[szName].m_fGravity.z = (float)fValBuffer;
	}
	else
	{
		m_mapEffectLibrary[szName].m_fGravity.x = 0.0f;
		m_mapEffectLibrary[szName].m_fGravity.y = 0.0f;
		m_mapEffectLibrary[szName].m_fGravity.z = 0.0f;
	}


	xmlElement = xmlRoot->FirstChildElement("localSpace");
	if( xmlElement )
	{
		int buff;
		xmlElement->Attribute("value", &buff );
		if( buff > 0 )
		{
			m_mapEffectLibrary[szName].m_bLocalSpace = true;
		}
		else
		{
			m_mapEffectLibrary[szName].m_bLocalSpace = false;
		}
	}
	else
		m_mapEffectLibrary[szName].m_bLocalSpace = false;

	xmlElement = xmlRoot->FirstChildElement("halfExtent");
	if( xmlElement )
	{
		xmlElement->Attribute("X", &fValBuffer );
		m_mapEffectLibrary[szName].m_fHalfExtents.x = (float)fValBuffer;
		xmlElement->Attribute("Y", &fValBuffer );
		m_mapEffectLibrary[szName].m_fHalfExtents.y = (float)fValBuffer;
		xmlElement->Attribute("Z", &fValBuffer );
		m_mapEffectLibrary[szName].m_fHalfExtents.z = (float)fValBuffer;
	}
	else
	{
		m_mapEffectLibrary[szName].m_fHalfExtents = vec3f( 0.0f, 0.0f, 0.0f );
	}

	xmlElement = xmlRoot->FirstChildElement("screenSpace");
	if( xmlElement )
	{
		xmlElement->Attribute("value", &fValBuffer );
		if( fValBuffer != 0.0f )
			m_mapEffectLibrary[szName].m_bScreenSpace = true;
		else
			m_mapEffectLibrary[szName].m_bScreenSpace = false;
	}
	else
	{
		m_mapEffectLibrary[szName].m_bScreenSpace = false;
	}

	return true;
}

void CParticleManager::CreateEffectOneShot( std::string szName, vec3f _pos )
{
	CPM_ParEmitter* pCedric = GetNextEmitter();

	if( !pCedric )
		return;

	*pCedric = CPM_ParEmitter( m_mapEffectLibrary[ szName ] );

	pCedric->m_bBurst = true;
	pCedric->m_fPosition = _pos;
	pCedric->Emit( 0.0f );
}

void CParticleManager::CreateEffectOneShot( std::string szName, vec3f _pos, vec3f _dir )
{
	CPM_ParEmitter* pCedric = GetNextEmitter();

	if( !pCedric )
		return;

	*pCedric = CPM_ParEmitter( m_mapEffectLibrary[ szName ] );

	pCedric->m_bBurst = true;
	pCedric->m_fPosition = _pos;
	pCedric->m_fDirection = _dir;
	pCedric->Emit( 0.0f );
}

CPM_ParEmitter* CParticleManager::CreateEffect( std::string szName, vec3f _pos )
{
	CPM_ParEmitter* pCedric = GetNextEmitter();

	if( pCedric )
	{
		*pCedric = CPM_ParEmitter( m_mapEffectLibrary[ szName ] );
		pCedric->m_fPosition = _pos;
		return pCedric;
	}
	else
		return nullptr;
}

void CParticleManager::DestroyEffect( CPM_ParEmitter* _toDestroy )
{
	m_plistActiveEmitters.remove( _toDestroy );
	*_toDestroy = CPM_ParEmitter();
	m_plistDeadEmitters.push_back( _toDestroy );
}