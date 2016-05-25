#include "Light.h"
#include "../RenderNode.h"

void Light::ApplyLight( ID3DXEffect* _effect )
{
	//_effect->SetVector( "DiffuseColor", &m_v4fDiffuse );
	//_effect->SetVector( "AmbientColor", &m_v4fAmbient );
	//_effect->SetVector( "SpecularColor", &m_v4fSpecular );
	//_effect->SetFloat( "SpecularPower", m_fSpecularPower );
	//_effect->SetFloat( "SpecularIntensity", m_fSpecularIntensity );	
	//_effect->SetBool( "Enabled", m_bEnabled );

	D3DXHANDLE handle;

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "DiffuseColor");
	HRESULT hr = _effect->SetVector(handle, &m_v4fDiffuse);

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "AmbientColor");
	hr = _effect->SetVector(handle, &m_v4fAmbient);

	handle = _effect->GetParameterByName(m_szLightName.c_str(), "SpecularColor");
	hr = _effect->SetVector(handle, &m_v4fSpecular);

	handle = _effect->GetParameterByName(m_szLightName.c_str(), "SpecularPower");
    hr = _effect->SetFloat(handle, m_fSpecularPower);

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "SpecularIntensity");
    hr = _effect->SetFloat(handle, m_fSpecularIntensity);

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Enabled");
	hr = _effect->SetBool(handle, m_bEnabled);
}