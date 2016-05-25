//////////////////////////////////////////////////////////////////////////////////////
// Filename:            MeshManager.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all loaded meshes
//////////////////////////////////////////////////////////////////////////////////////
#include "RenderSet.h"

RenderSet::RenderSet(void)
{
	ClearRenderSet();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClearRenderSet(): Clears the render set without clearing the memory
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderSet::ClearRenderSet(void)
{
	m_pHead = m_pTail = nullptr;
	drawCalls = 0u;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DestroyRenderSet(): Destroys the render set and the render node memory contained within it.
//                    DEPRECATED! Memory now managed by the mesh manager.   
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderSet::DestroyRenderSet(void) 
{
	while(m_pHead)
	{
		RenderNode* deleteMe = m_pHead;
		m_pHead = deleteMe->GetNext();
		delete deleteMe;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddRenderNode(): Add a render node to the set
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderSet::AddRenderNode(RenderNode *pNode)
{
	if( pNode == nullptr )
		return;

	RenderNode* pFind = m_pHead;
	while(pFind)
	{
		if( pFind == pNode )
			return;
		pFind = pFind->GetNext();
	}
	drawCalls++;
	if( nullptr == m_pHead )
	{
		m_pHead = m_pTail = pNode;
		m_pHead->SetNext(nullptr); 
		m_pTail->SetNext(nullptr);
	}
	else
	{
		m_pTail->SetNext(pNode);
		m_pTail = pNode;
		m_pTail->SetNext(nullptr);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClearObjectRenderSet(): Clears the render set and the matrix vectors
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderSet::ClearObjectRenderSet(void)
{
	while( m_pHead )
	{
		m_pHead->renderData.m_vTransform.clear();
		m_pHead = m_pHead->m_pNext;
	}
	m_pHead = m_pTail = nullptr;
	drawCalls = 0u;
}