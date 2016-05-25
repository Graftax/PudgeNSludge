#include "LightManager.h"
#include <fstream>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

LightManager* LightManager::m_pInstance = nullptr;

LightManager* LightManager::GetInstance(void)
{
	if( m_pInstance )
		return m_pInstance;
	else
		m_pInstance = new LightManager();
	return m_pInstance;
}

void LightManager::InitLightManager(void)
{
	m_vLights.clear();
	D3DXMATRIX iden;
	D3DXMatrixIdentity( &iden );
	MeshManager::GetInstance()->LoadMesh( "sphere", iden );
	m_rhSphere = MeshManager::GetInstance()->GetRenderNode( "sphere" );
}

void LightManager::AddLight( int _Type, Light* _Light )
{
	ManagerLight _toAdd;
	_toAdd.m_nType = _Type;
	_toAdd.m_pLight = _Light;
	m_vLights.push_back( _toAdd );
}

void LightManager::ApplyLights(void)
{
	for( unsigned int i = 0u; i < m_vLights.size(); ++i )
	{
		switch( m_vLights[i].m_nType )
		{
		case LIGHT_POINT:
			{
				RenderEngine::DrawPointLight( *(PointLight*)m_vLights[i].m_pLight );
				break;
			}
		case LIGHT_SPOT:
			{
				RenderEngine::DrawSpotLight( *(SpotLight*)m_vLights[i].m_pLight );
				break;
			}
		case LIGHT_DIRECT:
			{
				RenderEngine::DrawDirectionalLight( *(DirectionalLight*)m_vLights[i].m_pLight );
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void LightManager::ShutdownLightManager(void)
{
	for( unsigned int i = 0u; i < m_vLights.size(); ++i )
	{
		if( m_vLights[i].m_nType == LIGHT_DIRECT )
		{
			RenderNode* deleteMe = m_vLights[i].m_pLight->GetRenderNode();
			if( deleteMe )
				delete deleteMe;
		}		

		if( m_vLights[i].m_pLight )
			delete m_vLights[i].m_pLight;
		m_vLights[i].m_pLight = nullptr;
	}

	if( m_pInstance )
		delete m_pInstance;

	m_pInstance = nullptr;
}

Light* LightManager::GetLight( unsigned int _index )
{
	if( _index >= 0 && _index < m_vLights.size() )
		return m_vLights[_index].m_pLight;
	else
		return nullptr;
}

Light* LightManager::GetLight( Light* _light )
{
	for( unsigned int i = 0u; i < m_vLights.size(); ++i )
	{
		if( m_vLights[i].m_pLight == _light )
			return m_vLights[i].m_pLight;
	}

	return nullptr;
}

bool LightManager::RemoveLight( unsigned int _index )
{
	if( _index >= 0 && _index < m_vLights.size() )
	{
		std::vector< ManagerLight >::iterator iter( m_vLights.begin() + _index );

		m_vLights.erase( iter );
	}
	else
		return false;

	return false;
}

bool LightManager::RemoveLight( Light* _light )
{
	std::vector< ManagerLight >::iterator iter( m_vLights.begin() );

	while( iter != m_vLights.end() )
	{
		if( (*iter).m_pLight == _light )
		{
			m_vLights.erase( iter );
			return true; 
		}
		++iter;
	}

	return false;
}

struct BaseLightLoad
{
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	float SpecularIntensity;
	float SpecularPower;
};

void LightManager::LoadLights(const char* szFileName)
{
	for( unsigned int i = 0u; i < m_vLights.size(); ++i )
	{
		if( m_vLights[i].m_nType == LIGHT_DIRECT )
		{
			RenderNode* deleteMe = m_vLights[i].m_pLight->GetRenderNode();
			if( deleteMe )
				delete deleteMe;
		}		

		if( m_vLights[i].m_pLight )
			delete m_vLights[i].m_pLight;
		m_vLights[i].m_pLight = nullptr;
	}

	m_vLights.clear();
	D3DXMATRIX iden;
	D3DXMatrixIdentity( &iden );
	MeshManager::GetInstance()->LoadMesh( "sphere", iden );
	m_rhSphere = MeshManager::GetInstance()->GetRenderNode( "sphere" );

	fstream reader( szFileName, ios_base::in | ios_base::binary );

	if( reader.is_open() )
	{
		BaseLightLoad baseLight;
		int numLights = -1;
		reader.read( (char*)&numLights, 4 );
		for( int i = 0u; i < numLights; ++i )
		{
			int type = -1;
			reader.read( (char*)&type, sizeof(int));
			reader.read( (char*)&baseLight, sizeof(BaseLightLoad) );
			//reader.read( (char*)&type, sizeof(int));
			////Reader Ambient Color
			//unsigned char Ambient[4] = {0};
			//reader.read( (char*)&Ambient, sizeof(char) * 4 );

			//unsigned char Diffuse[4] = {0};
			//reader.read( (char*)&Diffuse, sizeof(char) * 4 );

			//unsigned char Specular[4] = {0};
			//reader.read( (char*)&Specular, sizeof(char) * 4 );

			//float SpecularIntensity = 0.0f;
			//reader.read( (char*)&SpecularIntensity, sizeof(float) );

			//float SpecularPower = 0.0f;
			//reader.read( (char*)&SpecularPower, sizeof(float) );

			switch(type)
			{
				
			case LIGHT_DIRECT:
				{
					DirectionalLight* _ToAddDL = new DirectionalLight();
					_ToAddDL->SetLightName("DirLight");
					_ToAddDL->SetAmbientColor( &D3DXVECTOR4(baseLight.Ambient[1] , baseLight.Ambient[2] , baseLight.Ambient[3] , baseLight.Ambient[0] ));
					_ToAddDL->SetDiffuseColor( &D3DXVECTOR4(baseLight.Diffuse[1] , baseLight.Diffuse[2] , baseLight.Diffuse[3] , baseLight.Diffuse[0] ));
					_ToAddDL->SetSpecularColor(&D3DXVECTOR4(baseLight.Specular[1], baseLight.Specular[2], baseLight.Specular[3], baseLight.Specular[0] )); 
					_ToAddDL->SetSpecularIntensity( baseLight.SpecularIntensity );
					_ToAddDL->SetSpecularPower( baseLight.SpecularPower );
					float rotation[3] = {0.0f};
					reader.read( (char*)&rotation, sizeof(float) * 3 );
					_ToAddDL->SetDirection( D3DXVECTOR3( rotation[0], rotation[1], rotation[2] ) );
					_ToAddDL->SetEnabled(true);
					RenderNode* pNode = new RenderNode;
					pNode->renderData = RenderEngine::GetScreenMesh();
					_ToAddDL->SetRenderNode( pNode );
					AddLight( LIGHT_DIRECT, _ToAddDL );
					break;
				}
			case LIGHT_POINT:
				{
					PointLight* _ToAddPL = new PointLight();
					_ToAddPL->SetLightName("PointLight0");
					_ToAddPL->SetAmbientColor( &D3DXVECTOR4(baseLight.Ambient[1] , baseLight.Ambient[2] , baseLight.Ambient[3] , baseLight.Ambient[0] ));
					_ToAddPL->SetDiffuseColor( &D3DXVECTOR4(baseLight.Diffuse[1] , baseLight.Diffuse[2] , baseLight.Diffuse[3] , baseLight.Diffuse[0] ));
					_ToAddPL->SetSpecularColor(&D3DXVECTOR4(baseLight.Specular[1], baseLight.Specular[2], baseLight.Specular[3], baseLight.Specular[0] ));
					_ToAddPL->SetSpecularIntensity( baseLight.SpecularIntensity );
					_ToAddPL->SetSpecularPower( baseLight.SpecularPower );
					float attenuation[3] = {0.0f};
					reader.read( (char*)&attenuation, sizeof(float) * 3 );
					
					float position[3] = {0.0f};
					reader.read( (char*)&position, sizeof(float) * 3 );
					
					float radius = 0.0f;
					reader.read( (char*)&radius, sizeof(float) );
					_ToAddPL->SetRadius( radius );
					_ToAddPL->SetAttenuation( &D3DXVECTOR3( attenuation[0], attenuation[1], attenuation[2] ));
					_ToAddPL->GetFrame().GetLocalMat().axis_pos = vec3f( position[0], position[1], position[2] );
					_ToAddPL->GetFrame().Update();
					_ToAddPL->SetEnabled(true);
					_ToAddPL->SetRenderNode( m_rhSphere );

					AddLight( LIGHT_POINT, _ToAddPL );
					break;
				}
			case LIGHT_SPOT:
				{
					SpotLight* _ToAddSL = new SpotLight();
					_ToAddSL->SetLightName("SpotLight0");
					_ToAddSL->SetAmbientColor( &D3DXVECTOR4(baseLight.Ambient[1] , baseLight.Ambient[2] , baseLight.Ambient[3] , baseLight.Ambient[0] ));
					_ToAddSL->SetDiffuseColor( &D3DXVECTOR4(baseLight.Diffuse[1] , baseLight.Diffuse[2] , baseLight.Diffuse[3] , baseLight.Diffuse[0] ));
					_ToAddSL->SetSpecularColor(&D3DXVECTOR4(baseLight.Specular[1], baseLight.Specular[2], baseLight.Specular[3], baseLight.Specular[0] )); 
					_ToAddSL->SetSpecularIntensity( baseLight.SpecularIntensity );
					_ToAddSL->SetSpecularPower( baseLight.SpecularPower );
					float attenuation[3] = {0.0f};
					reader.read( (char*)&attenuation, sizeof(float) * 3 );
					
					float position[3] = {0.0f};
					reader.read( (char*)&position, sizeof(float) * 3 );
					_ToAddSL->SetPosition( vec3f( position[0], position[1], position[2] ) );
					
					float radius = 0.0f;
					reader.read( (char*)&radius, sizeof(float) );
					_ToAddSL->SetRadius( radius );
					float Cutoff = 0.0f;
					reader.read( (char*)&Cutoff, sizeof(float) );
					_ToAddSL->SetCutOff( Cutoff );
					float Direction[3] = {0.0f};
					reader.read( (char*)&Direction, sizeof(float) * 3 );
					_ToAddSL->SetDirection( vec3f( Direction[0], Direction[1], Direction[2] ));
					float exponent = 0.0f;
					reader.read( (char*)&exponent, sizeof(float) );
					_ToAddSL->SetExponent( exponent );
					_ToAddSL->SetEnabled(true);
					_ToAddSL->SetRenderNode( m_rhSphere );
					_ToAddSL->SetAttenuation( &vec3f( attenuation[0], attenuation[1], attenuation[2] ), 15.0f);

					AddLight( LIGHT_SPOT, _ToAddSL );
					break;
				}
			default:
				break;
			}
		}
	}

	reader.close();
}
