#ifndef LIGHT_H_
#define LIGHT_H_

#include "../RenderEngine.h"
#include "../../Utilities/Frame/Frame.h"
#include "../RenderNode.h"

class RenderNode;
class Light
{
protected:
	//MESH AND LIGHT INFORMATION
	string			m_szLightName;
	RenderNode*		m_rhMesh;
	Frame			m_mMatrix;

	//COLORS
	D3DXVECTOR4		m_v4fDiffuse;
	D3DXVECTOR4		m_v4fAmbient;
	D3DXVECTOR4		m_v4fSpecular;

	float			m_fSpecularPower;
	float			m_fSpecularIntensity;
	bool			m_bEnabled;

	

public:
	Light(void) {}
	virtual ~Light(void){ /*if(m_rhMesh) delete m_rhMesh;*/ }

	virtual void ApplyLight( ID3DXEffect* _effect) = 0;

	void SetRenderNode( RenderNode* pNode ) { m_rhMesh = pNode; }
	void SetLightName( const char* _name ) { m_szLightName = _name; }
	void SetDiffuseColor( const D3DXVECTOR4* _diffuse ) { m_v4fDiffuse = *_diffuse; }
	void SetAmbientColor( const D3DXVECTOR4* _ambient ) { m_v4fAmbient = *_ambient; }
	void SetSpecularColor( const D3DXVECTOR4* _specular ) { m_v4fSpecular = *_specular; }
	void SetEnabled( bool _enabled ) { m_bEnabled = _enabled; }
	void SetSpecularPower( float _specularPower ) { m_fSpecularPower = _specularPower; }
	void SetSpecularIntensity( float _specularInten ) { m_fSpecularIntensity = _specularInten; }

	Frame& GetFrame(void) { return m_mMatrix; }
	RenderNode* GetRenderNode(void) { return m_rhMesh; }

};



















#endif //LIGHT_H_