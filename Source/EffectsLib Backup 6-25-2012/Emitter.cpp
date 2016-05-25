//////////////////////////////////////////////////////////////////////////////////////
// Filename: CEmitter.cpp
// Author: Kevin Clouden
// Date: 5/14/12
// Purpose: The Emitter will hold all particle data
//////////////////////////////////////////////////////////////////////////////////////

#include "Emitter.h"
#include "../Camera/Camera.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../RenderEngine/TextureManager.h"
#include "../ComponentManager/Component_Emitter.h"
#include "../ObjectLib/Object Factory/CObjectFactory.h"
#include "ParticleManager.h"

#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))
inline DWORD FtoDW(FLOAT f){return *((DWORD*)&f);}

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

CEmitter::CEmitter(void)
{
	m_nNumParticles = 0;
	m_nStartAlpha = 0; 
	m_nStartRed = 0; 
	m_nStartGreen = 0; 
	m_nStartBlue = 0;
	m_nEndAlpha = 0; 
	m_nEndRed = 0; 
	m_nEndGreen = 0;
	m_nEndBlue = 0;
	m_nSourceBlend = 0;
	m_nDestinationBlend = 0;
	m_nNumEmitters = 0;
	m_nCurDead = 0;

	m_fMinVelocityX = 0.0f;
	m_fMinVelocityY = 0.0f;
	m_fMinVelocityZ = 0.0f;
	m_fMaxVelocityX = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fMaxVelocityZ = 0.0f;
	m_fGravityX = 0.0f;
	m_fGravityY = 0.0f;
	m_fGravityZ = 0.0f;
	m_fPositionX = 0.0f;
	m_fPositionY = 0.0f;
	m_fPositionZ = 0.0f;
	m_fRotationX = 0.0f;
	m_fRotationY = 0.0f;
	m_fRotationZ = 0.0f;
	m_fMinLifeSpan = 0.0f;
	m_fMaxLifeSpan = 0.0f;
	m_fStartScale = 0.0f;
	m_fEndScale = 0.0f;

	m_pCgame = CGame::GetInstance();
	m_pInput = m_pCgame->GetInputManager();
	m_pTextureManager = TextureManager::GetInstance();
	m_pParticleManager = CParticleManager::GetInstance();

	m_nTexHandle = -1;

	m_bEmitterActive = true;
	m_pCamera = NULL;

	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	speed = 0.0f;
}

CEmitter::CEmitter( const CEmitter& var )
{
// 	if(m_vEmitter.size())
// 	{
// 		this->m_vEmitter.clear();
// 		m_vEmitter.resize (var.m_vEmitter.size());
// 		memcpy(&this->m_vEmitter[0], &var.m_vEmitter[0], sizeof(m_vEmitter[0]) * m_vEmitter.size());
// 	}

	this->m_vParticles.clear();
	for(unsigned int i = 0; i < var.m_vParticles.size(); i++)
	{
		this->m_vParticles.push_back(var.m_vParticles[i]);
	}

	m_nNumParticles = var.m_nNumParticles;
	m_nStartAlpha = var.m_nStartAlpha; 
	m_nStartRed = var.m_nStartRed; 
	m_nStartGreen = var.m_nStartGreen; 
	m_nStartBlue = var.m_nStartBlue;
	m_nEndAlpha = var.m_nEndAlpha; 
	m_nEndRed = var.m_nEndRed; 
	m_nEndGreen = var.m_nEndGreen;
	m_nEndBlue = var.m_nEndBlue;
	m_nSourceBlend = var.m_nSourceBlend;
	m_nDestinationBlend = var.m_nDestinationBlend;
	m_nNumEmitters = var.m_nNumEmitters;
	m_nCurDead = var.m_nCurDead;
	m_fMinVelocityX = var.m_fMinVelocityX;
	m_fMinVelocityY = var.m_fMinVelocityY;
	m_fMinVelocityZ = var.m_fMinVelocityZ;
	m_fMaxVelocityX = var.m_fMaxVelocityX;
	m_fMaxVelocityY = var.m_fMaxVelocityY;
	m_fMaxVelocityZ = var.m_fMaxVelocityZ;
	m_fGravityX = var.m_fGravityX;
	m_fGravityY = var.m_fGravityY;
	m_fGravityZ = var.m_fGravityZ;
	m_fPositionX = var.m_fPositionX;
	m_fPositionY = var.m_fPositionY;
	m_fPositionZ = var.m_fPositionZ;
	m_fRotationX = var.m_fRotationX;
	m_fRotationY = var.m_fRotationY;
	m_fRotationZ = var.m_fRotationZ;
	m_fMinLifeSpan = var.m_fMinLifeSpan;
	m_fMaxLifeSpan = var.m_fMaxLifeSpan;
	m_fStartScale = var.m_fStartScale;
	m_fEndScale = var.m_fEndScale;

	m_bContinous = var.m_bContinous;
	m_bEmitterActive = var.m_bEmitterActive;

	m_bUseCube = var.m_bUseCube;
	m_RectCenterPoint = var.m_RectCenterPoint;
	m_fRectWidth = var.m_fRectWidth;
	m_fRectHeight = var.m_fRectHeight;
	m_fRectLength = var.m_fRectLength;

	m_ve3fDirection = var.m_ve3fDirection;
	position = var.position;
	speed = var.speed;
	pVert = var.pVert;
	m_pInput = var.m_pInput;
	m_pCgame = var.m_pCgame;
	m_pPlayer = var.m_pPlayer;
	m_pCamera = var.m_pCamera;
	m_pParticleManager = var.m_pParticleManager;
	forActivePart = var.forActivePart;
	m_nTexHandle = var.m_nTexHandle;
	m_pTextureManager = var.m_pTextureManager;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEmitter::~CEmitter(void)
{
// 	if(m_pCamera)
// 		delete m_pCamera;
	//delete[] m_vParticles;
// 	if(m_vParticles.size() != 0)
// 		m_vParticles.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitParticles: Inits the particles 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::InitParticles()
{
	for (int i = 0; i < m_nNumParticles; i++)
	{
		tParticle temp;

		D3DXVECTOR3 pos, grav, rot, vel;
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		temp.position = pos;

		// 		if(GetUseCube() == true)
		// 			MakeCube(temp.position);

		vel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		temp.velocity = vel;
		grav = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		temp.gravity = grav;
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		temp.rotation = rot;
		temp.age = 0.0f;
		temp.lifeSpan = RAND_FLOAT(0.0f, 0.0f);
		temp.scale = 0.0;
		temp.IsDead = true;
		temp.spawnDelay = RAND_FLOAT(0.0f, 0.5f);
		temp.spawnTime = 0.0f;
		temp.color = D3DCOLOR_ARGB(m_nStartAlpha, m_nStartRed, m_nStartGreen, m_nStartBlue);
		temp.environmental = false;

		m_vParticles.push_back(temp);
	}
	//m_vEmitter.push_back(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset: When a particle dies that particle is reset with new random values
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::Reset( int index )
{
	Generate(m_vParticles[index]);
}

void CEmitter::ResetAllParticles()
{
	for (int i = 0; i < m_nNumParticles; i++)
	{
		m_vParticles[i].IsDead = true;
		m_bEmitterActive = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate: Sets particles to random values
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::Generate( tParticle& pPart )
{
	D3DXVECTOR3 pos, grav, rot, vel;
	pos = GetPosition();
	
	pPart.position = pos;

	// 	if(GetUseCube() == true)
	// 		MakeCube(pPart.position);

	vel = D3DXVECTOR3(RAND_FLOAT(m_fMinVelocityX, m_fMaxVelocityX), RAND_FLOAT(m_fMinVelocityY, m_fMaxVelocityY), 
		RAND_FLOAT(m_fMinVelocityZ, m_fMaxVelocityZ));

	pPart.velocity = vel;
	grav = D3DXVECTOR3(m_fGravityX, m_fGravityY, m_fGravityZ);
	pPart.gravity = grav;
	rot = D3DXVECTOR3(m_fRotationX, m_fRotationY, m_fRotationZ);
	pPart.rotation = rot;
	pPart.age = 0.0f;
	pPart.lifeSpan = RAND_FLOAT(m_fMinLifeSpan, m_fMaxLifeSpan);
	pPart.scale = m_fStartScale;
	pPart.IsDead = false;
	pPart.spawnDelay = RAND_FLOAT(0.0f, 0.5f);
	pPart.spawnTime = 0.0f;
	pPart.color = pPart.color = D3DCOLOR_ARGB(m_nStartAlpha, m_nStartRed, m_nStartGreen, m_nStartBlue);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Clear: Clears out vector of particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::Clear()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update: Updates the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEmitter::Update(float elapsedTime)
{
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

	m_nCurDead = 0;

	if(m_vParticles.size() != 0)
	{
		for (int i = 0; i < m_nNumParticles; i++)
		{
 			if(m_vParticles[i].IsDead == true)
 			{
				if(m_bEmitterActive)
				{
					Generate(m_vParticles[i]);
				}
 			//	m_nCurDead--;
 			}

			m_vParticles[i].spawnTime += elapsedTime;

			if(m_vParticles[i].spawnTime > m_vParticles[i].spawnDelay)
			{
				m_vParticles[i].velocity += (m_vParticles[i].gravity * elapsedTime );
				m_vParticles[i].position += (m_vParticles[i].velocity * elapsedTime);

				float ratio = m_vParticles[i].age / m_vParticles[i].lifeSpan;
				float scale = m_fStartScale - ( ( m_fStartScale - m_fEndScale ) * ratio );

				int colorA = (int)( m_nStartAlpha - ( ( m_nStartAlpha - m_nEndAlpha ) * ratio ) );
				int colorR = (int)( m_nStartRed   - ( ( m_nStartRed - m_nEndRed ) * ratio ) );
				int colorG = (int)( m_nStartGreen - ( ( m_nStartGreen - m_nEndGreen ) * ratio ) );
				int colorB = (int)( m_nStartBlue  - ( ( m_nStartBlue - m_nEndBlue ) * ratio ) );

				if(colorA > 255)
					colorA = 255;
				if(colorA < 0)
					colorA = 0;

				if(colorR > 255)
					colorR = 255;
				if(colorR < 0)
					colorR = 0;

				if(colorG > 255)
					colorG = 255;
				if(colorG < 0)
					colorG = 0;

				if(colorB > 255)
					colorB = 255;
				if(colorB < 0)
					colorB = 0;


				m_vParticles[i].scale = scale;
				m_vParticles[i].color = D3DCOLOR_ARGB(colorA, colorR, colorG, colorB);

				m_vParticles[i].age += 0.5f * elapsedTime;

				if (m_vParticles[i].age > m_vParticles[i].lifeSpan)
				{
					// 				if(m_bContinous == true)
					// 				{
					m_vParticles[i].IsDead = true;
					//Reset(i);
					//m_nCurDead++;
					// 				}

				}
			}
		}
	}
}

void CEmitter::UpdatePointToPoint( vec3f start, vec3f end, float elapsedTime )
{
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

	m_nCurDead = 0;

	vec3f temp;
	temp.x = end.x - start.x;
	temp.y = end.y - start.y;
	temp.z = end.z - start.z;
	//float length = temp.magnitude();

 	if(m_vParticles.size() != 0)
	{
		for (int i = 0; i < m_nNumParticles; i++)
		{
			if(m_vParticles[i].IsDead == true)
			{
				if(m_bEmitterActive)
				{
					Generate(m_vParticles[i]);
					m_vParticles[i].velocity = *(D3DXVECTOR3*)(&(temp));
				}
			}

			m_vParticles[i].spawnTime += elapsedTime;

			if(m_vParticles[i].spawnTime > m_vParticles[i].spawnDelay)
			{
				//D3DXVECTOR3 newVec = *(D3DXVECTOR3*)(&(temp));

				m_vParticles[i].position += m_vParticles[i].velocity * elapsedTime;

				m_vParticles[i].position += m_vParticles[i].gravity * elapsedTime;

				float ratio = m_vParticles[i].age / m_vParticles[i].lifeSpan;
				float scale = m_fStartScale - ( ( m_fStartScale - m_fEndScale ) * ratio );

				int colorA = (int)( m_nStartAlpha - ( ( m_nStartAlpha - m_nEndAlpha ) * ratio ) );
				int colorR = (int)( m_nStartRed   - ( ( m_nStartRed - m_nEndRed ) * ratio ) );
				int colorG = (int)( m_nStartGreen - ( ( m_nStartGreen - m_nEndGreen ) * ratio ) );
				int colorB = (int)( m_nStartBlue  - ( ( m_nStartBlue - m_nEndBlue ) * ratio ) );

				if(colorA > 255)
					colorA = 255;
				if(colorA < 0)
					colorA = 0;

				if(colorR > 255)
					colorR = 255;
				if(colorR < 0)
					colorR = 0;

				if(colorG > 255)
					colorG = 255;
				if(colorG < 0)
					colorG = 0;

				if(colorB > 255)
					colorB = 255;
				if(colorB < 0)
					colorB = 0;


				m_vParticles[i].scale = scale;
				m_vParticles[i].color = D3DCOLOR_ARGB(colorA, colorR, colorG, colorB);

				m_vParticles[i].age += 0.5f * elapsedTime;

 				if (m_vParticles[i].age > m_vParticles[i].lifeSpan)
 				{
					// 				if(m_bContinous == true)
					// 				{
					m_vParticles[i].IsDead = true;
					//Reset(i);
					//m_nCurDead++;
					// 				}

 				}
			}
		}
	}
}

void CEmitter::UpdatePointToDirection( float elapsedTime )
{
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

	m_nCurDead = 0;

// 	if(m_vParticles.size() != 0)
// 	{
		for (int i = 0; i < m_nNumParticles; i++)
		{
 			if(m_vParticles[i].IsDead == true)
 			{
				if(m_bEmitterActive)
				{
					Generate(m_vParticles[i]);
				}
				//m_nCurDead--;
			}

			m_vParticles[i].spawnTime += elapsedTime;

			if(m_vParticles[i].spawnTime > m_vParticles[i].spawnDelay)
			{
				vec3f dir = GetDirection();
				D3DXVECTOR3 temp = *((D3DXVECTOR3*)(&dir));
				m_vParticles[i].velocity = -(temp * speed) * elapsedTime;
				m_vParticles[i].position += m_vParticles[i].velocity ;

				m_vParticles[i].position += m_vParticles[i].gravity * elapsedTime;

				float ratio = m_vParticles[i].age / m_vParticles[i].lifeSpan;
				float scale = m_fStartScale - ( ( m_fStartScale - m_fEndScale ) * ratio );

				int colorA = (int)( m_nStartAlpha - ( ( m_nStartAlpha - m_nEndAlpha ) * ratio ) );
				int colorR = (int)( m_nStartRed   - ( ( m_nStartRed - m_nEndRed ) * ratio ) );
				int colorG = (int)( m_nStartGreen - ( ( m_nStartGreen - m_nEndGreen ) * ratio ) );
				int colorB = (int)( m_nStartBlue  - ( ( m_nStartBlue - m_nEndBlue ) * ratio ) );

				if(colorA > 255)
					colorA = 255;
				if(colorA < 0)
					colorA = 0;

				if(colorR > 255)
					colorR = 255;
				if(colorR < 0)
					colorR = 0;

				if(colorG > 255)
					colorG = 255;
				if(colorG < 0)
					colorG = 0;

				if(colorB > 255)
					colorB = 255;
				if(colorB < 0)
					colorB = 0;


				m_vParticles[i].scale = scale;
				m_vParticles[i].color = D3DCOLOR_ARGB(colorA, colorR, colorG, colorB);

				m_vParticles[i].age += elapsedTime;

				if (m_vParticles[i].age > m_vParticles[i].lifeSpan)
				{
					// 				if(m_bContinous == true)
					// 				{
					m_vParticles[i].IsDead = true;
					//Reset(i);
					//m_nCurDead++;
					// 				}
				}
			}
		}
// 	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render: Renders the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEmitter::Render()
{
	m_pCamera = m_pCgame->GetCamera();

	Particle_Vertex *pvbuff;
	m_pParticleManager->GetParticleBuffer()->Lock( 0, (m_nNumParticles - m_nCurDead)*sizeof(Particle_Vertex)/*0*/, (void**)&pvbuff, 0 );

	for( int i = 0; i < m_nNumParticles; i++)
	{
		if(!m_vParticles[i].IsDead)
		{
			//GPU			//CPU
			pvbuff->pos	 = m_vParticles[i].position;
			pvbuff->size = m_vParticles[i].scale;
			pvbuff->col	 = m_vParticles[i].color;

			pvbuff++;
		}
	}

	m_pParticleManager->GetParticleBuffer()->Unlock();  

	m_pParticleManager->GetDevice()->SetStreamSource(0, m_pParticleManager->GetParticleBuffer(), 0, sizeof(Particle_Vertex));

	m_pParticleManager->GetDevice()->SetVertexDeclaration(m_pParticleManager->GetParticleDecl());

	D3DXMATRIX transformMatrix;
	D3DXMatrixIdentity(&transformMatrix);


	if (m_pParticleManager->GetParticleShaderEffect() != NULL)
	{
		m_pParticleManager->GetParticleShaderEffect()->SetTechnique("Basic");

		UINT passes = 0;
		m_pParticleManager->GetParticleShaderEffect()->Begin(&passes, 0);
		for(UINT i=0; i<passes; i++)
		{
			m_pParticleManager->GetParticleShaderEffect()->BeginPass(i);
			m_pParticleManager->GetParticleShaderEffect()->SetMatrix("gWVP", &(transformMatrix * m_pCamera->GetViewMatrix()* m_pCamera->GetProjectionMatrix()));

			m_pParticleManager->GetParticleShaderEffect()->SetTexture("Particle", m_pTextureManager->GetTexture(m_nTexHandle));	

			m_pParticleManager->GetParticleShaderEffect()->CommitChanges();

			int particleCount = m_nNumParticles;// - m_nCurDead;
			m_pParticleManager->GetDevice()->DrawPrimitive( D3DPT_POINTLIST, 0, particleCount);

			m_pParticleManager->GetParticleShaderEffect()->EndPass();
		}
		m_pParticleManager->GetParticleShaderEffect()->End();
	}
}

void CEmitter::ResetParticlePosition( int numP, D3DXVECTOR3 pos )
{
	m_vParticles[numP].position = pos;
}

void CEmitter::MakeCube(D3DXVECTOR3 &pos)
{
	int width = (int)(GetRectWidth() * 1000);
	float newX = (float) (rand()%(width + 1) - width / 2.0f);
	pos.x = GetRectPointX() + newX / 1000;

	int height = (int)(GetRectHeight() * 1000);
	float newY = (float)(rand()%(height + 1) - height / 2.0f);
	pos.y = GetRectPointY() + newY / 1000;

	int length = (int)(GetRectLength() * 1000);
	float newZ = (float)(rand()%(length + 1) - length / 2.0f);
	pos.z = GetRectPointZ() + newZ / 1000;

}