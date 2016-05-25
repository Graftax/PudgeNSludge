/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: IBaseObject
//
// Purpose: Contain position data
//
// Original Author: Rueben Massey
//
// Creation Date: 5/8/2012
//
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "IBaseObject.h"


#include "..//..//ComponentManager/IComponent.h"
#include "../../ComponentManager/CComponent_Render.h"
#include "../../RenderEngine/RenderHandle.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: IBaseObject
// Purpose: Default Constructor for all Objects
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject::IBaseObject ()
{
	m_ObjectFrame.GetWorldMat().make_identity();
	m_sReferences = 0;
	m_eType = OBJ_NULL;
	m_bAlive = true;
	m_bRendered = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: IBaseObject
// Purpose: Default Destructor, should be defined in all objects the allocate memory
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject::~IBaseObject ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: AddComponent
// Purpose: Add the component to the component list
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IBaseObject::AddComponent( IComponent* pComponent )
{
	// Input Safety Check
	if( !pComponent )
		return;

	// This checks to see if the object already has
	// the component that is trying to be added
	if( !m_vComponentList.empty() )
	{
		for( std::list<IComponent*>::iterator iter = m_vComponentList.begin(); iter != m_vComponentList.end(); ++iter )
		{
			if( pComponent->GetType() == (*iter)->GetType() )
			{
				return;
			}
		}
	}
	// Set the component's parent to this object
	pComponent->SetParent( this );
	// add the component to the list
	m_vComponentList.push_back( pComponent );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetComponent
// Purpose: Get a component from the component list
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* IBaseObject::GetComponent( const unsigned int eComponentType )
{
	// Only try to get components
	// if the list is not empty
	if( !m_vComponentList.empty() )
	{
		// loop through the component list
		for( std::list<IComponent*>::iterator iter = m_vComponentList.begin(); iter != m_vComponentList.end(); ++iter )
		{
			// check if this component matches the component type
			if( (eComponentTypes)eComponentType == (*iter)->GetType() )
			{
				return *iter;
			}
		}
	}
	// if the component wasn't found
	// return null
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: RemoveComponent
// Purpose: Remove the component from the component list DO NOT USE...YET
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IBaseObject::ReleaseComponent( IComponent* pComponent )
{
	// Input Safety Check
	if( pComponent )
	{
		m_vComponentList.remove( pComponent );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Release
// Purpose: Removes a reference. YOU MUST USE THIS WHEN YOU STOP USING THE POINTER THAT IS POINTING
//			AT THE OBJECT
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void IBaseObject::Release()
{
	--m_sReferences;
	if (m_sReferences < 1)
	{
		// Remove from Object Factory
		// by resetting values
		m_ObjectFrame.GetWorldMat().make_identity();
		m_sReferences = 0;
		m_eType = OBJ_NULL;

		//// Reset component parents
		for( std::list<IComponent*>::iterator iter = m_vComponentList.begin(); iter != m_vComponentList.end(); ++iter )
		{
			(*iter)->SetParent( nullptr );
		}
		m_vComponentList.clear();
		delete pCollisionVolume;
		pCollisionVolume = NULL;
	}
}

void IBaseObject::Render()
{
	if( m_bRendered )
	{
		CComponent_Render* pToRender = (CComponent_Render*)GetComponent( ECOMP_RENDER );
		if( pToRender ) 
		{
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			matrix4f worldMat = m_ObjectFrame.GetWorldMat();

			mat._11 = worldMat.xx;
			mat._12 = worldMat.xy;
			mat._13 = worldMat.xz;
			mat._14 = worldMat.xw;

			mat._21 = worldMat.yx;
			mat._22 = worldMat.yy;
			mat._23 = worldMat.yz;
			mat._24 = worldMat.yw;

			mat._31 = worldMat.zx;
			mat._32 = worldMat.zy;
			mat._33 = worldMat.zz;
			mat._34 = worldMat.zw;

			mat._41 = worldMat.wx;
			mat._42 = worldMat.wy;
			mat._43 = worldMat.wz;
			mat._44 = worldMat.ww;

			RenderEngine::AddToObjectRenderSet( pToRender->GetRenderNode(), mat );
		}
	}
}

matrix4f IBaseObject::GetPositionMatrix(void)
{
	matrix4f returnMatrix;
	returnMatrix.make_identity();

	returnMatrix.axis_pos = m_ObjectFrame.GetWorldMat().axis_pos;

	return returnMatrix;
}

