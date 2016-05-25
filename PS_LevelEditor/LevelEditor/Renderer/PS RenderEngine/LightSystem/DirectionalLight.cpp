#include "DirectionalLight.h"


void DirectionalLight::ApplyLight( ID3DXEffect* _effect )
{
	//const float3 *pDirection = &float3(GetFrame().GetWorldMat().zx, GetFrame().GetWorldMat().zy, GetFrame().GetWorldMat().zz ); 

	D3DXVECTOR4 direction(m_v3fDirection, 0.0f );
	//_effect->SetVector( "Direction", &direction );
		D3DXHANDLE handle;

    handle = _effect->GetParameterByName(m_szLightName.c_str(), "Direction");
	_effect->SetVector(handle, &direction);
	Light::ApplyLight( _effect );
}