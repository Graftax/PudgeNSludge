#include "ObjectManager.h"
#include "EntryPoint.h"

CObjectManager* CObjectManager::m_pInstance = NULL;

CObjectManager* CObjectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CObjectManager();
	}
	return m_pInstance;
}

void CObjectManager::DeleteInstance ()
{
	delete m_pInstance;
	m_pInstance = NULL;
}

void CObjectManager::AddObject (IBaseObject* _iboObject)
{
	m_vObjects.push_back(_iboObject);
	if ( _iboObject->GetObjectType () == OT_MAX && _iboObject->GetHazardType () != HZ_MAX)
	{
		D3DXVECTOR4 TransformPos = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		D3DXVECTOR4 FinalPos;
		D3DXVec4Transform (&FinalPos, &TransformPos, &_iboObject->GetWorldMatrix());
		GetHazards().push_back (D3DXVECTOR3 (FinalPos.x, FinalPos.y, FinalPos.z));
		GetHazardTypes().push_back ((int)_iboObject->GetHazardType());
	}
	
	if ( _iboObject->GetObjectType () != OT_MAX && _iboObject->GetHazardType () == HZ_MAX)
	{
		//D3DXVECTOR4 TransformPos = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		//D3DXVECTOR4 FinalPos;
		//D3DXVec4Transform (&FinalPos, &TransformPos, &_iboObject->GetWorldMatrix());
		GetObjects().push_back (_iboObject);
		GetObjectTypes().push_back ((int)_iboObject->GetObjectType());
	}
}

void CObjectManager::RemoveObject (IBaseObject* _iboObject)
{
	for (std::vector<IBaseObject*>::iterator itr = m_vObjects.begin(); itr != m_vObjects.end(); itr++)
	{
		if ( (*itr) == _iboObject)
		{
			delete (*itr);
			m_vObjects.erase (itr);
			break;
		}
	}
}

void CObjectManager::ClearAll ()
{
	for (unsigned int Index = 0; Index < m_vObjects.size(); Index++)
	{
		delete m_vObjects[Index];
	}
	m_vObjects.clear();
}

void CObjectManager::ShutDown ()
{
	ClearAll ();
}

void CObjectManager::Render ()
{
	for (unsigned int Index = 0; Index < m_vObjects.size(); Index++)
	{
		m_vObjects[Index]->Render();
	}
}