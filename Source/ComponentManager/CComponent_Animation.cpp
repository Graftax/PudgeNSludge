//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Animation.cpp
// Author:              Andy Madruga
// Date:                5/14/12
// Purpose:             Defines the 
//////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_Animation.h"
#include "../AnimationLib/AnimationStateMachine.h"
#include "CComponent_Render.h"
#include "../RenderEngine/RenderNode.h"

// Constructor
CComponent_Animation::CComponent_Animation() : IComponent(ECOMP_ANIMATION)
{
	m_pAnimStateMachine = nullptr;
	m_pPose = nullptr;
	m_pRenderCmp = nullptr;
	m_vBlendedFrames.clear();
	m_Interpolator.SetTime(0.0f);
	m_BlendToInterpolator.SetTime(0.0f);
	m_bIsAnimatedObjectInScene = false;

}

CComponent_Animation::~CComponent_Animation()
{
}

//////////////////////////////////////////////////////////////////
//	Initialize():	Initializes the Component and sets all values
//					to default values to avoid crashes/bugs.
//
//	Returns:		bool - returns true if everything was initialized.
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
bool CComponent_Animation::Initialize(void)
{
	m_vBlendedFrames.clear();
	m_Interpolator.SetTime(0.0f);
	m_BlendToInterpolator.SetTime(0.0f);
	m_bIsAnimatedObjectInScene = false;

	m_pAnimStateMachine = new AnimationStateMachine(this);

	if( !m_pAnimStateMachine )
		return false;

	m_pRenderCmp = (CComponent_Render*)GetParent()->GetComponent(ECOMP_RENDER);

	return true;
};

//////////////////////////////////////////////////////////////////
//	Initialize():	Shutsdown the component and its state machine.
//
//	Returns:		void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void CComponent_Animation::Shutdown(void)
{
	if( m_pAnimStateMachine)
		delete m_pAnimStateMachine;

	m_pAnimStateMachine = nullptr;
	m_pPose = nullptr;
	m_pRenderCmp = nullptr;
	m_vBlendedFrames.clear();
	m_Interpolator.SetTime(0.0f);
	m_BlendToInterpolator.SetTime(0.0f);
	m_bIsAnimatedObjectInScene = false;
}

//////////////////////////////////////////////////////////////////
//	Update():	Calls Update on the State Machine and updates the
//				current State of the Animation.
//
//	Returns:	void
//
//	Modified By:
//	Modified Date:	
//////////////////////////////////////////////////////////////////
void CComponent_Animation::Update(float fTime)
{
	if( !GetParent() )
		return;

	if( !m_pRenderCmp )
		return;

	// Check to see if the object is in the frustum otherwise do not render it.
	RenderNode* pNode = m_pRenderCmp->GetRenderNode();
	CCollisionAABB aabb = pNode->m_pOwner->GetRenderData().m_vCollisions[0];
	aabb.SetWorldPoint( m_pParent->GetFrame().GetWorldMat().axis_pos);
	if( !RenderEngine::GetFrustum()->infrustum(  aabb ))
	{
		m_bIsAnimatedObjectInScene = false;
	}
	else
		m_bIsAnimatedObjectInScene = true;


	if( m_pAnimStateMachine )
		m_pAnimStateMachine->Update(fTime);
	
	// If object is not in scene just return out.
	if( !m_bIsAnimatedObjectInScene )
		return;

	// Add on the Animation Frames to this List.
	for( unsigned int i = 0; i < m_vBlendedFrames.size(); ++i )
	{
		m_pRenderCmp->GetRenderNode()->renderData.m_vTransform.push_back( (*(D3DXMATRIX*)&m_vBlendedFrames[i].GetWorldMat()) );
	}
}