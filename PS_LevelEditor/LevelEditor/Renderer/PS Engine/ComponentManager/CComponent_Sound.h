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

class CGame;
class CAudioManager;
struct TSoundObject;
class CComponent_Sound : public IComponent
{
private:
	CGame* m_pGame;
	CAudioManager* m_pAudio;
	TSoundObject* m_pSoundObject;
	IBaseObject* m_pParent;

public:
	CComponent_Sound(void);
	~CComponent_Sound(void);

	bool Initialize(void);
	bool RegisterSoundObject(void);
	void Update(float);
	void Shutdown(void);

	bool PlaySound(int nPlayerSound);
	void StopSound(int nPlayerSound);

	CAudioManager* GetAudio(void) { return m_pAudio; }
};

#endif //CCOMPONENT_PLAYERSOUND_H_