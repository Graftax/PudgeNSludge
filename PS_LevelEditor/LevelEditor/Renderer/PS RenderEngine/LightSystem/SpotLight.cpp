#include "SpotLight.h"


void SpotLight::ApplyLight( ID3DXEffect* _effect )
{
	D3DXHANDLE handle;

	D3DXVECTOR4 posExp( m_v3fPosition.x, m_v3fPosition.y, m_v3fPosition.z, 1.0f );
	handle = _effect->GetParameterByName(m_szLightName.c_str(), "Position");
	_effect->SetVector(handle, &posExp);

	D3DXVECTOR4 directionCut( m_v3fDirection.x, m_v3fDirection.y, m_v3fDirection.z, 1.0f );
	handle = _effect->GetParameterByName(m_szLightName.c_str(), "Direction" );
	_effect->SetVector(handle, &directionCut);

	handle = _effect->GetParameterByName(m_szLightName.c_str(), "Range" );
	_effect->SetFloat(handle, m_fRadius);

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Attenuation");
	_effect->SetVector(handle, &D3DXVECTOR4((D3DXVECTOR3)m_v3fAttenuation.v, 1));

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Exponent");
	_effect->SetFloat(handle, m_fExponent);

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Cutoff");
	_effect->SetFloat(handle, m_fCutOff);

	D3DXMATRIX cam = RenderEngine::GetCamera()->GetViewMatrix();
	float3 toCamera = float3(float3( cam._41, cam._42, cam._43 ) - *(float3 *)&posExp).normalize();

	if( dot_product( toCamera, m_v3fDirection) > m_fCutOff )
		_effect->SetTechnique( "RenderSpotLightInside" );
	else
		_effect->SetTechnique( "RenderSpotLightOutside" );

	Light::ApplyLight( _effect );
}

void SpotLight::SetCutOff(float _cutOff )
{
	m_fCutOff = _cutOff;
}

void SpotLight::SetAttenuation( const vec3f* _attenuation, float _radius )
{
	m_v3fAttenuation = *_attenuation;

	//float adjacent = (-m_v3fAttenuation.y + sqrt( m_v3fAttenuation.y * m_v3fAttenuation.y
		//- 4.0f * m_v3fAttenuation.z * ( m_v3fAttenuation.x - 255.0f))) / ( 2.0f * m_v3fAttenuation.z);
	//float adjacent = _radius;

	//float radAngle = acos(Max(m_fCutOff,1.0f));
	//float tanr = tan(radAngle);
	//float opposite = (tanr * adjacent) / 25.0f;

	GetFrame().GetLocalMat().make_identity();
	GetFrame().GetLocalMat().axis_w = m_v3fPosition;
	//GetFrame().GetLocalMat().scale_pre( opposite, opposite, adjacent );
	GetFrame().GetLocalMat().scale_pre( m_fRadius / (_radius * 0.10f), m_fRadius / (_radius * 0.10f), 20.0f );
	GetFrame().GetLocalMat().translate_pre( 0.0f, 0.5f, 0.0f );
	
	GetFrame().Update();
}