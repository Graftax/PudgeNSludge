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
	m_vBlendedFrames.clear();
	m_Interpolator.SetTime(0.0f);
	m_BlendToInterpolator.SetTime(0.0f);
	m_bBlendStateBackward = false;
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
	m_pAnimStateMachine = nullptr;
	m_pPose = nullptr;
	m_vBlendedFrames.clear();
	m_Interpolator.SetTime(0.0f);
	m_BlendToInterpolator.SetTime(0.0f);
	m_bBlendStateBackward = false;

	m_pAnimStateMachine = new AnimationStateMachine(this);

	if( !m_pAnimStateMachine )
		return false;

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

	if( m_pAnimStateMachine )
		m_pAnimStateMachine->Update(fTime);
	
	CComponent_Render* rendercmp = (CComponent_Render*)GetParent()->GetComponent(ECOMP_RENDER);

	int size = rendercmp->GetRenderNode()->renderData.m_vTransform.size();
	D3DXMATRIX* originalMat = NULL;
	if( size == 1 )
	{
		originalMat = &rendercmp->GetRenderNode()->renderData.m_vTransform[size-1];
		// Clear the List to get rid of excess data
		rendercmp->GetRenderNode()->renderData.m_vTransform.clear();
	}

	// Add on the Animation Frames to this List.
	for( unsigned int i = 0; i < m_vBlendedFrames.size(); ++i )
	{
		rendercmp->GetRenderNode()->renderData.m_vTransform.push_back( (*(D3DXMATRIX*)&m_vBlendedFrames[i].GetWorldMat()) );
	}

	if( originalMat )
		/*rendercmp->GetRenderNode()->renderData.m_vTransform.push_back(*originalMat);
	else*/
		rendercmp->GetRenderNode()->renderData.m_vTransform.push_back( (*(D3DXMATRIX*)rendercmp->GetParent()->GetWorldMat()) );
}