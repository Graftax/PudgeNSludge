#include "IBaseObject.h"
#include "EntryPoint.h"

IBaseObject::IBaseObject () : m_HazardType (HZ_MAX), m_ObjectType (OT_MAX)
{
	D3DXMatrixIdentity (&m_WorldMatrix);
}

IBaseObject::~IBaseObject ()
{

}

void IBaseObject::Render ()
{
	if (m_pRenderNode)
	{
		GetRenderer ()->AddToObjectRenderSet ( m_pRenderNode, m_WorldMatrix );
	}
}