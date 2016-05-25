#include "CLevel.h"

CLevel::CLevel ()
{
	m_szLevelName = "";
	m_szNextLevel = "";

	m_v3KdMax = vec3f(0.0f,0.0f,0.0f);
	m_v3KdMin = vec3f(0.0f,0.0f,0.0f);

	m_v3CheckPointPos = vec3f(0.0f,0.0f,0.0f);

	//m_pLightManager = NULL;
	m_pLightManager.InitLightManager();
}

CLevel::~CLevel ()
{
	m_pLightManager.ShutdownLightManager();
}