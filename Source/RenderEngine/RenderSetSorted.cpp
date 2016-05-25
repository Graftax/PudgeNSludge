#include "RenderSetSorted.h"


RenderSetSorted::RenderSetSorted(void)
{
	m_pHead = m_pTail = nullptr;
}

RenderSetSorted::~RenderSetSorted(void)
{
	ClearRenderSet();
	//for( unsigned int i = 0u; i < m_vNewMemoryPool.size(); ++i )
	//{
	//	//if( m_vNewMemoryPool[i]->m_pExtraData )
	//		//m_vNewMemoryPool[i]->ClearExtraData();
	//	if( m_vNewMemoryPool[i] )
	//		delete m_vNewMemoryPool[i];
	//	m_vNewMemoryPool[i] = nullptr;
	//}
}

RenderNode* RenderSetSorted::GetAvailableNode( RenderNode* pNode )
{
	if( m_vNewMemoryPool.size() == 0 )
	{
		RenderNode* _returnNode = new RenderNode( pNode );
		return _returnNode;
	}
	else
	{
		RenderNode* _returnNode = m_vNewMemoryPool.back();
		_returnNode->Render = pNode->Render;
		_returnNode->renderData = pNode->renderData;

		// Pop back the last matrix to keep yourself from having two transforms for its position.
		// This fixes animations when pausing and resuming game.
		_returnNode->renderData.m_vTransform.pop_back();

		_returnNode->m_unBitFlags = pNode->m_unBitFlags;
		_returnNode->m_pOwner = pNode->m_pOwner;
		_returnNode->m_fExtraData = pNode->m_fExtraData;
		m_vNewMemoryPool.pop_back();
		return _returnNode;
	}
	RenderNode* _returnNode = new RenderNode( pNode );
	return _returnNode;
}

void RenderSetSorted::SendBackToPool( RenderNode* _addBack )
{
	//if( _addBack->m_pExtraData )
	//{
	//	delete _addBack->m_pExtraData;
	//	_addBack->m_pExtraData = nullptr;
	//}
	m_vNewMemoryPool.push_back( _addBack );
}

bool RenderSetSorted::ZSortGreater(float *left, float *right)
{
	return (*left > *right);
}

bool RenderSetSorted::ZSortSmaller(float *left, float *right)
{
	return (*left < *right);
}

void RenderSetSorted::AddRenderNode(RenderNode *pNode)
{
	RenderNode* current;
	RenderNode* previous;

	unsigned int _unNumTrans = pNode->renderData.m_vTransform.size();
	unsigned int _unNumColl = pNode->renderData.m_vCollisions.size();
	unsigned int i = 0u;
	unsigned int colIndex = 0;

	if( pNode->TestFlag( RE_ANIMATED ) )
	{
		i = _unNumTrans - 1;

		if( _unNumColl < 1 )
		{
			pNode->renderData.m_vCollisions.push_back( pNode->m_pOwner->GetRenderData().m_vCollisions[0]);
			_unNumColl = pNode->renderData.m_vCollisions.size();
		}
	}

	if( _unNumTrans == 0 )
		return;

	if( nullptr == m_pHead )
	{
		RenderNode* toAdd = GetAvailableNode( pNode );
		toAdd->renderData.m_vTransform.push_back( pNode->renderData.m_vTransform[i] );
		if( _unNumColl > colIndex )
			toAdd->renderData.m_vCollisions.push_back( pNode->renderData.m_vCollisions[colIndex++] );
		m_pHead = m_pTail = toAdd;
		m_pHead->SetNext(nullptr);
		++i;
	}
	bool continueInLoop = false;
	//LOG( LOG_INFO, "RenderSetSorted::AddRenderNode", "NewNodeToSort" );
	for( ; i < _unNumTrans; ++i )
	{
		current = m_pHead;
		previous = nullptr;
		continueInLoop = false;
		RenderNode* toAdd = GetAvailableNode( pNode );
		toAdd->renderData.m_vTransform.push_back( pNode->renderData.m_vTransform[i] );
		if( _unNumColl > colIndex )
			toAdd->renderData.m_vCollisions.push_back( pNode->renderData.m_vCollisions[colIndex++] );
		while( current )
		{
			if( ZSortSmaller( &current->renderData.m_vTransform[0]._43, &toAdd->renderData.m_vTransform[0]._43 ) )
			{
				if( nullptr == previous )
				{
					m_pHead = toAdd;
					m_pHead->SetNext(current);	
					continueInLoop = true;
					break;
				}
				else
				{
					previous->SetNext(toAdd);
					toAdd->SetNext(current);
					continueInLoop = true;
					break;
				}
			}
			previous = current;
			current = current->GetNext();
		}
		if( !continueInLoop )
		{
			m_pTail->SetNext(toAdd);
			toAdd->SetNext(nullptr);
			m_pTail = toAdd;
		}
	}
	pNode->renderData.m_vTransform.clear();
}

void RenderSetSorted::ClearRenderSet(void)
{
	while( m_pHead )
	{
		m_pHead->renderData.m_vTransform.clear();
		SendBackToPool( m_pHead );
		m_pHead = m_pHead->GetNext();
	}
	m_pHead = m_pTail = nullptr;
}

void RenderSetSorted::DeleteCopyMemoryPool(void)
{
	for( unsigned int i = 0u; i < m_vNewMemoryPool.size(); ++i )
	{
		//m_vNewMemoryPool[i]->ClearExtraData();

		if( m_vNewMemoryPool[i] )
			delete m_vNewMemoryPool[i];
		m_vNewMemoryPool[i] = nullptr;
	}
	m_vNewMemoryPool.clear();
}