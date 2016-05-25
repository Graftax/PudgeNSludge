#ifndef RENDERSETSORTED_H_
#define RENDERSETSORTED_H_

#include "RenderNode.h"

//__declspec(align(32))
class RenderSetSorted
{
private:
	friend class RenderEngine;

	std::vector< RenderNode* > m_vNewMemoryPool;

	RenderNode* m_pHead;
	RenderNode* m_pTail;

	RenderNode* GetAvailableNode( RenderNode* pNode );
	void SendBackToPool( RenderNode* _addBack );

public:
	RenderSetSorted(void);
	~RenderSetSorted(void);

	//void DestroyRenderSet(void); //Memory Handled in MeshManager and Destructor
	void AddRenderNode(RenderNode *pNode);
	void ClearRenderSet(void);
	void DeleteCopyMemoryPool(void);

	RenderNode *GetHead() { return m_pHead; }
	RenderNode *GetTail() { return m_pTail; }

	bool ZSortGreater(float *left, float *right);
	bool ZSortSmaller(float *left, float *right);

};








#endif //RENDERSETSORTED_H_