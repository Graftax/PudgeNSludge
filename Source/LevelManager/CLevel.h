#ifndef _CLEVEL_H
#define _CLEVEL_H

#include <string>
//#include "../PS/config.h"
#include "../Utilities/MathLib/vec3.h"

#include "../RenderEngine/LightSystem/LightManager.h"

class CLevel
{
private:
	std::string m_szLevelName;
	std::string m_szNextLevel;

	vec3f m_v3KdMax;
	vec3f m_v3KdMin;

	vec3f m_v3CheckPointPos;

	// The Players Spawn position.
	vec3f m_v3PlayerSpawnPosition;
	LightManager m_pLightManager;

	char m_cLevelID;				// The Level ID for this level.

public:
	CLevel();
	~CLevel();

	std::string GetName () { return m_szLevelName; }
	void SetName ( char* _szName ) { m_szLevelName = _szName; }

	std::string GetNextLevel () { return m_szNextLevel; }
	void SetNextLevel ( char* _szName ) { m_szNextLevel = _szName; }

	vec3f GetKdTreeMax () { return m_v3KdMax; }
	void SetKdTreeMax ( vec3f _v3Max ) { m_v3KdMax = _v3Max; }

	vec3f GetKdTreeMin () { return m_v3KdMin; }
	void SetKdTreeMin ( vec3f _v3Min ) { m_v3KdMin = _v3Min; }

	vec3f GetCheckPointPos () { return m_v3CheckPointPos; }
	void SetCheckPointPos ( vec3f _v3Pos ) { m_v3CheckPointPos = _v3Pos; }

	vec3f GetPlayerSpawnPos(void) { return m_v3PlayerSpawnPosition; }
	void SetPlayerSpawnPos( vec3f _pos ) { m_v3PlayerSpawnPosition = _pos; }

	char GetLevelID( void ) { return m_cLevelID; }
	void SetLevelID( char _id ) { m_cLevelID = _id; }

	LightManager& GetLightManager( void ) { return m_pLightManager; }
};
#endif