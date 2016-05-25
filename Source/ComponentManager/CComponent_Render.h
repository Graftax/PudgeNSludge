#ifndef CCOMPONENT_RENDERH
#define CCOMPONENT_RENDERH

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Render.h
// Author:              Ethan Pendergraft
// Date:                5/11/12
// Purpose:             Stores the necessary data to render an object.
//////////////////////////////////////////////////////////////////////////////////////

#include "IComponent.h"
#include "../RenderEngine/RenderNode.h"

__declspec(align(32))
class CComponent_Render : public IComponent
{

private:

	RenderNode* m_pRenderNode;

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CComponent_Render():	Default constructor overload.
	//
	// Returns:				Void
	//
	// Mod. Name:			EP
	// Mod. Date:			5/9/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CComponent_Render() : IComponent( ECOMP_RENDER ), m_pRenderNode( nullptr ) {}

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	RenderNode* GetRenderNode() { return m_pRenderNode; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetRenderNode( RenderNode* _toSet ) { m_pRenderNode = _toSet; }

	void Shutdown( void );
};

#endif // CCOMPONENT_RENDERH