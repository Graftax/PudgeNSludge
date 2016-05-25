#ifndef _BOSSATTACKAI_H
#define _BOSSATTACKAI_H

#include "IBaseAI.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../ObjectLib/Objects/IBaseObject.h"

class CComponent_Animation;

class BossAttackAI : public IBaseAI
{
private:
	Sphere m_FloatSphere;
	vec3f m_v3MoveDir;
	float m_fAttackWait;
	float m_fAttackTime;
	std::vector <IBaseObject*> m_vSpawnedGoos;
	int m_nMaxGoos;

	// Pointer To Sludge and His Ship. NOTE: AI Updates his Ship not Pudge.
	IBaseObject* pSludge;
	IBaseObject* pSludgesShip;

	CComponent_Animation* pAnimComp;

public:
	BossAttackAI();
	~BossAttackAI();

	void Init (CComponent_Collision* _pParent);
	void Shutdown ();
	void Update (float _fElapsedTime);

	void SetMoveDir (vec3f _vDir) { m_v3MoveDir = _vDir; }
	vec3f GetMoveDir () { return  m_v3MoveDir; }
	
	int GetNumSpawnedGoos () { return m_vSpawnedGoos.size (); }
	int GetMaxGooSpawn () { return m_nMaxGoos; }

	void RemoveSpawnedGoo ();
};
#endif