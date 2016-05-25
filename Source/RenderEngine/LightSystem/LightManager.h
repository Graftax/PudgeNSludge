#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_

#include "Light.h"
#include "../../Utilities/CollisionLib/CollisionSphere.h"
enum Light_Types
{
	LIGHT_DIRECT = 1,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_FAILURE = -1,
};

class CLevel;

class LightManager
{
	friend class CLevel;
private:
	//static LightManager* m_pInstance;

	RenderNode* m_rhSphere;

	struct ManagerLight
	{
		Light*				m_pLight;
		INT					m_nType;
	};

	LightManager(){}									// Private so that it can  not be called
	LightManager(LightManager const&){}				// copy constructor is private
	//LightManager& operator=(LightManager const&){}  // assignment operator is private

	std::vector< ManagerLight > m_vLights;

	float m_fBrightness;
	bool m_bRaiseBrightness;				// Bool to determine whether we should raise the brightness or lower it.
	float m_fTimeToRenderLights;			// Timer that will change the brightness based on the bool above.

public:
	//static LightManager* GetInstance(void);
	void LoadLights(const char* szFileName);
	void InitLightManager(void);
	void AddLight( int _Type, Light* _Light );
	void ApplyLights(void);
	void ShutdownLightManager(void);

	void Update(float _fDT );

	Light* GetLight( unsigned int _index );
	Light* GetLight( Light* _light );

	bool RemoveLight( unsigned int _index );
	bool RemoveLight( Light* _light );

	void SetBrightness( float _value ) { m_fBrightness = _value; }
	float GetBrightness(void) { return m_fBrightness; }

	void SetRaiseBrightness( bool _val ) { m_bRaiseBrightness = _val; }
	void SetTimeToRender( float _val ) { m_fTimeToRenderLights = _val; }
	float GetTimeToRender () { return m_fTimeToRenderLights; }

};


























#endif