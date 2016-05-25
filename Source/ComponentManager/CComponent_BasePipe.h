//////////////////////////////////////////////////////////////////////////////////////////
//	Title:		CComponent_BasePipe
//	Author:		Josh Morgan
//	Date:		6/26/12
//	Purpose:	To unify the spawn pipes, so that anything that spawns items
//				will have a common pipe with the basics it needs to be a spawn pipe.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_BASEPIPE_H_
#define CCOMPONENT_BASEPIPE_H_

#include "CComponent_Collision.h"
#include "CComponent_Sound.h"

enum ePIPE_TYPE {
	EPIPE_DEFAULT,
	EPIPE_SPAWN,
	EPIPE_BOX,
};

class CComponent_BasePipe : public CComponent_Collision
{
protected:
	vector<IBaseObject*> m_vChildren;			//a vector of the pipes current spawn
	int m_nActiveSpawns;						//the number of current spawns
	unsigned int m_unMaxSpawns;					//the number of things the pipe can spawn until it cannot spawn anymore
	vector<CSceneObject*> m_vNearestObjects;	//the list of nearest objects returned by the KD tree
	bool m_bActive;								//used to tell if the pipe is currently active (turned off if the pipe is out of range of the player maybe?)
	float m_fElapsedTime;						//The time between frames. Set in Update().
	unsigned int m_nPipeType;					//Pipe type specified by the enum
	CComponent_Sound* m_pSoundCmp;				//This is the pointer to the sound component.

public:
	CComponent_BasePipe(void);
	~CComponent_BasePipe(void);

	bool Initialize(void);
	void Update(float fDT);
	void Render(void);
	void Shutdown(void);

	void AddChild(IBaseObject* _child);
	void RemoveChild(IBaseObject* _child);
	void CheckDynamicCollisions(CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume);

	  ///////////////
	 // accessors //
	///////////////
	vector<IBaseObject*> GetChildren(void) { return m_vChildren; }
	int GetActiveSpawns(void) { return m_nActiveSpawns; }
	unsigned int GetMaxSpawns(void) { return m_unMaxSpawns; }
	bool GetActive(void) { return m_bActive; }

	  //////////////
	 // mutators //
	//////////////
	void SetMaxSpawns(unsigned int _newSpawnCap) { m_unMaxSpawns = _newSpawnCap; }
	void SetActive(bool _bActive) { m_bActive = _bActive; }
};

#endif //CCOMPONENT_BASEPIPE_H_