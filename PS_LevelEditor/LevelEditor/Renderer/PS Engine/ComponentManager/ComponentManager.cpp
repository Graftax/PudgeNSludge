#include "ComponentManager.h"
#include "IComponent.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	.	Update the component system.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponentManager::Update( float _fDT )
{
	std::map< unsigned int, std::list< IComponent* > >::iterator currList = m_cpActiveComponents.begin();

	while( currList != m_cpActiveComponents.end() )
	{
		std::list< IComponent* >::iterator currComp = (*currList).second.begin();

		while( currComp != (*currList).second.end() )
		{
			
			(*currComp)->Update( _fDT );
			if( !m_bDunskies )
			{
				return;
			}
				currComp++;

		}

		currList++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render():	.	Renders the component system. This function was made so any components that have 
// particles can be rendered.
//
// Returns:			Void                              
//
// Mod. Name:       KC
// Mod. Date:       5/30/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponentManager::Render()
{
	std::map< unsigned int, std::list< IComponent* > >::iterator currList = m_cpActiveComponents.begin();

	while( currList != m_cpActiveComponents.end() )
	{
		std::list< IComponent* >::iterator currComp = (*currList).second.begin();

		while( currComp != (*currList).second.end() )
		{

			(*currComp)->Render(  );
			if( !m_bDunskies )
			{
				return;
			}
			currComp++;

		}

		currList++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddComponent():	Adds a component to the active component system.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponentManager::AddComponent( IComponent* _pToAdd )
{
	m_bDunskies = true;

	if( _pToAdd )
	{
		m_cpActiveComponents[ _pToAdd->GetType() ].push_back( _pToAdd );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveComponent():	Removes a component from the active component system. 
//						Note: This does not clean up any memory.
//
// Returns:				Void                              
//	
// Mod. Name:			EP
// Mod. Date:			5/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponentManager::RemoveComponent( IComponent* _pToRemove )
{
	if( _pToRemove )
	{
		m_cpActiveComponents[ _pToRemove->GetType() ].remove( _pToRemove );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Clear():			Clears all components from the active system.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponentManager::Clear()
{
	m_bDunskies = false;
	m_cpActiveComponents.clear();
}