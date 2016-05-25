//////////////////////////////////////////////////////////////////////////////////////
// Filename: CEmitter.h
// Author: Kevin Clouden
// Date: 5/14/12
// Purpose: The Emitter will hold all particle data
//////////////////////////////////////////////////////////////////////////////////////

#ifndef EMITTER_H_
#define EMITTER_H_

#include "../PS/config.h"
#include <ctime>
#include <vector>
#include "../InputLib/InputManager.h"
#include "../PS/CGame.h"
using std::string;

class CCamera;
class IBaseObject;
class CParticleManager;
class TextureManager;
#define MAX_PARTICLES 5000

struct Particle_Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 uv;
	float size;
	D3DCOLOR col;

};

struct tParticle
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 gravity;
	D3DXVECTOR3 rotation; 
	D3DCOLOR color;
	float age;
	float lifeSpan;
	float scale;
	float spawnTime;
	float spawnDelay;
	bool IsDead;
	bool environmental;
};

class CEmitter
{
private:
	int m_nNumParticles;

	int m_nStartAlpha, m_nStartRed, m_nStartGreen, m_nStartBlue;
	int m_nEndAlpha, m_nEndRed, m_nEndGreen, m_nEndBlue;

	int m_nSourceBlend;
	int m_nDestinationBlend;

	int m_nNumEmitters;
	int m_nCurDead;
	int m_nParticleIndex;

	int m_nTexHandle;

	float m_fMinVelocityX;
	float m_fMinVelocityY;
	float m_fMinVelocityZ;
	float m_fMaxVelocityX;
	float m_fMaxVelocityY;
	float m_fMaxVelocityZ;

	float m_fGravityX;
	float m_fGravityY;
	float m_fGravityZ;

	float m_fPositionX;
	float m_fPositionY;
	float m_fPositionZ;

	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;

	float m_fMinLifeSpan;
	float m_fMaxLifeSpan;

	float m_fStartScale;
	float m_fEndScale;

	bool m_bContinous;
	bool m_bEmitterActive;

	bool m_bUseCube;
	D3DXVECTOR3 m_RectCenterPoint;
	float m_fRectWidth;
	float m_fRectHeight;
	float m_fRectLength;

	vec3f m_ve3fDirection;
	D3DXVECTOR3 position;//Position for mesh effect
	float speed;//Position for mesh effect

	std::vector<tParticle> m_vParticles;
	//std::vector<CEmitter*> m_vEmitter;
	Particle_Vertex* pVert;
	CInputManager* m_pInput;
	CGame* m_pCgame;
	IBaseObject* m_pPlayer;
	CCamera* m_pCamera;
	CParticleManager* m_pParticleManager;
	int forActivePart;

	TextureManager* m_pTextureManager;

	bool m_bBurst;
	float m_fSpawnTime;
	float m_fSpawnDelay;
public:	
	bool GetBurst() const { return m_bBurst; }
	void SetBurst(bool val) { m_bBurst = val; }

	float SpawnTime() const { return m_fSpawnTime; }
	void SetSpawnTime(float val) { m_fSpawnTime = val; }
	float GetSpawnDelay() const { return m_fSpawnDelay; }
	void SetSpawnDelay(float val) { m_fSpawnDelay = val; }
	
	int GetCurDead() const { return m_nCurDead; }
	void SetCurDead(int val) { m_nCurDead = val; }

	int GetTexHandle() const { return m_nTexHandle; }
	void SetTexHandle(int val) { m_nTexHandle = val; }

	int GetForActivePart() const { return forActivePart; }
	void SetForActivePart(int val) { forActivePart = val; }
	void MakeCube(D3DXVECTOR3 &pos);

	bool GetUseCube(){return m_bUseCube;}

	D3DXVECTOR3 GetRectCenterPoint(){ return m_RectCenterPoint; }
	float GetRectPointX(){ return m_RectCenterPoint.x; }
	float GetRectPointY(){ return m_RectCenterPoint.y; }
	float GetRectPointZ(){ return m_RectCenterPoint.z; }
	float GetRectWidth() { return m_fRectWidth; }
	float GetRectHeight(){ return m_fRectHeight; }
	float GetRectLength(){ return m_fRectLength; }

	void SetUseCube(bool x){m_bUseCube = x;}

	void SetRectCenterPoint(D3DXVECTOR3 x){ m_RectCenterPoint = x; }
	void SetRectPointX(float x){ m_RectCenterPoint.x = x; }
	void SetRectPointY(float x){ m_RectCenterPoint.y = x; }
	void SetRectPointZ(float x){ m_RectCenterPoint.z = x; }
	void SetRectWidth(float x) { m_fRectWidth = x; }
	void SetRectHeight(float x){ m_fRectHeight = x; }
	void SetRectLength(float x){ m_fRectLength = x; }


	float GetSpeed() const { return speed; }
	void SetSpeed(float val) { speed = val; }
	vec3f GetDirection() const { return m_ve3fDirection; }
	void SetDirection(vec3f val) { m_ve3fDirection = val; }

// 	std::vector<CEmitter*> GetEmitter() const { return m_vEmitter; }
// 	void SetEmitter(std::vector<CEmitter*> val) { m_vEmitter = val; }

	void ResetParticlePosition(int numP, D3DXVECTOR3 pos);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Constructors: Default constructor, destructor
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CEmitter(void);
	~CEmitter(void);
	CEmitter(const CEmitter& var);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InitParticles: Inits the particles 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitParticles();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update: Updates the particles
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float elapsedTime);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update: Updates the particles
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdatePointToPoint(vec3f start, vec3f end, float elapsedTime);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update: Updates the particles
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UpdatePointToDirection(float elapsedTime);

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render: Renders the particles
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	void Render();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Reset: When a particle dies that particle is reset with new random values
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Reset(int index);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Generate: Sets particles to random values
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Generate(tParticle& part);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Clear: Clears out vector of particles
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear();

	void ResetAllParticles();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Accessors: Returns data 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetSourceBlend() { return m_nSourceBlend; }
	int GetDestinationBlend() { return m_nDestinationBlend; }

	int GetStartAplha() { return m_nStartAlpha; }
	int GetStartRed() { return m_nStartRed; }
	int GetStartGreen() { return m_nStartGreen; }
	int GetStartBlue() { return m_nStartBlue; }

	int GetEndAplha() { return  m_nEndAlpha; }
	int GetEndRed() { return m_nEndRed; }
	int GetEndGreen() { return m_nEndGreen; }
	int GetEndBlue() { return m_nEndBlue; }

	int GetParticleIndex() { return m_nParticleIndex; }

	int GetNumParticles(){ return m_nNumParticles; }

	float GetPositionX(){ return m_fPositionX; }
	float GetPositionY(){ return m_fPositionY; }
	float GetPositionZ(){ return m_fPositionZ; }

	float GetRotationX(){ return m_fRotationX; }
	float GetRotationY(){ return m_fRotationY; }
	float GetRotationZ(){ return m_fRotationZ; }

	float GetMinLifeSpan(){ return m_fMinLifeSpan; }
	float GetMaxLifeSpan(){ return m_fMaxLifeSpan; }
	float GetStartScale(){ return m_fStartScale; }
	float GetEndScale(){ return m_fEndScale; }

	float GetMinVelocityX(){ return m_fMinVelocityX; }
	float GetMinVelocityY(){ return m_fMinVelocityY; }
	float GetMinVelocityZ(){ return m_fMinVelocityZ; }
	float GetMaxVelocityX(){ return m_fMaxVelocityX; }
	float GetMaxVelocityY(){ return m_fMaxVelocityY; }
	float GetMaxVelocityZ(){ return m_fMaxVelocityZ; }

	float GetGravityX(){ return m_fGravityX; }
	float GetGravityY(){ return m_fGravityY; }
	float GetGravityZ(){ return m_fGravityZ; }


	bool GetEmitterActive() { return m_bEmitterActive; }

	bool GetContinous() { return m_bContinous; }

	std::vector<tParticle>* GetParticles()	{ return &m_vParticles; }

	D3DXVECTOR3 GetPosition() const { return position; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mutators: Sets data 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetContinous(bool Bool) { m_bContinous = Bool; } 
	

	void SetPositionX(float pos){ m_fPositionX = pos; }
	void SetPositionY(float pos){ m_fPositionY = pos; }
	void SetPositionZ(float pos){ m_fPositionZ = pos; }

	void SetRotationX(float rot){ m_fRotationX = rot; }
	void SetRotationY(float rot){ m_fRotationY = rot; }
	void SetRotationZ(float rot){ m_fRotationZ = rot; }

	void SetMinLifeSpan(float life){ m_fMinLifeSpan = life; }
	void SetMaxLifeSpan(float life){ m_fMaxLifeSpan = life; }
	void SetStartScale(float s){ m_fStartScale = s; }
	void SetEndScale(float s){ m_fEndScale = s; }
	void SetNumParticles(int numP){ m_nNumParticles = numP; }

	void SetMinVelocityX(float vel){ m_fMinVelocityX = vel; }
	void SetMinVelocityY(float vel){ m_fMinVelocityY = vel; }
	void SetMinVelocityZ(float vel){ m_fMinVelocityZ = vel; }
	void SetMaxVelocityX(float vel){ m_fMaxVelocityX = vel; }
	void SetMaxVelocityY(float vel){ m_fMaxVelocityY = vel; }
	void SetMaxVelocityZ(float vel){ m_fMaxVelocityZ = vel; }

	void SetGravityX(float vel){ m_fGravityX = vel; }
	void SetGravityY(float vel){ m_fGravityY = vel; }
	void SetGravityZ(float vel){ m_fGravityZ = vel; }

	void SetSourceBlend(int x) { m_nSourceBlend = x; }
	void SetDestinationBlend(int x) { m_nDestinationBlend = x; }

	void SetStartAplha(int x) { m_nStartAlpha = x; }
	void SetStartRed(int x) { m_nStartRed = x; }
	void SetStartGreen(int x) { m_nStartGreen = x; }
	void SetStartBlue(int x) { m_nStartBlue = x; }

	void SetEndAplha(int x) { m_nEndAlpha = x; }
	void SetEndRed(int x) { m_nEndRed = x; }
	void SetEndGreen(int x) { m_nEndGreen = x; }
	void SetEndBlue(int x) { m_nEndBlue = x; }

	void SetEmitterActive(bool val) { m_bEmitterActive = val; }

	void SetParticleIndex(int val) { m_nParticleIndex = val; }

	void SetParticles(std::vector<tParticle>* val) { m_vParticles = *val; }

	void SetPosition(D3DXVECTOR3 val) { position = val; }

};

#endif //EMITTER_H_