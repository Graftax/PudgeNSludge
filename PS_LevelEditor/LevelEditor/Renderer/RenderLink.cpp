#include "RenderLink.h"
#include "IBaseObject.h"
#include "PS RenderEngine\MeshManager.h"
#include "ObjectManager.h"
#include "PS RenderEngine\DebugShapes.h"
#include "PS RenderEngine\LightSystem\LightManager.h"
#include "PS RenderEngine\LightSystem\DirectionalLight.h"
#include "PS RenderEngine\LightSystem\PointLight.h"
#include "PS RenderEngine\LightSystem\SpotLight.h"


bool WINAPI _Init(HWND handle, int width, int height)
{
	if (FAILED (InitD3D(handle, width, height)))
		return false;

	return true;
}

void WINAPI _Update (float fElapsedTime)
{
	Update (fElapsedTime);
}

void WINAPI _Render ()
{
	Render();
}

void WINAPI _Shutdown ()
{
	//Cleanup();
}

void WINAPI _Up ()
{
	GetCam()->TranslateGlobalY (100.0f);
}

void WINAPI _Down ()
{
	GetCam()->TranslateGlobalY (-100.0f);
}

void WINAPI _Left ()
{
	GetCam()->TranslateGlobalX (-100.0f);
}

void WINAPI _Right ()
{
	GetCam()->TranslateGlobalX (100.0f);
}

void WINAPI _Forward ()
{
	GetCam()->TranslateGlobalZ (100.0f);
}

void WINAPI _Backward ()
{
	GetCam()->TranslateGlobalZ (-100.0f);
}

VEC3F WINAPI _GetCamPos ()
{
	VEC3F cam;
	cam.x = GetCam()->GetViewPosition().x;
	cam.y = GetCam()->GetViewPosition().y;
	cam.z = GetCam()->GetViewPosition().z;

	return cam;
}

int WINAPI _GetNumHazards ()
{
	return GetHazards().size();
	//return GetNumHazards ();
}

int WINAPI _GetNumObjects ()
{
	return GetObjects().size();
	//return GetNumHazards ();
}

VEC3F WINAPI _GetObjectPos (int index)
{
	VEC3F ReturnPos;
	ReturnPos.x = 0;
	ReturnPos.y = 0;
	ReturnPos.z = 0;

	if (index < (int)GetHazards().size())
	{
		D3DXVECTOR4 TransformPos = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		D3DXVECTOR4 FinalPos;
		D3DXVec4Transform (&FinalPos, &TransformPos, &GetObjects ()[index]->GetWorldMatrix());

		ReturnPos.x = FinalPos.x;
		ReturnPos.y = FinalPos.y;
		ReturnPos.z = FinalPos.z;
	}

	return ReturnPos;
}

VEC3F WINAPI _GetHazard (int index)
{
	VEC3F ReturnPos;
	ReturnPos.x = 0;
	ReturnPos.y = 0;
	ReturnPos.z = 0;

	if (index < (int)GetHazards().size())
	{
		D3DXVECTOR3 TempPos = GetHazards ()[index];
		D3DXVec3TransformCoord(&TempPos, &TempPos, &GetCam()->GetViewMatrix());
		D3DXVec3TransformCoord(&TempPos, &TempPos, &GetCam()->GetProjectionMatrix());

		D3DVIEWPORT9 viewport;

		GetCam()->l_pd3dDevice->GetViewport(&viewport);

		ReturnPos.x = viewport.Width *(TempPos.x + 1.0f)/2.0f;
        ReturnPos.y = viewport.Height * (1.0f - ((TempPos.y + 1.0f) / 2.0f)); 
	}

	return ReturnPos;
}

int WINAPI _GetHazardType (int index)
{
	return GetHazardTypes()[index];
}

int WINAPI _GetObjectType (int index)
{
	return GetObjectTypes()[index];
}

void WINAPI _LoadLevel (char* szFileName, int length)
{
	szFileName[length] = '\0';
	LoadLevel (szFileName);
}

void WINAPI _SwapLevel (char* szFileName, int length)
{
	szFileName[length] = '\0';
	SwapLevel (szFileName);
}

void WINAPI _ReloadRenderer ()
{
	ReloadRenderer ();
}

void WINAPI _AddObject (int _nType, VEC3F _vPosition)
{
	D3DXMATRIX _worldMatrix;
	D3DXMatrixIdentity (&_worldMatrix);
	D3DXMatrixTranslation (&_worldMatrix, _vPosition.x, _vPosition.y, _vPosition.z);

	switch( _nType )
	{
	case 0:
		{
			IBaseObject* pObject = new IBaseObject();
			pObject->SetObjectType (OT_CRATE);
			pObject->SetWorldMatrix (_worldMatrix);
			pObject->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("Crate"));
			CObjectManager::GetInstance()->AddObject ( pObject );
		}
		break;
	case 1:
		{
			IBaseObject* pObject = new IBaseObject();
			pObject->SetObjectType (OT_CORK);
			pObject->SetWorldMatrix (_worldMatrix);
			pObject->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("CorkShape"));
			CObjectManager::GetInstance()->AddObject ( pObject );
		}
		break;
	case 2:
		{
			IBaseObject* pObject = new IBaseObject();
			pObject->SetObjectType (OT_PLUG);
			pObject->SetWorldMatrix (_worldMatrix);
			pObject->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("Plug"));
			CObjectManager::GetInstance()->AddObject ( pObject );
		}
		break;
	case 3:
		{
			IBaseObject* pObject = new IBaseObject();
			pObject->SetObjectType (OT_PUDGE);
			pObject->SetWorldMatrix (_worldMatrix);
			pObject->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("Pudge"));
			CObjectManager::GetInstance()->AddObject ( pObject );
		}
		break;
	case 4:
		{
			IBaseObject* pObject = new IBaseObject();
			pObject->SetObjectType (OT_SLIMEMONSTER);
			pObject->SetWorldMatrix (_worldMatrix);
			pObject->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("SlimeMonster"));
			CObjectManager::GetInstance()->AddObject ( pObject );
		}
		break;
	}
}

void WINAPI _RemoveObject (int _nArrayPos)
{
	CObjectManager::GetInstance()->RemoveObject (GetObjects()[_nArrayPos]);
	GetObjects().erase (GetObjects().begin() + _nArrayPos);
	GetObjectTypes ().erase (GetObjectTypes().begin() + _nArrayPos);
}

void WINAPI _ChangeObjectType (int _nArrayPos, int _nType)
{
	switch( _nType )
	{
	case 0:
		{
			GetObjects()[_nArrayPos]->SetObjectType (OT_CRATE);
			GetObjects()[_nArrayPos]->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("Crate"));
		}
		break;
	case 1:
		{
			GetObjects()[_nArrayPos]->SetObjectType (OT_CORK);
			GetObjects()[_nArrayPos]->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("CorkShape"));
		}
		break;
	case 2:
		{
			GetObjects()[_nArrayPos]->SetObjectType (OT_PLUG);
			GetObjects()[_nArrayPos]->SetRenderNode (MeshManager::GetInstance()->GetRenderNode ("Plug"));
		}
		break;
	case 3:
		{
			GetObjects()[_nArrayPos]->SetObjectType (OT_PUDGE);
			GetObjects()[_nArrayPos]->SetRenderNode (MeshManager::GetInstance()->GetRenderNodeCopy ("Pudge"));
		}
		break;
	case 4:
		{
			GetObjects()[_nArrayPos]->SetObjectType (OT_SLIMEMONSTER);
			GetObjects()[_nArrayPos]->SetRenderNode (MeshManager::GetInstance()->GetRenderNodeCopy ("SlimeMonster"));
		}
		break;
	}
}

void WINAPI _ChangeObjectPos (int _nArrayPos, VEC3F _vPosition)
{
	D3DXMATRIX _worldMatrix;
	D3DXMatrixIdentity (&_worldMatrix);
	D3DXMatrixTranslation (&_worldMatrix, _vPosition.x, _vPosition.y, _vPosition.z);

	GetObjects()[_nArrayPos]->SetWorldMatrix (_worldMatrix);
}

void WINAPI _RenderDebugSphere (VEC3F _Position)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity (&mat);
	mat._41 = _Position.x;
	mat._42 = _Position.y;
	mat._43 = _Position.z;
	DebugShapes::RenderSphere (mat);
}

//void WINAPI _AddDirectionalLight (BASELIGHT base, VEC3F Direction)
//{
//	DirectionalLight* dirLight = new DirectionalLight ();
//
//	dirLight->SetAmbientColor ( &D3DXVECTOR4 (base.AmbientColorX/255.0f, base.AmbientColorY/255.0f, base.AmbientColorZ/255.0f, base.AmbientColorW/255.0f) );
//	dirLight->SetDiffuseColor ( &D3DXVECTOR4 (base.DiffuseColorX/255.0f, base.DiffuseColorY/255.0f, base.DiffuseColorZ/255.0f, base.DiffuseColorW/255.0f) );
//	dirLight->SetSpecularColor ( &D3DXVECTOR4 (base.SpecularColorX/255.0f, base.SpecularColorY/255.0f, base.SpecularColorZ/255.0f, base.SpecularColorW/255.0f) );
//	dirLight->SetSpecularIntensity ( base.SpecularIntensity );
//	dirLight->SetSpecularPower ( base.SpecularPower );
//	//dirLight->
//	LightManager::GetInstance()->AddLight (LIGHT_DIRECT, dirLight);
//}
//
//void WINAPI _AddPointLight (BASELIGHT base, float radius, VEC3F Attenuation, VEC3F Position)
//{
//	PointLight* pointLight = new PointLight ();
//	pointLight->SetAmbientColor ( &D3DXVECTOR4 (base.AmbientColorX/255.0f, base.AmbientColorY/255.0f, base.AmbientColorZ/255.0f, base.AmbientColorW/255.0f) );
//	pointLight->SetDiffuseColor ( &D3DXVECTOR4 (base.DiffuseColorX/255.0f, base.DiffuseColorY/255.0f, base.DiffuseColorZ/255.0f, base.DiffuseColorW/255.0f) );
//	pointLight->SetSpecularColor ( &D3DXVECTOR4 (base.SpecularColorX/255.0f, base.SpecularColorY/255.0f, base.SpecularColorZ/255.0f, base.SpecularColorW/255.0f) );
//	pointLight->SetSpecularIntensity ( base.SpecularIntensity );
//	pointLight->SetSpecularPower ( base.SpecularPower );
//	pointLight->SetAttenuation ( &D3DXVECTOR3 (Attenuation.x, Attenuation.y, Attenuation.z) );
//	pointLight->SetRadius ( radius );
//	pointLight->GetFrame ().GetLocalMat ().axis_pos = vec3f ( Position.x, Position.y, Position.z);
//	pointLight->GetFrame().Update();
//
//	LightManager::GetInstance()->AddLight (LIGHT_POINT, pointLight);
//}
//
//void WINAPI _AddSpotLight (BASELIGHT base, float CutOff, float Exponent, float Radius, VEC3F Direction, VEC3F Position, VEC3F Attenuation)
//{
//	SpotLight* spotLight = new SpotLight ();
//	spotLight->SetAmbientColor ( &D3DXVECTOR4 (base.AmbientColorX/255.0f, base.AmbientColorY/255.0f, base.AmbientColorZ/255.0f, base.AmbientColorW/255.0f) );
//	spotLight->SetDiffuseColor ( &D3DXVECTOR4 (base.DiffuseColorX/255.0f, base.DiffuseColorY/255.0f, base.DiffuseColorZ/255.0f, base.DiffuseColorW/255.0f) );
//	spotLight->SetSpecularColor ( &D3DXVECTOR4 (base.SpecularColorX/255.0f, base.SpecularColorY/255.0f, base.SpecularColorZ/255.0f, base.SpecularColorW/255.0f) );
//	spotLight->SetSpecularIntensity ( base.SpecularIntensity );
//	spotLight->SetSpecularPower ( base.SpecularPower );
//	spotLight->SetCutOff ( CutOff );
//	spotLight->SetExponent ( Exponent );
//	spotLight->SetRadius ( Radius );
//	spotLight->SetAttenuation ( &vec3f (Attenuation.x, Attenuation.y, Attenuation.z), 15.0f );
//	spotLight->SetDirection ( vec3f (Direction.x, Direction.y, Direction.z) );
//	spotLight->SetPosition ( vec3f (Position.x, Position.y, Position.z) );
//	spotLight->GetFrame();
//
//	LightManager::GetInstance()->AddLight (LIGHT_SPOT, spotLight);
//}
//
//void WINAPI _RemoveLight(int _nArrayPos)
//{
//
//	LightManager::GetInstance()->RemoveLight ( _nArrayPos );
//}

void WINAPI _AddDirectionalLight (BASELIGHT base, VEC3F Direction)
{
	DirectionalLight* _ToAddDL = new DirectionalLight();
	_ToAddDL->SetLightName("DirLight");
	_ToAddDL->SetAmbientColor( &D3DXVECTOR4(base.AmbientColorX/255.0f , base.AmbientColorY/255.0f , base.AmbientColorZ/255.0f , base.AmbientColorW/255.0f ));
	_ToAddDL->SetDiffuseColor( &D3DXVECTOR4(base.DiffuseColorX/255.0f , base.DiffuseColorY/255.0f , base.DiffuseColorZ/255.0f , base.DiffuseColorW/255.0f ));
	_ToAddDL->SetSpecularColor(&D3DXVECTOR4(base.SpecularColorX/255.0f, base.SpecularColorY/255.0f, base.SpecularColorZ/255.0f, base.SpecularColorW/255.0f )); 

	_ToAddDL->SetSpecularIntensity( base.SpecularIntensity );
	_ToAddDL->SetSpecularPower( base.SpecularPower );

	_ToAddDL->SetDirection( D3DXVECTOR3( Direction.x, Direction.y, Direction.z ) );
	_ToAddDL->SetEnabled(true);

	RenderNode* pNode = new RenderNode;
	pNode->renderData = RenderEngine::GetScreenMesh();
	_ToAddDL->SetRenderNode( pNode );

	LightManager::GetInstance()->AddLight (LIGHT_DIRECT, _ToAddDL);
}

void WINAPI _AddPointLight (BASELIGHT base, VEC3F Position, VEC3F Atten, float radius)
{
	PointLight* _ToAddPL = new PointLight();
	_ToAddPL->SetLightName("PointLight0");
	_ToAddPL->SetAmbientColor( &D3DXVECTOR4(base.AmbientColorX/255.0f , base.AmbientColorY/255.0f , base.AmbientColorZ/255.0f , base.AmbientColorW/255.0f ));
	_ToAddPL->SetDiffuseColor( &D3DXVECTOR4(base.DiffuseColorX/255.0f , base.DiffuseColorY/255.0f , base.DiffuseColorZ/255.0f , base.DiffuseColorW/255.0f ));
	_ToAddPL->SetSpecularColor(&D3DXVECTOR4(base.SpecularColorX/255.0f, base.SpecularColorY/255.0f, base.SpecularColorZ/255.0f, base.SpecularColorW/255.0f ));
	
	_ToAddPL->SetSpecularIntensity( base.SpecularIntensity );
	_ToAddPL->SetSpecularPower( base.SpecularPower );
	
	_ToAddPL->SetRadius( radius );
	_ToAddPL->GetFrame().GetLocalMat().make_identity();
	_ToAddPL->SetAttenuation( &D3DXVECTOR3( Atten.x, Atten.y, Atten.z ));
	_ToAddPL->GetFrame().GetLocalMat().axis_pos = vec3f( Position.x, Position.y, Position.z );
	
	_ToAddPL->GetFrame().Update();
	_ToAddPL->SetEnabled(true);
	_ToAddPL->SetRenderNode( LightManager::GetInstance()->m_rhSphere );
	
	LightManager::GetInstance()->AddLight (LIGHT_POINT, _ToAddPL);
}

void WINAPI _AddSpotLight (BASELIGHT base, VEC3F Position, VEC3F Direction, VEC3F Atten, float Radius, float Cutoff, float Expo)
{
	SpotLight* _ToAddSL = new SpotLight();
	_ToAddSL->SetLightName("SpotLight0");

	_ToAddSL->SetAmbientColor( &D3DXVECTOR4(base.AmbientColorX/255.0f , base.AmbientColorY/255.0f , base.AmbientColorZ/255.0f , base.AmbientColorW/255.0f ));
	_ToAddSL->SetDiffuseColor( &D3DXVECTOR4(base.DiffuseColorX/255.0f , base.DiffuseColorY/255.0f , base.DiffuseColorZ/255.0f , base.DiffuseColorW/255.0f ));
	_ToAddSL->SetSpecularColor(&D3DXVECTOR4(base.SpecularColorX/255.0f, base.SpecularColorY/255.0f, base.SpecularColorZ/255.0f, base.SpecularColorW/255.0f )); 

	_ToAddSL->SetSpecularIntensity( base.SpecularIntensity );
	_ToAddSL->SetSpecularPower( base.SpecularPower );

	_ToAddSL->GetFrame().GetLocalMat().make_identity();
	_ToAddSL->SetPosition( vec3f( Position.x, Position.y, Position.z ) );

	_ToAddSL->SetRadius( Radius );

	_ToAddSL->SetCutOff( Cutoff );

	_ToAddSL->SetDirection( vec3f( Direction.x, Direction.y, Direction.z ));

	_ToAddSL->SetExponent( Expo );
	_ToAddSL->SetEnabled(true);
	_ToAddSL->SetRenderNode( LightManager::GetInstance()->m_rhSphere );

	_ToAddSL->SetAttenuation( &vec3f( Atten.x, Atten.y, Atten.z ), 15.0f);

	LightManager::GetInstance()->AddLight (LIGHT_SPOT, _ToAddSL);
}

void WINAPI _RemoveLight(int _nArrayPos)
{
	LightManager::GetInstance()->RemoveLight (_nArrayPos);
}

void WINAPI _ChangeLightDiffuseColor (int _nArrayPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetDiffuseColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	case LIGHT_POINT:
		{
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetDiffuseColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetDiffuseColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	}
}

void WINAPI _ChangeLightAmbientColor (int _nArrayPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetAmbientColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	case LIGHT_POINT:
		{
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetAmbientColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetAmbientColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	}
}

void WINAPI _ChangeLightSpecularColor (int _nArrayPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	case LIGHT_POINT:
		{
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularColor (&D3DXVECTOR4(_fRed/255.0f , _fGreen/255.0f , _fBlue/255.0f , _fAlpha/255.0f ));
		}
		break;
	}
}

void WINAPI _ChangeLightPosition (int _nArrayPos, VEC3F Position)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_POINT:
		{
			//((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().make_identity();
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().axis_pos = vec3f( Position.x, Position.y, Position.z );
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().Update();
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetPosition (vec3f(Position.x, Position.y, Position.z ));
		}
		break;
	}
}

void WINAPI _ChangeLightDirection (int _nArrayPos, VEC3F Direction)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetDirection (D3DXVECTOR3(Direction.x, Direction.y, Direction.z ));
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetDirection (vec3f(Direction.x, Direction.y, Direction.z ));
		}
		break;
	}
}

void WINAPI _ChangeLightAtten (int _nArrayPos, VEC3F Atten)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_POINT:
		{
			vec3f oldpos = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().axis_pos;
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().make_identity();
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().axis_pos = oldpos;
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetAttenuation (&D3DXVECTOR3(Atten.x, Atten.y, Atten.z ));
		}
		break;
	case LIGHT_SPOT:
		{
			//vec3f oldpos = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetPosition();
			//((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().make_identity();
			//((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetPosition ( oldpos );
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetAttenuation (&vec3f(Atten.x, Atten.y, Atten.z ), 15.0f);
		}
		break;
	}
	((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight);
}

void WINAPI _ChangeLightCutOff (int _nArrayPos, float _fCutoff)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetCutOff (_fCutoff);
		}
		break;
	}
}

void WINAPI _ChangeLightExpo (int _nArrayPos, float _fExpo)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetExponent (_fExpo);
		}
		break;
	}
}

void WINAPI _ChangeLightRadius (int _nArrayPos, float _fRadius)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_POINT:
		{
			VEC3F atten;
			atten.x = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.x;
			atten.y = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.y;
			atten.z = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.z;

			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetRadius (_fRadius);
			_ChangeLightAtten (_nArrayPos, atten);
		}
		break;
	case LIGHT_SPOT:
		{
			VEC3F atten;
			atten.x = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.x;
			atten.y = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.y;
			atten.z = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.z;

			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetRadius (_fRadius);
			_ChangeLightAtten (_nArrayPos, atten);
		}
		break;
	}
	((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight);
}

void WINAPI _ChangeLightSpecularIntensity (int _nArrayPos, float _fIntensity)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularIntensity (_fIntensity);
		}
		break;
	case LIGHT_POINT:
		{
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularIntensity (_fIntensity);
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularIntensity (_fIntensity);
		}
		break;
	}
}

void WINAPI _ChangeLightSpecularPower (int _nArrayPos, float _fPower)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularPower (_fPower);
		}
		break;
	case LIGHT_POINT:
		{
			((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularPower (_fPower);
		}
		break;
	case LIGHT_SPOT:
		{
			((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->SetSpecularPower (_fPower);
		}
		break;
	}
}

void WINAPI _ChangeLightType (int _nArrayPos, int _nType)
{
	if (_nType > LIGHT_SPOT && _nType < LIGHT_DIRECT)
	{
		return;
	}

	LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType = _nType;
	Light* pLight = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight;

	switch (_nType)
	{
	case LIGHT_DIRECT:
		{
			delete pLight;
			 pLight = new DirectionalLight();

			((DirectionalLight*)pLight)->SetLightName("DirLight");

			((DirectionalLight*)pLight)->SetAmbientColor( &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));
			((DirectionalLight*)pLight)->SetDiffuseColor( &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));
			((DirectionalLight*)pLight)->SetSpecularColor(&D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f )); 

			((DirectionalLight*)pLight)->SetSpecularIntensity( 0.0f );
			((DirectionalLight*)pLight)->SetSpecularPower( 0.0f );

			((DirectionalLight*)pLight)->SetDirection( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

			((DirectionalLight*)pLight)->SetEnabled(true);

			RenderNode* pNode = new RenderNode;
			pNode->renderData = RenderEngine::GetScreenMesh();
			((DirectionalLight*)pLight)->SetRenderNode( pNode );
		}
		break;
	case LIGHT_POINT:
		{
			delete pLight;
			pLight = new PointLight();

			((PointLight*)pLight)->SetLightName("PointLight0");
			((PointLight*)pLight)->SetAmbientColor( &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));
			((PointLight*)pLight)->SetDiffuseColor( &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));
			((PointLight*)pLight)->SetSpecularColor(&D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));

			((PointLight*)pLight)->SetSpecularIntensity( 0.0f );
			((PointLight*)pLight)->SetSpecularPower( 0.0f );

			((PointLight*)pLight)->SetRadius( 0.0f );
			((PointLight*)pLight)->SetAttenuation( &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ));
			((PointLight*)pLight)->GetFrame().GetLocalMat().axis_pos = vec3f( 0.0f, 0.0f, 0.0f );

			((PointLight*)pLight)->GetFrame().Update();

			((PointLight*)pLight)->SetEnabled(true);
			((PointLight*)pLight)->SetRenderNode( LightManager::GetInstance()->m_rhSphere );
		}
		break;
	case LIGHT_SPOT:
		{
			delete pLight;
			pLight = new SpotLight();

			((SpotLight*)pLight)->SetLightName("SpotLight0");

			((SpotLight*)pLight)->SetAmbientColor( &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));
			((SpotLight*)pLight)->SetDiffuseColor( &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ));
			((SpotLight*)pLight)->SetSpecularColor(&D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f )); 

			((SpotLight*)pLight)->SetSpecularIntensity( 0.0f );
			((SpotLight*)pLight)->SetSpecularPower( 0.0f );

			((SpotLight*)pLight)->SetPosition( vec3f( 0.0f, 0.0f, 0.0f ) );
			((SpotLight*)pLight)->SetAttenuation( &vec3f( 0.0f, 0.0f, 0.0f ), 15.0f);

			((SpotLight*)pLight)->SetRadius( 0.0f );

			((SpotLight*)pLight)->SetCutOff( 0.0f );

			((SpotLight*)pLight)->SetDirection( vec3f( 0.0f, 0.0f, 0.0f ));

			((SpotLight*)pLight)->SetExponent( 0.0f );

			((SpotLight*)pLight)->SetEnabled(true);
			((SpotLight*)pLight)->SetRenderNode( LightManager::GetInstance()->m_rhSphere );
		}
		break;
	}

	LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight = pLight;
}

int WINAPI _GetNumLights ()
{
	return LightManager::GetInstance()->GetLights().size();
}

int WINAPI _GetLightType (int index)
{
	return LightManager::GetInstance()->GetLights()[index].m_nType;
}


float WINAPI _GetLightRadius (int _nArrayPos)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_POINT:
		{
			return ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetRadius ();
		}
		break;
	case LIGHT_SPOT:
		{
			return ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_fRadius;
		}
		break;
	}
	return 0.0f;
}

VEC3F WINAPI _GetLightPos (int _nArrayPos)
{
	VEC3F pos;

	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_POINT:
		{
			pos.x = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().axis_pos.x;
			pos.y = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().axis_pos.y;
			pos.z = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetFrame().GetLocalMat().axis_pos.z;
		}
		break;
	case LIGHT_SPOT:
		{
			pos.x = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetPosition().x;
			pos.y = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetPosition().y;
			pos.z = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->GetPosition().z;
		}
		break;
	}
	return pos;
}

VEC3F WINAPI _GetLightDir (int _nArrayPos)
{
	VEC3F pos;

	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_DIRECT:
		{
			pos.x = ((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fDirection.x;
			pos.y = ((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fDirection.y;
			pos.z = ((DirectionalLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fDirection.z;
		}
		break;
	case LIGHT_SPOT:
		{
			pos.x = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fDirection.x;
			pos.y = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fDirection.y;
			pos.z = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fDirection.z;
		}
		break;
	}
	return pos;
}

VEC3F WINAPI _GetLightAtten (int _nArrayPos)
{
	VEC3F pos;

	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_POINT:
		{
			pos.x = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.x;
			pos.y = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.y;
			pos.z = ((PointLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.z;
		}
		break;
	case LIGHT_SPOT:
		{
			pos.x = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.x;
			pos.y = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.y;
			pos.z = ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_v3fAttenuation.z;
		}
		break;
	}
	return pos;
}

float WINAPI _GetLightExpo (int _nArrayPos)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_SPOT:
		{
			return ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_fExponent;
		}
		break;
	}
	return 0.0f;
}

float WINAPI _GetLightCutOff (int _nArrayPos)
{
	switch (LightManager::GetInstance()->GetLights()[_nArrayPos].m_nType)
	{
	case LIGHT_SPOT:
		{
			return ((SpotLight*)LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight)->m_fCutOff;
		}
		break;
	}
	return 0.0f;
}

BASELIGHT WINAPI _GetLightBase (int _nArrayPos)
{
	BASELIGHT light;

	light.AmbientColorX = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fAmbient.x * 255.0f;
	light.AmbientColorY = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fAmbient.y * 255.0f;
	light.AmbientColorZ = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fAmbient.z * 255.0f;
	light.AmbientColorW = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fAmbient.w * 255.0f;
																									    
	light.DiffuseColorX = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fDiffuse.x * 255.0f;
	light.DiffuseColorY = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fDiffuse.y * 255.0f;
	light.DiffuseColorZ = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fDiffuse.z * 255.0f;
	light.DiffuseColorW = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fDiffuse.w * 255.0f;

	light.SpecularColorX = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fSpecular.x * 255.0f;
	light.SpecularColorY = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fSpecular.y * 255.0f;
	light.SpecularColorZ = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fSpecular.z * 255.0f;
	light.SpecularColorW = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_v4fSpecular.w * 255.0f;

	light.SpecularIntensity = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_fSpecularIntensity;
	light.SpecularPower = LightManager::GetInstance()->GetLights()[_nArrayPos].m_pLight->m_fSpecularPower;

	return light;
}

void WINAPI _ChangeCameraPos (VEC3F pos)
{
	GetCam()->SetViewPosition(pos.x , pos.y, -1000.0f);
}