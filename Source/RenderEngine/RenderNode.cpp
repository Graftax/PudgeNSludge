#include "RenderNode.h"
//#include "RenderEngine.h"
//#include "MeshManager.h"

RenderNode::RenderNode( RenderNode* that )
{
	renderData = that->renderData;
	Render = that->Render;
	renderData.m_vTransform.clear();
	renderData.m_vCollisions.clear();
	m_pNext = nullptr;
	m_unBitFlags = that->m_unBitFlags;
	m_pOwner = that->m_pOwner;
	m_fExtraData = that->m_fExtraData;
}

void RenderNode::AddFlags( unsigned int _bitFlags )
{
	*m_unBitFlags |= _bitFlags;
}

void RenderNode::ClearFlags( unsigned int _bitFlags )
{
	*m_unBitFlags &= ~(_bitFlags);
}

void RenderNode::ClearAllFlags(void)
{
	*m_unBitFlags = 0u;
}

bool RenderNode::TestFlag( unsigned int _bitFlags )
{
	if( *m_unBitFlags & _bitFlags )
	{
		return true;
	}
	return false;
}

//void RenderNode::ClearExtraData( void )
//{ 
//	if( m_pExtraData ) 
//	{ 
//		delete m_pExtraData; 
//		m_pExtraData = nullptr; 
//	} 
//}

bool RenderNode::SetExtraData( float _pData )
{
	m_fExtraData = _pData;
	return true;
	//if( m_pExtraData == nullptr )
	//{ 
	//	m_pExtraData = _pData; 
	//	return true; 
	//} 
	//if( *m_pExtraData < 0.0f )
	//{
	//	delete m_pExtraData;
	//	m_pExtraData = _pData;
	//	return true;
	//}
	//delete _pData; 
	//_pData = nullptr; 
	//return false; 
}
//void RenderNode::AddVerts( const void* _vertexData, unsigned int _size )
//{
//	//RenderEngine::InitVertexBuffer( &m_pVertexBuffer, _size );
//
//	/*void* _buffer;
//	m_pVertexBuffer->Lock(0,0,&_buffer,0);
//	memcpy( _buffer, _vertexData, _size );
//	m_pVertexBuffer->Unlock();
//
//	delete _vertexData;*/
//}
//
//void RenderNode::AddIndices( const void* _indexData, unsigned int _size )
//{
//	//RenderEngine::InitIndexBuffer( &m_pIndexBuffer, _size );
//
//	/*void* _buffer;
//	m_pIndexBuffer->Lock(0,0,&_buffer,0);
//	memcpy( _buffer, _indexData, _size );
//	m_pIndexBuffer->Unlock();
//
//	delete _indexData;*/
//}
