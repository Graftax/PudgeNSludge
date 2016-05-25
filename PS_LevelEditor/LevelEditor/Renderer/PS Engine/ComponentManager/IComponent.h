#ifndef ICOMPONENT_H
#define ICOMPONENT_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:			IComponent.h
// Author:				Ethan Pendergraft
// Date:				5/8/12
// Purpose:				The interface for components into the component manager.
//////////////////////////////////////////////////////////////////////////////////////

enum eComponentTypes{	ECOMP_NULL, 
						ECOMP_AI,
						ECOMP_INPUT,
						ECOMP_ANIMATION,
						ECOMP_ITEM,
						ECOMP_SPAWN,
						ECOMP_EMITTER,
						ECOMP_HEALTH,
						ECOMP_MESHEFFECT,
						ECOMP_CHECKPOINT,
						ECOMP_COLLISION,
						ECOMP_SOUND,
						ECOMP_RENDER, 
						ECOMP_MAX
								}; 

#include "../ObjectLib/Objects/IBaseObject.h"

#define SOUND_RANGE 360000.0f

__declspec(align(32))
class IComponent
{

private:

	// The type of component this is
	unsigned int m_unCompType;

protected:

	// The handle to the parent component
	IBaseObject* m_pParent;

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IComponent():	THe constructor overload.
	//
	// Returns:			Void                              
	//
	// Mod. Name:       EP
	// Mod. Date:       5/9/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent( unsigned int _unType ) : m_unCompType( _unType ), m_pParent( nullptr ) {}
	IComponent( unsigned int _unType, IBaseObject* _pParent ) : m_unCompType( _unType ), m_pParent( _pParent ) {}

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	unsigned int GetType() { return m_unCompType; }
	IBaseObject* GetParent() { return m_pParent; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetParent( IBaseObject* _pParent ) { m_pParent = _pParent; }

	////////////////////////////////////////////////////////////////////////////
	//Initialize():		The initialize that will be used for some components.
	//					Used to initialize values in a component to a starting
	//					value.
	//
	//Returns:			void
	//
	//Mod. Name:		JM
	//Mod. Date:		5/11/12
	////////////////////////////////////////////////////////////////////////////
	virtual bool Initialize(void) { return true; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():		THe update function template that should be overloaded for updates.
	//
	// Returns:			Void                              
	//
	// Mod. Name:       EP
	// Mod. Date:       5/9/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Update( float fDT ) {};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render():		The render function template that should be overloaded for renders.
	//
	// Returns:			Void                              
	//
	// Mod. Name:       KC
	// Mod. Date:       5/30/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Render(  ) {};

	///////////////////////////////////////////////////////////////////////////
	//Shutdown():		This function will be used by components to free any
	//					memory that has been allocated by the component.
	//
	//Returns:			void
	//
	//Mod. Name:		JM
	//Mod. Date:		5/11/12
	///////////////////////////////////////////////////////////////////////////
	virtual void Shutdown(void) {};

	// COMPONENTMANAGER IS YOUR FRIEND! IT DOES WHAT IT WANTS!
	friend class CComponentManager;
};

#endif