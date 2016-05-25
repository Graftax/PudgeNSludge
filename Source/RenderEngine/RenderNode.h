//////////////////////////////////////////////////////////////////////////////////////
// Filename:            RenderNode.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A pointer to be used to make render sets,
//						stores all render information needed
//////////////////////////////////////////////////////////////////////////////////////
#ifndef RENDERNODE_H_
#define RENDERNODE_H_


#include "RenderEngine.h"
#include "MeshManager.h"
#include "../ObjectLib/Objects/IBaseObject.h"
//Pointer to this render nodes render call
typedef void (*RenderFunc)(RenderNode &node);

__declspec(align(32))
class RenderNode
{
private:
	//We have tons of friends, YAYYY!!
	friend class MeshManager;
	friend class RenderEngine;
	friend class RenderSet;
	friend class CComponent_Animation;
	friend class IBaseObject;
	friend class RenderSetSorted;
	friend class LightManager;
	friend class CObjectFactory;
	//Pointer to the next node, used within a RenderSet
	RenderNode* m_pNext;
	RenderNode* m_pOwner;
	//A structure to all information needed to render this node
	RenderHandle renderData;	

	//bool m_bAnimated;

	unsigned int* m_unBitFlags;
	RenderNode& operator=(const RenderNode& that)
	{
		if( this != &that )
		{
			renderData = that.renderData;
			Render = that.Render;
			renderData.m_vTransform.clear();
			renderData.m_vCollisions.clear();
			m_pNext = nullptr;
			m_unBitFlags = that.m_unBitFlags;
			m_pOwner = that.m_pOwner;
			m_fExtraData = that.m_fExtraData;
		}

		return *this;
	}
	float m_fExtraData;

public:
	//constructor and destructor
	RenderNode(void) { m_pNext = nullptr; m_pOwner = this; m_fExtraData = 1.0f; m_unBitFlags = 0u; }
	RenderNode( RenderNode* that );

	~RenderNode(void)
	{
		//ClearExtraData();
	}
	//The nodes render function within the render engine
	RenderFunc Render;
	void AddFlags( unsigned int _bitFlags );
	void ClearFlags( unsigned int _bitFlags );
	void ClearAllFlags(void);
	bool TestFlag( unsigned int _bitFlags );
	//Process a render call
	void RenderProcess(void)
	{
		m_pOwner->Render(*this);
	}	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FillRenderData(): Fills out the render information for this node
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FillRenderData( const void* _vertexData, unsigned int _numVerts, UINT _stride, const void* _indexData, unsigned int _numIndices )
	{
		renderData = MeshManager::GetInstance()->AddVerts( _vertexData, _numVerts, _stride, _indexData, _numIndices );
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FillAnimRenderData(): Fills out the render information for this node
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void FillAnimRenderData( const void* _vertexData, unsigned int _numVerts, UINT _stride, const void* _indexData, unsigned int _numIndices )
	{
		renderData = MeshManager::GetInstance()->AddAnimVerts( _vertexData, _numVerts, _stride, _indexData, _numIndices );
	}	
	/////////////////////////////
	//  Accessors
	/////////////////////////////
	inline RenderNode* GetNext(void) { return m_pNext; }
	inline RenderHandle& GetRenderData(void) { return renderData; }
	/*void ClearExtraData(void); */
	/////////////////////////////
	// Mutators
	/////////////////////////////
	inline void SetNext(RenderNode* pNext) { m_pNext = pNext; }
	inline void SetMatrixData( D3DXMATRIX _mat, unsigned int _index ) 
	{
		if( _index >= renderData.m_vTransform.capacity() || _index < 0 )
			return;
		else
			renderData.m_vTransform[_index] = _mat;
	}
	bool SetExtraData( float _pData );
	inline float GetExtraData(void) { return m_fExtraData; }
};






#endif //RENDERNODE_H_