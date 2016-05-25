////////////////////////////////////////////////////////////
//Name:		CComponent_PlayerSound.h
//Author:	Josh Morgan
//Date:		5/31/12
//Purpose:	This class is used to localize the use of sound
//			by the player.
////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_PLAYERSOUND_H_
#define CCOMPONENT_PLAYERSOUND_H_

#include "IComponent.h"
#include "../PS/CGame.h"

//class CGame;
//class CAudioManager;
//struct TSoundObject;
class CComponent_Collision;
class CComponent_Sound : public IComponent
{
private:
	CGame* m_pGame;
	CAudioManager* m_pAudio;
	TSoundObject* m_pSoundObject;
	IBaseObject* m_pClosestGooPit;
	CComponent_Collision* m_pParentCollision;

	//bools used for turning on and off the layers of the music for when certain enemies are near
	bool m_bLilGooNear;
	bool m_bSlimeNear;

	bool m_bPrevLilGooNear;
	bool m_bPrevSlimeNear;

	bool m_bSoundLoop1;
	bool m_bSoundLoop2;

	bool m_bInRange;

public:
	CComponent_Sound(void);
	~CComponent_Sound(void);

	bool Initialize(void);
	bool RegisterSoundObject(void);
	void Update(float);
	void Shutdown(void);

	bool PlaySound(unsigned int nPlayerSound);
	void StopSound(unsigned int nPlayerSound);

	//accessors
	CAudioManager* GetAudio(void) { return m_pAudio; }
	IBaseObject* GetClosestGooPit() { return m_pClosestGooPit; }
	bool GetInRange(void) { return m_bInRange; }

	//mutators
	void SetClosestGooPit(IBaseObject* _pNewClosest) { m_pClosestGooPit = _pNewClosest; }
	void SetInRange(bool _bInRange) { m_bInRange = _bInRange; }
};

#endif //CCOMPONENT_PLAYERSOUND_H_