#include "ParticleManager.h"
#include "Effects.h"
#include "Emitter.h"
#include "../Camera/Camera.h"
#include "../RenderEngine/RenderEngine.h"
#include "../RenderEngine/TextureManager.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

CParticleManager* CParticleManager::m_pInstance = NULL;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager::CParticleManager(void)
{
	particleBuffer = NULL;
	particleDecl = NULL;
	device = NULL;
	particleShaderEffect = NULL;
	m_nNumEmitters = 0;
	m_pEffects = NULL;
	m_pEmitter = NULL;
	m_nParticleIndex = -1;
	indexForActiveparticle = -1;
	m_pTextureManager = TextureManager::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor, destructor
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager::~CParticleManager(void)
{
	
// 	for(unsigned int i = 0; i < m_vParticleEffect.size(); i++)
// 	{
// 		/*for (unsigned int EmitterIndex = 0; EmitterIndex < m_vParticleEffect[i]->GetEmitters().size(); EmitterIndex++)
// 		{
// 			if(m_pEmitter)
// 			{
// 				delete m_vParticleEffect[i]->GetEmitters()[EmitterIndex];
// 			}
// 		}*/
// 
// 		m_pEffects = m_vParticleEffect[i];
// 		if(m_pEffects)
// 		{
// 			delete m_pEffects;
// 		}
// 
// 		
// 	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns the instance
// Original Author: KC
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager* CParticleManager::GetInstance ()
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadParticles: Loads the particles from binary files
// Returns: Bool
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CParticleManager::LoadParticles(std::string szFilename)
{
	std::fstream out;
	out.clear();
	out.open(szFilename.c_str(), std::ios_base::in | std::ios_base::binary);
	if(out.is_open())
	{
		m_pEffects = new CEffects();

		out.read((char*) &m_nNumEmitters, sizeof(int));
		m_pEffects->m_nNumEmitters = m_nNumEmitters;

		for (int i = 0; i < m_pEffects->m_nNumEmitters; i++)
		{
			m_pEmitter = new CEmitter();

			float minVelocityX = 0.0f, minVelocityY = 0.0f, minVelocityZ = 0.0f, maxVelocityX = 0.0f, maxVelocityY = 0.0f, maxVelocityZ = 0.0f; 
			float gravityX = 0.0f, gravityY = 0.0f, gravityZ = 0.0f, positionX = 0.0f, positionY = 0.0f, positionZ = 0.0f, rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
			float minLifeSpan = 0.0f, maxLifeSpan = 0.0f, startScale = 0.0f, endScale = 0.0f;
			int sA = 0, sR = 0, sG = 0, sB = 0, eA = 0, eR = 0, eG = 0, eB = 0, sourceBlend = 0, destinationBlend = 0, numParticles = 0;

			out.read((char*)& numParticles, sizeof(int));
			m_pEmitter->SetNumParticles(numParticles);

			out.read((char*)& minVelocityX, sizeof(float));
			out.read((char*)& minVelocityY, sizeof(float));
			out.read((char*)& minVelocityZ, sizeof(float));
			out.read((char*)& maxVelocityX, sizeof(float));
			out.read((char*)& maxVelocityY, sizeof(float));
			out.read((char*)& maxVelocityZ, sizeof(float));
			m_pEmitter->SetMinVelocityX(minVelocityX);
			m_pEmitter->SetMinVelocityY(minVelocityY);
			m_pEmitter->SetMinVelocityZ(minVelocityZ);
			m_pEmitter->SetMaxVelocityX(maxVelocityX);
			m_pEmitter->SetMaxVelocityY(maxVelocityY);
			m_pEmitter->SetMaxVelocityZ(maxVelocityZ);

			out.read((char*)& gravityX,sizeof(float));
			out.read((char*)& gravityY,sizeof(float));
			out.read((char*)& gravityZ,sizeof(float));
			m_pEmitter->SetGravityX(gravityX);
			m_pEmitter->SetGravityY(gravityY);
			m_pEmitter->SetGravityZ(gravityZ);

			out.read((char*)& positionX, sizeof(float));
			out.read((char*)& positionY, sizeof(float));
			out.read((char*)& positionZ, sizeof(float));
			m_pEmitter->SetPositionX(positionX);
			m_pEmitter->SetPositionY(positionY);
			m_pEmitter->SetPositionZ(positionZ);

			out.read((char*)& rotationX, sizeof(float));
			out.read((char*)& rotationY, sizeof(float));
			out.read((char*)& rotationZ, sizeof(float));
			m_pEmitter->SetRotationX(rotationX);
			m_pEmitter->SetRotationY(rotationY);
			m_pEmitter->SetRotationZ(rotationZ);

			out.read((char*)& minLifeSpan, sizeof(float));
			out.read((char*)& maxLifeSpan, sizeof(float));
			m_pEmitter->SetMinLifeSpan(minLifeSpan);
			m_pEmitter->SetMaxLifeSpan(maxLifeSpan);

			out.read((char*)& startScale, sizeof(float));
			out.read((char*)& endScale, sizeof(float));
			m_pEmitter->SetStartScale(startScale);
			m_pEmitter->SetEndScale(endScale);

			out.read((char*)& sA, sizeof(int));
			out.read((char*)& sR, sizeof(int));
			out.read((char*)& sG, sizeof(int));
			out.read((char*)& sB, sizeof(int));
			out.read((char*)& eA, sizeof(int));
			out.read((char*)& eR, sizeof(int));
			out.read((char*)& eG, sizeof(int));
			out.read((char*)& eB, sizeof(int));
			m_pEmitter->SetStartAplha(sA);
			m_pEmitter->SetEndAplha(eA);
			m_pEmitter->SetStartRed(sR);
			m_pEmitter->SetEndRed(eR);
			m_pEmitter->SetStartGreen(sG);
			m_pEmitter->SetEndGreen(eG);
			m_pEmitter->SetStartBlue(sB);
			m_pEmitter->SetEndBlue(eB);

			out.read((char*)& sourceBlend, sizeof(int));
			out.read((char*)& destinationBlend, sizeof(int));
			m_pEmitter->SetSourceBlend(sourceBlend);
			m_pEmitter->SetDestinationBlend(destinationBlend);

			bool continous = false;
			out.read((char*)& continous, sizeof(bool));
			m_pEmitter->SetContinous(continous);

			int length = 0;
			out.read((char*)&length, sizeof(int));
			m_szName = new char[length+1];
			out.read((char*)m_szName, length);
			m_szName[length] = '\0';
			string holder = m_szName;
			//m_pEmitter->SetParticleTexture(holder);
			char buffer[64];
			sprintf_s(buffer, 64, "./Resources/Textures/%s", m_szName);
			m_pEmitter->SetTexHandle(m_pTextureManager->LoadTexture(buffer));

			bool cube = false;
			out.read((char*)& cube, sizeof(bool));
			m_pEmitter->SetUseCube(cube);

			float rectWidth = 0.0f, rectHeight = 0.0f, rectLength = 0.0f, rectX = 0.0f, rectY = 0.0f, rectZ = 0.0f;
			out.read((char*)& rectWidth, sizeof(float));
			out.read((char*)& rectHeight, sizeof(float));
			out.read((char*)& rectLength, sizeof(float));
			out.read((char*)& rectX, sizeof(float));
			out.read((char*)& rectY, sizeof(float));
			out.read((char*)& rectZ, sizeof(float));

			m_pEmitter->SetRectWidth(rectWidth);
			m_pEmitter->SetRectHeight(rectHeight);
			m_pEmitter->SetRectLength(rectLength);
			m_pEmitter->SetRectPointX(rectX);
			m_pEmitter->SetRectPointY(rectY);;
			m_pEmitter->SetRectPointZ(rectZ);

			m_pEmitter->SetEmitterActive(false);

			m_nParticleIndex++;
			if(m_nParticleIndex == 1)
			{
				m_pEmitter->SetMinVelocityX(0.0f);
				m_pEmitter->SetMinVelocityY(0.0f);
				m_pEmitter->SetMinVelocityZ(0.0f);
				m_pEmitter->SetMaxVelocityX(0.0f);
				m_pEmitter->SetMaxVelocityY(0.0f);
				m_pEmitter->SetMaxVelocityZ(0.0f);
			}
				
			m_pEffects->m_vEmitters.push_back(*m_pEmitter);

// 			delete m_pEmitter;
// 			m_pEmitter = NULL;

			m_pEffects->m_vEmitters[i].InitParticles();
		}

		m_vParticleEffect.push_back(m_pEffects);

		out.close();
	}
	out.clear();
	//delete[] m_szName;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render: Renders the particles
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::Render(/*LPDIRECT3DDEVICE9 dev, CCamera* cam*/)
{
	for (unsigned int i = 0; i < m_vParticleEffect.size(); i++)
	{
		m_vParticleEffect[i]->Render();
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
	//Sure thing right here use if shit doesnt work. 
	for (unsigned int i = 0; i < m_vParticleEffect.size(); i++)
	{
		m_vParticleEffect[i]->Update(elapsedTime);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateVertexBuffer: Creates a single buffer large enough for all particles in the game
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::CreateVertexBuffer(LPDIRECT3DDEVICE9 dev)
{
	device = dev;
	D3DVERTEXELEMENT9 decl[] = 
	{ 
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE, 0},
		{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	dev->CreateVertexDeclaration(decl, &particleDecl);
	dev->CreateVertexBuffer(MAX_PARTICLES * sizeof(Particle_Vertex), 
		D3DUSAGE_POINTS, 
		0, D3DPOOL_MANAGED, 
		&particleBuffer, NULL );

	ID3DXBuffer *error;
	
	D3DXCreateEffectFromFile(dev, (LPCSTR)"./Resources/Shaders/Particle.fx", 0, 0, D3DXSHADER_DEBUG, 0, &particleShaderEffect, &error);
	if (error)
	{
		MessageBox(0, (LPCSTR)error->GetBufferPointer(),0,0);
	}

}


