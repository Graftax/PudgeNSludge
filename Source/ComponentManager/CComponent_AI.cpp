//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CComponent_AI.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds the information for basic AI
//////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_AI.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../AI/IBaseAI.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_AI():	Default Constructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_AI::CComponent_AI () : IComponent (ECOMP_AI)
{
	m_vLoadedAIs.clear();
	m_vLoadedAIs.resize (AI_MAX, nullptr);
	m_pCurrAI = nullptr;

	//m_aiType = AI_NULL;
	//m_pTarget = NULL;
	//m_pCurPlatform = NULL;
	//m_bOnGround = false;
	//m_v3Velocity = vec3f (0.0f, 0.0f, 0.0f);
	//m_bOnGround = false;
	m_bActive = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CComponent_AI():	Default Destructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_AI::~CComponent_AI ()
{
	Shutdown ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize():	Initialize, Inits all ai information
//
// Returns:		bool
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_AI::Initialize ()
{
	//m_vLoadedAIs.clear();
	//m_vLoadedAIs.resize (AI_MAX, nullptr);
	//m_pCurrAI = nullptr;

	// Init default values to null
	//m_aiType = AI_NULL;
	//m_pTarget = NULL;
	//m_pCurPlatform = NULL;
	//m_v3Velocity = vec3f (0.0f, 0.0f, 0.0f);
	//m_bOnGround = false;

	InitLoadedAI();

	m_bActive = true;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	Shutdown
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_AI::Shutdown ()
{
	for (unsigned int AIindex = 0; AIindex < m_vLoadedAIs.size(); AIindex++)
	{
		if (m_vLoadedAIs[AIindex])
		{
			m_vLoadedAIs[AIindex]->Shutdown();
		}

		delete m_vLoadedAIs[AIindex];
		m_vLoadedAIs[AIindex] = nullptr;
	}

	m_pCurrAI = nullptr;

	// set the target and ai type to null
	//m_pTarget = NULL;
	//m_aiType = AI_NULL;
	//m_pTarget = NULL;
	//m_pCurPlatform = NULL;
	//m_v3Velocity = vec3f (0.0f, 0.0f, 0.0f);
	//m_bOnGround = false;
	m_bActive = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update(float _fElapsedTime):	virtual Update, updates the ai information
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_AI::Update ( float _fElapsedTime )
{
	if (m_pCurrAI && m_bActive)
	{
		m_pCurrAI->Update(_fElapsedTime);
	}
	// there is no AI so dont update
	//if (m_aiType == AI_NULL)
	//{
	//	return;
	//}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SwitchAI(AITypes _eType):	Changes the current ai to an ai that is in the loaded ai vector based on the ai type
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_AI::SwitchAI ( AIType _eType )
{
	// make sure the type of ai we are switching to is inside the bounds
	// then set the current ai to the loaded ai of that type
	if ( _eType > AI_NULL && _eType < AI_MAX)
	{
		m_pCurrAI = m_vLoadedAIs[_eType];
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddAI ( AITypes _eType, IBaseAI* _pAI ):	Addes the passed in base ai to the loaded ai vector, this inits and will
//											delete the allocated ai if there is any.
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_AI::AddAI ( AIType _eType, IBaseAI* _pAI )
{
	// make sure the type of ai is inside of the bounds
	// if so then set the loaded at of that type equal to the passed in ai
	if ( _eType > AI_NULL && _eType < AI_MAX)
	{
		delete m_vLoadedAIs[_eType];
		_pAI->SetParentAI (this);
		m_vLoadedAIs[_eType] = _pAI;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetLoadedAI (AITypes _eType): returns the base ai in the loaded ai vector based on the type you want returned.
//								 this can return null for a base ai if one wasnt loaded please check for that.
//
// Returns:		IBaseAI*
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseAI* CComponent_AI::GetLoadedAI (AIType _eType)
{
	// make sure the passed in type is valid
	// then return the ai at the position in the loaded ai vector
	if ( _eType > AI_NULL && _eType < AI_MAX)
	{
		return m_vLoadedAIs[_eType];
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitLoadedAI (): Loops through all the loaded AI and inits them.
//
// Returns:		void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_AI::InitLoadedAI ()
{
	//Init all loaded ai
	for (unsigned int index = 0; index < m_vLoadedAIs.size (); index++)
	{
		if (m_vLoadedAIs[index])
		{
			m_vLoadedAIs[index]->Init();
		}
	}
}

