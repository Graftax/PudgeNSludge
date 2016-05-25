#include "PointLight.h"



PointLight::PointLight(void)
{
	m_fRadius = 0.0f;
	m_v3fAttenuation = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
}

void PointLight::ApplyLight( ID3DXEffect* _effect )
{
	D3DXHANDLE handle;
    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Position");
	float3 pos = GetFrame().GetWorldMat().axis_pos;
    D3DXVECTOR4 position(pos.x, pos.y, pos.z, 1);
	_effect->SetVector(handle, &position);

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Attenuation");
	_effect->SetVector(handle, &D3DXVECTOR4(m_v3fAttenuation, 1));

	handle = _effect->GetParameterByName(m_szLightName.c_str(), "Range" );
	_effect->SetFloat(handle, m_fRadius );

	D3DXMATRIX cam = RenderEngine::GetCamera()->GetWorldMatrix();
	float3 diff = float3( GetFrame().GetWorldMat().wx, GetFrame().GetWorldMat().wy, GetFrame().GetWorldMat().wz ) - float3( cam._41, cam._42, cam._43 );

	if( dot_product( diff, diff ) + 0.001f < ( m_fRadius * m_fRadius ) )
	{
		_effect->SetTechnique( "RenderPointLightInside" );
	}
	else
	{
		_effect->SetTechnique( "RenderPointLightOutside" );
	}

	Light::ApplyLight( _effect );
}

void PointLight::SetAttenuation( const D3DXVECTOR3* _attenuation )
{
	m_v3fAttenuation = *_attenuation;

	//m_fRadius = (-m_v3fAttenuation.y + sqrt( m_v3fAttenuation.y * m_v3fAttenuation.y - 4.0f
	//	* m_v3fAttenuation.z * ( m_v3fAttenuation.x - 255.0f))) / ( 2 * m_v3fAttenuation.z );
	//m_fRadius = 100.0f;// 413.7f * 0.8f;
	GetFrame().GetLocalMat().scale_pre( GetRadius() * 0.3f, GetRadius() * 0.3f, GetRadius() * 0.1f );
	GetFrame().Update();
	GetFrame().GetWorldMat();
}