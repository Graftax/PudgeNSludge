//////////////////////////////////////////////////////////////////////////////////////
// Filename:            MeshManager.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all loaded meshes
//////////////////////////////////////////////////////////////////////////////////////
#ifndef MESHMANAGER_H_
#define MESHMANAGER_H_

#include "RenderEngine.h"
#include <vector>

class BaseMesh;
class RenderNode;
class TextureManager;
class CCollisionVolume;
class Mesh;

//Structure to store already loaded mesh data, also manages the render node memory
struct loadedMeshData
{
	std::string m_sMeshName;
	RenderNode* m_pNode;
	CCollisionVolume* m_pColVolume;
	unsigned int* m_unBitFlags;

	loadedMeshData(void)
	{
		m_sMeshName = "";
		m_pNode = nullptr;
		m_pColVolume = nullptr;
		m_unBitFlags = new unsigned int(0u);
	}

};

//SINGLETON CLASS USED FOR LOADING AND STORING MESHES
__declspec(align(32))
class MeshManager
{
private:
	//Give the render engine complete access
	friend class RenderEngine;
	
	////////////////////////////////////////////////////////////////////////////////
	//This class is a singleton 
	MeshManager(){}  // Private so that it can  not be called
	MeshManager(MeshManager const&){}            // copy constructor is private
	MeshManager& operator=(MeshManager const&){}  // assignment operator is private
	////////////////////////////////////////////////////////////////////////////////

	//DRX COM Objects used for storing all mesh and animation data
	IDirect3DVertexBuffer9* m_pSharedVertexBuffer;
	IDirect3DIndexBuffer9* m_pSharedIndexBuffer;
	IDirect3DVertexBuffer9* m_pAnimationVertexBuffer;
	IDirect3DIndexBuffer9* m_pAnimationIndexBuffer;

	//Offsets in bytes within the buffers
	UINT m_uiCurrentVertBuffOffset;
	UINT m_uiAnimCurrentVertBuffOffset;

	//Size to increase the buffers by( ie. 1984 verts - increase buffer by 2000 )
	const static UINT m_uiIncrementSize = 1000;

	//used to keep track of the size of all the buffers in use
	UINT m_uiNumVerts;
	UINT m_uiVertsUsed;
	UINT m_uiNumIndices;
	UINT m_uiIndicesUsed;
	UINT m_uiAnimNumVerts;
	UINT m_uiAnimVertsUsed;
	UINT m_uiAnimNumIndices;
	UINT m_uiAnimIndicesUsed;

	//vector to store all loaded meshs, also used to clear the memory in shutdown
	std::vector<loadedMeshData> m_vpLoadedNodes;
	std::vector<RenderNode*> m_vpRenderNodeCopies;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IncreaseAnimVerts(): Increase the size of the animation vertex buffer using the num verts and stride size
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IncreaseAnimVerts( UINT numVerts, UINT _strideSize );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddAnimIndices(): Add indices to the animation index buffer on the GPU
	//                       
	// Returns: UINT - start index into the index buffer for the indices passed in
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UINT AddAnimIndices( const void* indices, UINT numIndices );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IncreaseAnimIndices(): Increase the size of the animation index buffer on the GPU
	//                       
	// Returns: voids
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IncreaseAnimIndices( UINT numIndices );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IncreaseVerts(): Increase the size of the vertex buffer
	//                       
	// Returns: voids
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IncreaseVerts(UINT numVerts, UINT _strideSize);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddIndices(): Add indices to the index buffer
	//                       
	// Returns: UINT - start index for the mesh being loaded
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UINT AddIndices(const void* indices, UINT numIndices);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IncreaseIndices(): Increase the size of the index buffer
	//                       
	// Returns: voids
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IncreaseIndices(UINT numIndices);
	
	//Our one and only instance of the MeshManager
	static MeshManager* m_pInstance;

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InitMeshManager(): Defaults all values, starts up with the render engine
	//                       
	// Returns: voids
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitMeshManager(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddVerts(): Adds all vertex and index data to the buffers on the GPU
	//                       
	// Returns: RenderHandle - stores all information the renderer needs to render the mesh
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RenderHandle AddVerts( const void* verts, UINT _numVerts, UINT _strideSize,	const void* indices, UINT numIndices );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AddAnimVerts(): Adds all vertex and index data to the animation buffers on the GPU
	//                       
	// Returns: RenderHandle - stores all information the renderer needs to render the mesh
	//                            TODO CHANGE TO A ANIMATION RENDER HANDLE - NOT CREATED YET
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RenderHandle AddAnimVerts( const void* verts, UINT _numVerts, UINT _strideSize,	const void* indices, UINT numIndices );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ShutdownMeshManager(): Shuts down and cleans up all memory allocated by the mesh manager
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ShutdownMeshManager(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadAnimatedMesh(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadAnimatedMesh(const char* szInfiniteHPMeshName, D3DXMATRIX transformMat );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadAnimatedMeshFromFile(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadAnimatedMeshFromFile( const char* InfiniteHPMeshData, int& pos, D3DXMATRIX transformMat);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadMesh(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadMesh(const char* szInfiniteHPMeshName, D3DXMATRIX transformMat );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadMeshFromFile(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadMeshFromFile( const char* InfiniteHPMeshData, int& pos, D3DXMATRIX transformMat);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadMeshFromFile(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int LoadMeshFromFile( const char* InfiniteHPMeshData, int& pos);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadCommonMeshData(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadCommonMeshData( const char* InfiniteHPMeshData, BaseMesh **ToFill, int& pos, int texHandles[MAX_TEXTURES], unsigned int& vertAttrib);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadCollisionEnvironment(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadCollisionEnvironment(const char* szInfiniteHPLevelName);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadEnvironment(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadEnvironment(const char* szInfiniteHPLevelName);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadEnvironmentTraps(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadEnvironmentTraps( const char* szInfiniteHPLevelTrapFile );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadEnvironmentMoveableObjects(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadEnvironmentMoveableObjects( const char* szInfiniteHPLevelObjectsFile );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// _LoadBoundingVolumes(): Loads all shaders needed for rendering
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Andy Madruga
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int _LoadBoundingVolumes( const char* InfiniteHPMeshData, BaseMesh** ToFill);

	bool AddNodeFlags( const char* szNodeName, unsigned int _bitFlags );
	bool AddNodeFlags( RenderNode* _pToFind, unsigned int _bitFlags );
	void ClearNodeFlags( const char* szNodeName, unsigned int _bitFlags );
	unsigned int* GetNodeBitFlags( const char* szNodeName );

        /////////////////////////////
	//  Accessors
	/////////////////////////////
	static MeshManager* GetInstance(void);
	inline IDirect3DVertexBuffer9* GetVertexBuffer(void) { return m_pSharedVertexBuffer; }
	inline IDirect3DIndexBuffer9* GetIndexBuffer(void) { return m_pSharedIndexBuffer; }
	inline IDirect3DVertexBuffer9* GetAnimVertexBuffer(void) { return m_pAnimationVertexBuffer; }
	inline IDirect3DIndexBuffer9* GetAnimIndexBuffer(void) { return m_pAnimationIndexBuffer; }
	inline UINT GetNumVertsUsed(void) { return m_uiVertsUsed; }
	inline UINT GetNumIndicesUsed(void) { return m_uiIndicesUsed; }
	RenderNode* GetRenderNode( const char* _meshName );
	RenderNode* GetRenderNodeCopy( const char* _meshName );
	RenderNode* GetRenderNodeMasterCopy( const char* _meshName, const char* _copyMeshName );
	void GetCollisionVolume( const char* _meshName );
    void GetCollisionVolume( RenderNode* node );
	int IsMeshAlreadyLoaded( const char* _meshName );

	int LoadEnvironmentMeshFromFile( const char* InfiniteHPMeshData, int& pos, Mesh** newMesh );
	void _LoadBoundingVolumesForEnvironment( Mesh* ToFill, int _meshIndex, D3DXMATRIX meshMatrix );

	void LoadHazard(const char* szInfiniteHPMeshName,  D3DXMATRIX transformMat );
	int LoadHazardMeshFromFile( const char* InfiniteHPMeshData, Mesh** newMesh );
	void _GenerateVolumesForHazards( Mesh* ToFill, D3DXMATRIX meshMatrix );


};









#endif //MESHMANAGER_H_