#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#include "Light.h"

enum Light_Types
{
	LIGHT_DIRECT = 1,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_FAILURE = -1,
};



class LightManager
{
private:
	static LightManager* m_pInstance;

	

	struct ManagerLight
	{
		Light*				m_pLight;
		INT					m_nType;
	};

	LightManager(){}									// Private so that it can  not be called
	LightManager(LightManager const&){}				// copy constructor is private
	LightManager& operator=(LightManager const&){}  // assignment operator is private

	std::vector< ManagerLight > m_vLights;

public:
	RenderNode* m_rhSphere;
	static LightManager* GetInstance(void);
	void LoadLights(const char* szFileName);
	void InitLightManager(void);
	void AddLight( int _Type, Light* _Light );
	void ApplyLights(void);
	void ShutdownLightManager(void);

	Light* GetLight( unsigned int _index );
	Light* GetLight( Light* _light );

	bool RemoveLight( unsigned int _index );
	bool RemoveLight( Light* _light );

	std::vector< ManagerLight >& GetLights () { return m_vLights; }
};


























#endif