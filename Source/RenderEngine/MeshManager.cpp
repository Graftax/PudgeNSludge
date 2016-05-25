//////////////////////////////////////////////////////////////////////////////////////
// Filename:            MeshManager.cpp
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all loaded meshes
//////////////////////////////////////////////////////////////////////////////////////
#include "MeshManager.h"
#include "RenderNode.h"
#include "TextureManager.h"
#include "../Utilities/CollisionLib/CollisionVolume.h"
#include "../AnimationLib/AnimationIncludes.h"
#include <sstream>

///////////////REMOVE THIS LATER
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../LevelManager/CLevelManager.h"


#include <fstream>
using namespace std;

#include "Mesh.h"
#include "PackUtilities.h"

#ifdef _DEBUG 
	#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__) 
#else 
	#define DEBUG_CLIENTBLOCK 
#endif 

//Initialize the instance to null
MeshManager* MeshManager::m_pInstance = nullptr;

//Defines used to load up assests from their folders
#define MESH_DIR "./Resources/Models/"
#define MESH_DIR_LEN strlen(MESH_DIR)
#define FILE_EXT ".InfiniteHPMesh"
#define FILE_EXT_LEN strlen(FILE_EXT)
#define TEXTURE_DIR "./Resources/Textures/"
#define TEXTURE_DIR_LEN strlen(TEXTURE_DIR)

MeshManager* MeshManager::GetInstance(void)
{
	//Singleton - Check if an instance has been created
	if(!m_pInstance)
	{
		//if not make a new instance and init that instance
		m_pInstance = new MeshManager;
		m_pInstance->InitMeshManager();
	}
	//return the current instance
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitMeshManager(): Defaults all values, starts up with the render engine
//                       
// Returns: voids
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::InitMeshManager(void)
{
	//Initialize all values to zero
	m_pSharedVertexBuffer = nullptr;
	m_pSharedIndexBuffer = nullptr;

	m_pAnimationVertexBuffer = nullptr;
	m_pAnimationIndexBuffer = nullptr;

	m_uiCurrentVertBuffOffset = 0u;
	m_uiAnimCurrentVertBuffOffset = 0u;

	m_uiNumVerts = 0u;
	m_uiVertsUsed = 0u;
	m_uiNumIndices = 0u;
	m_uiIndicesUsed = 0u;

	m_uiAnimNumVerts = 0u;
	m_uiAnimVertsUsed = 0u;
	m_uiAnimNumIndices = 0u;
	m_uiAnimIndicesUsed = 0u;

	LOG(LOG_INFO, "MeshManager::InitMeshManager(void)", "MeshManager was initialized!!");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ShutdownMeshManager(): Shuts down and cleans up all memory allocated by the mesh manager
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void MeshManager::ShutdownMeshManager(void)
 { 
	 //Loop through all render nodes that have been passed out
	 for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	 {
		 //if the node has been allocated
		 if( m_vpLoadedNodes[i].m_pNode != nullptr )
		 {
			 //m_vpLoadedNodes[i].m_pNode->ClearExtraData();

			 //Get the pointer to the node
			 RenderNode* toDelete = m_vpLoadedNodes[i].m_pNode;
			 //Delete the node
			 delete toDelete;
			 //Set the node to null
			 toDelete = nullptr;
			 //if the current index has a collision volume delete it, FIXED MAJOR MEMORY LEAK
			 if( m_vpLoadedNodes[i].m_pColVolume != nullptr )
				 delete m_vpLoadedNodes[i].m_pColVolume;
		 }
		 if( m_vpLoadedNodes[i].m_unBitFlags != nullptr )
		 {
			 unsigned int* toDelete = m_vpLoadedNodes[i].m_unBitFlags;

			 delete toDelete;

			 toDelete = nullptr;
		 }
	 }
	 for( unsigned int i = 0u; i < m_vpRenderNodeCopies.size(); ++i )
	 {
		 if( m_vpRenderNodeCopies[i] != nullptr )
		 {
			 //Get the pointer to the node
			 RenderNode* toDelete = m_vpRenderNodeCopies[i];
			 //Delete the node
			 delete toDelete;
			 //Set the node to null
			 toDelete = nullptr;
		 }
	 }
	 //RELEASE ALL DRX COM OBJECTS
	 SAFE_RELEASE( m_pAnimationIndexBuffer );
	 SAFE_RELEASE( m_pAnimationVertexBuffer );
	 SAFE_RELEASE( m_pSharedVertexBuffer );
	 SAFE_RELEASE( m_pSharedIndexBuffer );
	 m_pAnimationIndexBuffer  = nullptr; m_pAnimationVertexBuffer  = nullptr; m_pSharedVertexBuffer  = nullptr; m_pSharedIndexBuffer = nullptr;
	 //Delete the instance of the class if it has been created
	 if( m_pInstance )
	 {
		delete m_pInstance; 
		m_pInstance = nullptr;
	 }
 }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddVerts(): Adds all vertex and index data to the buffers on the GPU
//                       
// Returns: RenderHandle - stores all information the renderer needs to render the mesh
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderHandle MeshManager::AddVerts( const void* verts, UINT _numVerts, UINT _strideSize, const void* indices, UINT numIndices )
{
	//Store the current vert location, used for locking the buffer
	UINT vertLocation = m_uiVertsUsed;
	//Make a renderHandle
	RenderHandle renderInformation;
	//Fill out the correct information needed by the render engine for rendering
	//Offset in bytes to within the vertex buffer
	renderInformation.m_uiBufferOffset = m_uiCurrentVertBuffOffset;
	//The start index into the index buffer, used for DRAWINDEXEDPRIM
	renderInformation.m_uiStartIndex = AddIndices( indices, numIndices );
	//Number of verts to draw the passed in mesh
	renderInformation.m_uiVertices = _numVerts;
	//The stride in bytes for the vertex format, FIXED ISSUE WITH DIFFERENT STRIDE SIZES
	renderInformation.m_uiStride = _strideSize;
	//Number of polygons contained within the mesh
	renderInformation.m_uiPrimCount = numIndices / 3;	
	//Incrememnt the number of total verts already loaded in the buffer
	m_uiVertsUsed += _numVerts;

	//Check to see if there is enough room in the buffer for the mesh
	if( m_uiVertsUsed > m_uiNumVerts )
	{
		//if not allocate more space into the buffer
		IncreaseVerts( m_uiVertsUsed, _strideSize );
	}
	//pointer used to get the address of the buffer on the GPU
	void* _buffer;
	//Lock the vertex buffer to add the vertex data
	m_pSharedVertexBuffer->Lock( vertLocation * _strideSize, _numVerts * _strideSize, &_buffer, 0 );
	//copy the passed in verts to the buffer on the GPU
	memcpy( _buffer, verts, _numVerts * _strideSize );
	//unlock access to the CPU
	m_pSharedVertexBuffer->Unlock();

	//UPDATED BUFFER OFFSET
	m_uiCurrentVertBuffOffset += _numVerts * _strideSize;
	//return the render information needed by the render engine
	return renderInformation;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddAnimVerts(): Adds all vertex and index data to the animation buffers on the GPU
//                       
// Returns: RenderHandle - stores all information the renderer needs to render the mesh
//                            TODO CHANGE TO A ANIMATION RENDER HANDLE - NOT CREATED YET
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderHandle MeshManager::AddAnimVerts( const void* verts, UINT _numVerts, UINT _strideSize, const void* indices, UINT numIndices )
{
	//Store the current vert location, used for locking the buffer
	UINT vertLocation = m_uiAnimVertsUsed;
	//Make a renderHandle
	RenderHandle renderInformation;
	//Fill out the correct information needed by the render engine for rendering
	//Offset in bytes to within the vertex buffer
	renderInformation.m_uiBufferOffset = m_uiAnimCurrentVertBuffOffset;
	//The start index into the index buffer, used for DRAWINDEXEDPRIM
	renderInformation.m_uiStartIndex = AddAnimIndices( indices, numIndices );
	//Number of verts to draw the passed in mesh
	renderInformation.m_uiVertices = _numVerts;
	//The stride in bytes for the vertex format, FIXED ISSUE WITH DIFFERENT STRIDE SIZES
	renderInformation.m_uiStride = _strideSize;
	//Number of polygons contained within the mesh
	renderInformation.m_uiPrimCount = numIndices / 3;	
	//Incrememnt the number of total verts already loaded in the buffer
	m_uiAnimVertsUsed += _numVerts;

	//Check to see if there is enough room in the buffer for the mesh
	if( m_uiAnimVertsUsed > m_uiAnimNumVerts )
	{
		//if not allocate more space into the buffer
		IncreaseAnimVerts( m_uiAnimVertsUsed, _strideSize );
	}
	//pointer used to get the address of the buffer on the GPU
	void* _buffer;
	//Lock the vertex buffer to add the vertex data
	m_pAnimationVertexBuffer->Lock( vertLocation * _strideSize, _numVerts * _strideSize, &_buffer, 0 );
	//copy the passed in verts to the buffer on the GPU
	memcpy( _buffer, verts, _numVerts * _strideSize );
	//unlock access to the CPU
	m_pAnimationVertexBuffer->Unlock();

	//UPDATED BUFFER OFFSET
	m_uiAnimCurrentVertBuffOffset += _numVerts * _strideSize;
	//return the render information needed by the render engine
	return renderInformation;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IncreaseVerts(): Increase the size of the vertex buffer
//                       
// Returns: voids
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::IncreaseVerts(UINT numVerts, UINT _strideSize)
{
	//Increase the vert buffer size by the nearest greater thousands
	UINT increments = (numVerts / m_uiIncrementSize) * m_uiIncrementSize + m_uiIncrementSize;
	//Increase the amount the vertex buffer can hold
	m_uiNumVerts += increments;
	//Save the location of the old vertex buffer
	IDirect3DVertexBuffer9 *pOldVertexBuff = m_pSharedVertexBuffer;

	//m_pSharedVertexBuffer = RenderEngine::InitVertexBuffer( /*&m_pSharedVertexBuffer,*/ m_uiNumVerts*_strideSize ); DEPRECATED
	//We are a friend so make a new vertex buffer of the newly created size
	RenderEngine::m_pD3Dev->CreateVertexBuffer( m_uiNumVerts * _strideSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pSharedVertexBuffer, 0 );

	//Check to see if we have even created a buffer yet
	if(pOldVertexBuff)
	{
		//If we have, lock the old buffer
		void *oldBuffer;
		pOldVertexBuff->Lock(0,	(m_uiNumVerts - increments)*_strideSize, &oldBuffer, 0);		
		//Lock the new buffer
		void *buffer;
		m_pSharedVertexBuffer->Lock(0, (m_uiNumVerts - increments)*_strideSize, &buffer, 0);	
		//Copy the data from the old buffer to the new bigger buffer
		memcpy(buffer, oldBuffer, (m_uiNumVerts - increments)*_strideSize);
		//Unlock both buffers
		pOldVertexBuff->Unlock();
		m_pSharedVertexBuffer->Unlock();
		//Release the old buffer
		pOldVertexBuff->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IncreaseIndices(): Increase the size of the index buffer
//                       
// Returns: voids
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::IncreaseIndices(UINT numIndices)
{
	//Increase our amount of indices by the nearest highest thousands
	UINT increments = (numIndices / m_uiIncrementSize) * m_uiIncrementSize + m_uiIncrementSize;
	//pointer to be used to the old buffer
	IDirect3DIndexBuffer9* oldBuff = nullptr;
	//If there was no index buffer create one
	if( m_uiNumIndices != 0 )
		//oldBuff = RenderEngine::InitIndexBuffer( /*&oldBuff,*/ m_uiNumIndices * sizeof(DWORD) ); DEPRECATED
		RenderEngine::m_pD3Dev->CreateIndexBuffer( m_uiNumIndices * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &oldBuff, 0 );
	//Save the location of the current buffer
	oldBuff = m_pSharedIndexBuffer;
	//Save the original number of indices 
	UINT originalNumIndices = m_uiNumIndices;
	//Increase the amount of indices that are available in the buffer
	m_uiNumIndices += increments;

	//m_pSharedIndexBuffer = RenderEngine::InitIndexBuffer( /*&m_pSharedIndexBuffer,*/ m_uiNumIndices * sizeof(DWORD) ); DEPRECATED
	//Create a new bigger and better buffer! :D
	RenderEngine::m_pD3Dev->CreateIndexBuffer( m_uiNumIndices * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pSharedIndexBuffer, 0 );

	//Just incase the render engine is dead
	if( oldBuff == nullptr )
	{
		return;
	}
	//Pointers used to get access to the index buffers on the GPU
	void* oldBuffFrom;
	void* newBuff;
	//lock the memory of the old buffer
	oldBuff->Lock( 0, originalNumIndices * sizeof(DWORD), &oldBuffFrom, 0 );
	//lock the new buffer
	m_pSharedIndexBuffer->Lock( 0, m_uiNumIndices * sizeof(DWORD), &newBuff, 0 );
	{
		//copy the already allocated indices over
		memcpy( newBuff, oldBuffFrom, originalNumIndices * sizeof(DWORD));
	}
	//unlock the buffers
	m_pSharedIndexBuffer->Unlock();
	oldBuff->Unlock();
	//release the old buffers memory
	oldBuff->Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddIndices(): Add indices to the index buffer
//                       
// Returns: UINT - start index for the mesh being loaded
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT MeshManager::AddIndices(const void* indices, UINT numIndices)
{
	//Save the original amound of indices being used
	UINT originalSize = m_uiIndicesUsed;
	//Increase the amount of indices being used
	m_uiIndicesUsed += numIndices;
	//check to see if there is enough space for the amount being added
	if( m_uiIndicesUsed > m_uiNumIndices )
	{
		//if not make more room!
		IncreaseIndices( m_uiIndicesUsed );
	}		
	//pointer for access to the CPU memory block
	void* pBuffer;
	//lock the index buffer
	m_pSharedIndexBuffer->Lock( originalSize * sizeof(DWORD), numIndices * sizeof(DWORD), &pBuffer, 0 );
	{
		//copy the new data in
		memcpy( pBuffer, indices, numIndices * sizeof(DWORD));
	}
	//unlock the buffer
	m_pSharedIndexBuffer->Unlock();
	//return the start index for the indices added
	return originalSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IncreaseAnimVerts(): Increase the size of the animation vertex buffer using the num verts and stride size
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::IncreaseAnimVerts( UINT numVerts, UINT _strideSize )
{
	//Increase the vert buffer size by the nearest greater thousands
	UINT increments = (numVerts / m_uiIncrementSize) * m_uiIncrementSize + m_uiIncrementSize;
	//Increase the amount the vertex buffer can hold
	m_uiAnimNumVerts += increments;
	//Save the location of the old vertex buffer
	IDirect3DVertexBuffer9 *pOldVertexBuff = m_pAnimationVertexBuffer;

	//m_pSharedVertexBuffer = RenderEngine::InitVertexBuffer( /*&m_pSharedVertexBuffer,*/ m_uiNumVerts*_strideSize ); DEPRECATED
	//We are a friend so make a new vertex buffer of the newly created size
	RenderEngine::m_pD3Dev->CreateVertexBuffer( m_uiAnimNumVerts * _strideSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pAnimationVertexBuffer, 0 );
	//Check to see if we have even created a buffer yet
	if(pOldVertexBuff)
	{
		//If we have, lock the old buffer
		void *oldBuffer;
		pOldVertexBuff->Lock(0,	(m_uiAnimNumVerts - increments)*_strideSize, &oldBuffer, 0);		
		//Lock the new buffer
		void *buffer;
		m_pAnimationVertexBuffer->Lock(0, (m_uiAnimNumVerts - increments)*_strideSize, &buffer, 0);	
		//Copy the data from the old buffer to the new bigger buffer
		memcpy(buffer, oldBuffer, (m_uiAnimNumVerts - increments)*_strideSize);
		//Unlock both buffers
		pOldVertexBuff->Unlock();
		m_pAnimationVertexBuffer->Unlock();
		//Release the old buffer
		pOldVertexBuff->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddAnimIndices(): Add indices to the animation index buffer on the GPU
//                       
// Returns: UINT - start index into the index buffer for the indices passed in
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT MeshManager::AddAnimIndices( const void* indices, UINT numIndices )
{
	//Save the original amound of indices being used
	UINT originalSize = m_uiAnimIndicesUsed;
	//Increase the amount of indices being used
	m_uiAnimIndicesUsed += numIndices;
	//check to see if there is enough space for the amount being added
	if( m_uiAnimIndicesUsed > m_uiAnimNumIndices )
	{
		//if not make more room!
		IncreaseAnimIndices( m_uiAnimIndicesUsed );
	}		
	//pointer for access to the CPU memory block
	void* pBuffer;
	//lock the index buffer
	m_pAnimationIndexBuffer->Lock( originalSize * sizeof(DWORD), numIndices * sizeof(DWORD), &pBuffer, 0 );
	{
		//copy the new data in
		memcpy( pBuffer, indices, numIndices * sizeof(DWORD));
	}
	//unlock the buffer
	m_pAnimationIndexBuffer->Unlock();
	//return the start index for the indices added
	return originalSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IncreaseAnimIndices(): Increase the size of the animation index buffer on the GPU
//                       
// Returns: voids
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshManager::IncreaseAnimIndices( UINT numIndices )
{
	//Increase our amount of indices by the nearest highest thousands
	UINT increments = (numIndices / m_uiIncrementSize) * m_uiIncrementSize + m_uiIncrementSize;
	//pointer to be used to the old buffer
	IDirect3DIndexBuffer9* oldBuff = nullptr;
	//If there was no index buffer create one
	if( m_uiAnimNumIndices != 0 )
		//oldBuff = RenderEngine::InitIndexBuffer( /*&oldBuff,*/ m_uiNumIndices * sizeof(DWORD) ); DEPRECATED
		RenderEngine::m_pD3Dev->CreateIndexBuffer( m_uiAnimNumIndices * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &oldBuff, 0 );
	//Save the location of the current buffer
	oldBuff = m_pAnimationIndexBuffer;
	//Save the original number of indices
	UINT originalNumIndices = m_uiAnimNumIndices;
	//Increase the amount of indices that are available in the buffer
	m_uiAnimNumIndices += increments;

	//m_pSharedIndexBuffer = RenderEngine::InitIndexBuffer( /*&m_pSharedIndexBuffer,*/ m_uiNumIndices * sizeof(DWORD) ); DEPRECATED
	//Create a new bigger and better buffer! :D

	RenderEngine::m_pD3Dev->CreateIndexBuffer( m_uiAnimNumIndices * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pAnimationIndexBuffer, 0 );

	//Just incase the render engine is dead
	if( oldBuff == nullptr )
	{
		return;
	}
	//Pointers used to get access to the index buffers on the GPU
	void* oldBuffFrom;
	void* newBuff;
	//lock the memory of the old buffer
	oldBuff->Lock( 0, originalNumIndices * sizeof(DWORD), &oldBuffFrom, 0 );
	//lock the new buffer
	m_pAnimationIndexBuffer->Lock( 0, m_uiAnimNumIndices * sizeof(DWORD), &newBuff, 0 );
	{
		//copy the already allocated indices over
		memcpy( newBuff, oldBuffFrom, originalNumIndices * sizeof(DWORD));
	}
	//unlock the buffers
	m_pAnimationIndexBuffer->Unlock();
	oldBuff->Unlock();
	//release the old buffers memory
	oldBuff->Release();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
///ACCESSORS FOR LOADED INFORMATION
int MeshManager::IsMeshAlreadyLoaded( const char* _meshName )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded send back the index it was found at
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), _meshName ) == 0 )
			return i;
	}
	//if it was not found return fail value
	return -1;
}

RenderNode* MeshManager::GetRenderNode( const char* _meshName )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), _meshName ) == 0 )
			return m_vpLoadedNodes[i].m_pNode;
	}
	//if it was not found return the fail value
	return nullptr;
}

RenderNode* MeshManager::GetRenderNodeCopy( const char* _meshName )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), _meshName ) == 0 )
		{
			RenderNode* copy = new RenderNode( m_vpLoadedNodes[i].m_pNode );
			m_vpRenderNodeCopies.push_back( copy );
			return copy;
		}
	}
	//if it was not found return the fail value
	return nullptr;
}

RenderNode* MeshManager::GetRenderNodeMasterCopy( const char* _meshName, const char* _copyMeshName )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), _meshName ) == 0 )
		{
			RenderNode* copy = new RenderNode( m_vpLoadedNodes[i].m_pNode );
			loadedMeshData toAdd;
			toAdd.m_pNode = copy;
			toAdd.m_sMeshName = _copyMeshName;
			copy->m_unBitFlags = toAdd.m_unBitFlags;
			copy->m_pOwner = copy;
			m_vpLoadedNodes.push_back( toAdd );
			return copy;
		}
	}
	//if it was not found return the fail value
	return nullptr;
}

bool MeshManager::AddNodeFlags( const char* szNodeName, unsigned int _bitFlags )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), szNodeName ) == 0 )
		{
			*m_vpLoadedNodes[i].m_unBitFlags |= _bitFlags;
			return true;
		}
	}
	//if it was not found return the fail value
	return false;
}

bool MeshManager::AddNodeFlags( RenderNode* _pToFind, unsigned int _bitFlags )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( m_vpLoadedNodes[i].m_pNode == _pToFind )
		{
			*m_vpLoadedNodes[i].m_unBitFlags |= _bitFlags;
			return true;
		}
	}
	//if it was not found return the fail value
	return false;
}

void MeshManager::ClearNodeFlags( const char* szNodeName, unsigned int _bitFlags )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), szNodeName ) == 0 )
		{
			//*m_vpLoadedNodes[i].m_unBitFlags ^= _bitFlags;
			*m_vpLoadedNodes[i].m_unBitFlags &= ~(_bitFlags);
		}
	}
}

unsigned int* MeshManager::GetNodeBitFlags( const char* szNodeName )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the mesh is already loaded return the pointer to the render information
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), szNodeName ) == 0 )
		{
			return m_vpLoadedNodes[i].m_unBitFlags;
		}
	}
	//if it was not found return the fail value
	return nullptr;
}

CCollisionVolume* MeshManager::GetCollisionVolume( const char* _meshName )
{
	//Loop through the already loaded nodes
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		//if the meshes collision volume is already loaded send it back
		if( _stricmp( m_vpLoadedNodes[i].m_sMeshName.c_str(), _meshName ) == 0 )
			return m_vpLoadedNodes[i].m_pColVolume;
	}
	//if it was not found return the fail value
	return nullptr;
}

CCollisionVolume* MeshManager::GetCollisionVolume( RenderNode* node )
{
	for( unsigned int i = 0u; i < m_vpLoadedNodes.size(); ++i )
	{
		if( m_vpLoadedNodes[i].m_pNode == node )
			return m_vpLoadedNodes[i].m_pColVolume;
	}

	return NULL;
}

void MeshManager::LoadMesh(const char* szInfiniteHPMeshName, D3DXMATRIX transformMat )
{
	// Load Mesh from file.

	//if( GetInstance() == NULL )
		// Error Manager not yet initialized.
	//if( szInfiniteHPMeshName == NULL )
		// Error Mesh name passed is not valid.

	int len = (strlen(szInfiniteHPMeshName) + 1) + MESH_DIR_LEN + FILE_EXT_LEN;
	char* MeshFileName = new char[len];
	sprintf_s(MeshFileName, len, "%s%s%s", MESH_DIR, szInfiniteHPMeshName, FILE_EXT);

	// Declare the stream for read/writing.
	fstream MeshStream;
	// Open the file
	MeshStream.open( MeshFileName, ios_base::in | ios_base::binary);

	// If file is not open then directory/name was wrong.
	if( !MeshStream.is_open() )
	{
		// Log Error.
	}
	else
	{
		// The file was opened correctly so lets grab the data.
		MeshStream.seekg(0,ios_base::end);

		int filesize = (int)MeshStream.tellg();

		// Allocate enough data to hold the file in memory.
		char* fileData = new char[filesize];

		// Put the file data into the local data.
		MeshStream.seekg(0, ios_base::beg);
		MeshStream.read(fileData, filesize);

		int index = 0;
		LoadMeshFromFile(fileData, index, transformMat); 

		delete[] fileData;
	}

	delete MeshFileName;
	MeshStream.close();
}

void MeshManager::LoadMeshFromFile( const char* InfiniteHPMeshData, int& pos, D3DXMATRIX transformMat)
{
	//if( GetInstance() == NULL )
		// Log Error and return NULL.

	Mesh* newMesh = NULL;
	pos = 0;
	unsigned int vertSize, tricount, triSize, vertAttrib = 0;
	int textureHandles[MAX_TEXTURES] = {0};
	for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
	{
#if DEFERREDRENDER
		textureHandles[i] = 0;
#else
		textureHandles[i] = 0;
#endif
	}
	if( LoadCommonMeshData(InfiniteHPMeshData, (BaseMesh **)&newMesh, pos, textureHandles, vertAttrib) || newMesh == NULL)
		//return newMesh or handle;
		

	vertSize = sizeof(VERTEX_POSNORMTANUV) * newMesh->m_vVerts.size();
	memcpy( &newMesh->m_vVerts[0], &InfiniteHPMeshData[pos], vertSize);
	pos += vertSize;
	memcpy( &tricount, &InfiniteHPMeshData[pos], sizeof(tricount));
	pos += sizeof(tricount);

	// Resize the mesh triangles to the tri count.
	newMesh->m_vTris.resize(tricount);
	triSize = sizeof(Triangle) * tricount;
	memcpy( &newMesh->m_vTris[0], &InfiniteHPMeshData[pos], triSize);
	//for( unsigned int i = 0u; i < newMesh->m_vTris.size(); ++i )
	//{
	//	newMesh->m_vTris[i].Index0 += this->m_uiVertsUsed;
	//	newMesh->m_vTris[i].Index1 += this->m_uiVertsUsed;
	//	newMesh->m_vTris[i].Index2 += this->m_uiVertsUsed;
	//}
	pos += triSize;



	int nCheckResult = IsMeshAlreadyLoaded( newMesh->GetMeshName() );
	if(  nCheckResult == -1 )
	{
		RenderNode* node = new RenderNode;	
		loadedMeshData toVector;
		*toVector.m_unBitFlags = vertAttrib;
		node->m_unBitFlags = toVector.m_unBitFlags;
		node->Render = RenderEngine::DrawTexturedMesh;
		node->FillRenderData( (void*)&newMesh->m_vVerts[0], newMesh->m_vVerts.size(), sizeof(VERTEX_POSNORMTANUV), (void*)&newMesh->m_vTris[0], tricount * 3 );
		//node->renderData.m_vTransform.push_back(transformMat);
		node->renderData.m_d3dVertDecl = RenderEngine::GetPosNormTanUVDecl();
		for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
			node->renderData.m_uiTextureHandles[i] = textureHandles[i];
		RenderEngine::AddToObjectRenderSet( node, transformMat );
			
//#if DEFERREDRENDER
//		if( strcmp( newMesh->GetMeshName(), "GooPit" ) == 0 )
//		{
//			node->AddFlags( RE_TRANSPARENT );
//		}
//#endif
		toVector.m_pNode = node;
		toVector.m_sMeshName = newMesh->GetMeshName();

		m_vpLoadedNodes.push_back( toVector );

		pos += _LoadBoundingVolumes( &InfiniteHPMeshData[pos], (BaseMesh**)&newMesh);

		//static bool player = false;
		//if(!player)
		//{
		//	CPlayer::GetInstance()->SetRenderNodePointer( node );
		//	CPlayer::GetInstance()->m_mat = node->renderData.m_vTransform[0];
		//	player = true;
		//}
	}
	else
	{
		m_vpLoadedNodes[nCheckResult].m_pNode->renderData.m_vTransform.push_back(transformMat);
	}

	
	// Load Bounding Volumes

			// Read in a BVcount which is a unsigned char
			// If bvCount is a valid value...
				// read in bvType also a unsigned char
				// Read in appropriate data for
					// AABB - 0
					// Sphere - 1
					// Capsule - 2;
			// Do not forget to increment the pos in the data.

	// After bounding volume loaded load up matrix.
	delete newMesh;
	// Push back mesh onto list of "Loaded" Meshes.
	// return mesh or handle.
	
}

bool MeshManager::LoadCommonMeshData( const char* InfiniteHPMeshData, BaseMesh **ToFill, int& pos, int texHandles[MAX_TEXTURES], unsigned int& vertAttrib)
{
	if( ToFill == NULL )
		return false;

	char*meshName = NULL;
	unsigned int meshNameLen;

	// First get the mesh name
	memcpy(&meshNameLen, &InfiniteHPMeshData[pos], sizeof(meshNameLen));
	pos += sizeof(meshNameLen);
	meshName = new char[meshNameLen];
	memcpy(meshName, &InfiniteHPMeshData[pos], meshNameLen);
	pos += meshNameLen;
	//(*ToFill) = NULL;

	// Now try and see if this mesh exists.
	// This will differ between static and animated meshes, its type 
	// will be determined by the attribute flag.

	// Get the Vertex Attributes of this mesh
	unsigned int VertexAttributes;
	memcpy( &VertexAttributes, &InfiniteHPMeshData[pos], 4);
	pos += 4;

	vertAttrib = VertexAttributes;

	if( (*ToFill) == NULL && (VertexAttributes & RE_ANIMATED)) // If this is animated.
	{
		(*ToFill) = (BaseMesh*)new AnimatedMesh();
	}
	else if ((*ToFill) == NULL && !(VertexAttributes & RE_ANIMATED))
	{
		(*ToFill) = (BaseMesh *)new Mesh();
	}

	if( (*ToFill) == NULL)
		return false;

	// Copy name of mesh
	(*ToFill)->m_Name = new char[meshNameLen];
	memcpy((*ToFill)->m_Name, meshName, meshNameLen);
	// Remember to De-Allocate memory.
	delete[] meshName;

	unsigned char texCount, texType;
	unsigned int texNameLen, vertCount, uvCount;
	char* texName = NULL;

	string textureToLoad;
	// Load Textures
	memcpy( &texCount, &InfiniteHPMeshData[pos], sizeof(texCount));
	pos += sizeof(texCount);
	for( unsigned int i = 0; i < texCount; ++i)
	{
		memcpy( &texType, &InfiniteHPMeshData[pos], sizeof(texType));
		pos += sizeof(texType);
		memcpy( &texNameLen, &InfiniteHPMeshData[pos], sizeof(texNameLen) );
		pos += sizeof(texNameLen);
		texName = new char[texNameLen];
		memcpy( texName, &InfiniteHPMeshData[pos], texNameLen);
		pos += texNameLen;

		// Insert call here to load texture.
		textureToLoad = TEXTURE_DIR;
		textureToLoad += texName;
		texHandles[i] = TextureManager::GetInstance()->LoadTexture( textureToLoad.c_str() );
		// Delete memory allocated
		delete[] texName;
	}

	// Get vertex and UV data
	memcpy( &vertCount, &InfiniteHPMeshData[pos], sizeof(vertCount) );
	pos += sizeof(vertCount);
	memcpy( &uvCount, &InfiniteHPMeshData[pos], sizeof(uvCount) );
	pos += sizeof(uvCount);

	//if( uvCount > 1 )
		// Log error here.

	if( VertexAttributes & RE_ANIMATED )
		((AnimatedMesh *)(*ToFill))->GetVerts().resize(vertCount);
	else
		((Mesh *)(*ToFill))->GetVerts().resize(vertCount);

	return true;
}

bool MeshManager::LoadEnvironment(const char* szInfiniteHPLevelName)
{
	// Forward declare the vector of fileheaders, will be filled in later.
	vector<FileHeader> m_vFileHeaders;

	fstream IOPakStream;

	// Open up the File
	IOPakStream.open(szInfiniteHPLevelName, ios_base::in | ios_base::binary);

	// Make sure the file is open otherwise we failed!
	if( !IOPakStream.is_open() )
	{
		// Log error that file failed to open.

		return false;
	}
	else
	{
		MainHeader mainheader;
		IOPakStream.seekg(0, ios_base::beg);

		// Read in Main Header
		IOPakStream.read( (char*)&mainheader, sizeof(mainheader) );

		// Calculate Number of Files in the Pack File
		int NumberOfFiles = mainheader.nDirectoryLength / sizeof(FileHeader);

		// After the number of files are calculated, seek to the directory
		// offset and begin loading in files based on their data.
		IOPakStream.seekg(mainheader.nDirectoryOffSet, ios_base::beg);

		// Loop through each file storing the File headers.
		for( int fileIndex = 0; fileIndex < NumberOfFiles; fileIndex++ )
		{
			FileHeader tempHeader;
			IOPakStream.read((char*)&tempHeader, sizeof(FileHeader));
			// Add in the header to the list.
			m_vFileHeaders.push_back(tempHeader);
		}

		// Clear the fail bit in case end of file was read.
		IOPakStream.clear();

		for( int fileIndex = 0; fileIndex < NumberOfFiles; fileIndex++)
		{
			IOPakStream.seekg( m_vFileHeaders[fileIndex].nFileOffSet, ios_base::beg);

			// Allocate memory for file size then read it all in at once to be passed to load.
			char* fileData = new char[m_vFileHeaders[fileIndex].nFileLength];
			IOPakStream.read( fileData, m_vFileHeaders[fileIndex].nFileLength);

			IOPakStream.clear();

			// Can insert call to Factory here to load meshes...
					// For now I'll write the code.
			// Environment Load
			{
				int count = 0;

				// Allocate memory to load up mesh.
				Mesh* newMesh = NULL;

				// Read in all the mesh data
				int _meshIndex = LoadEnvironmentMeshFromFile(fileData, count, &newMesh);

				// Read in Number of Matrices
				unsigned int NumberOfMatrices = 0;
				memcpy(&NumberOfMatrices, &fileData[count], sizeof(NumberOfMatrices));
				count += sizeof(NumberOfMatrices);

				// Forward declare a temp matrix.
				D3DXMATRIX tempMatrix;
				

				for( unsigned int matrixIndex = 0; matrixIndex < NumberOfMatrices; matrixIndex++)
				{
					D3DXMatrixIdentity(&tempMatrix);
					memcpy( &tempMatrix, &fileData[count], sizeof(D3DXMATRIX) );
					m_vpLoadedNodes[_meshIndex].m_pNode->renderData.m_vTransform.push_back( tempMatrix );
					count += sizeof(D3DXMATRIX);

					_LoadBoundingVolumesForEnvironment( newMesh, _meshIndex, tempMatrix );
				}

				RenderEngine::AddToEnvironmentRenderSet( m_vpLoadedNodes[_meshIndex].m_pNode );
				m_vpLoadedNodes[_meshIndex].m_pNode->renderData.m_vCollisions.clear();

				delete newMesh;

			}
			delete[] fileData;

		}

		// If everything worked out fine, close file and return.
		//hello rueben
		IOPakStream.close();
	}

	return true;
}

bool MeshManager::LoadEnvironmentTraps( const char* szInfiniteHPLevelTrapFile, const char* szInfiniteHPLevelTrapDataFile, char _levelId )
{
	fstream TrapStream;
	TrapStream.open( szInfiniteHPLevelTrapFile, ios_base::in | ios_base::binary );

	// ope the Trap Data file that loads all the Trap Attributes
	fstream DataStream;
	DataStream.open( szInfiniteHPLevelTrapDataFile, ios_base::in | ios_base::binary );

	// If the Trap file and Data file is open
	if( TrapStream.is_open() &&  DataStream.is_open())
	{
		std::unordered_map< unsigned int, std::vector <IBaseObject*> > mpRecievers;
		std::unordered_map< unsigned int, std::vector <IComponent*> > mpSenders;

		// Read in Number of Traps.
		int NumberOfTraps;
		TrapStream.read( (char*)&NumberOfTraps, sizeof(NumberOfTraps) );

		// read the number of data Trps
		int DataTraps;
		DataStream.read( (char*)&DataTraps, sizeof(DataTraps) );

		// make sure the number of Traps and the number of Trap attributes match.

		// Loop through each Trap
		for( int i = 0; i < NumberOfTraps; i++)
		{
			unsigned char TrapID;
			D3DXMATRIX _worldMatrix;
			//unsigned char TrapChannel;

			// Read in the Trap ID - char.
			TrapStream.read( (char*)&TrapID, sizeof(TrapID) );

			int DataID = -1;
			if( TrapID < 14 )
			{
				// read in the data trap id
				DataStream.read( (char*)&DataID, sizeof(DataID) );
			}

			// Read in the World Matrix - D3DXMATRIX.
			TrapStream.read( (char*)&_worldMatrix, sizeof(_worldMatrix) );

			// After the trap data is read in,
			// Create the Object using the Object Factory.
			// Trap Values are the Following:
					// 0 - Steam Pipe
					// 1 - Spawn Pipe
					// 2 - Bridge
					// 3 - Breakable Wall
					// 4 - OutLet
					// 5 - Crusher
					// 6 - Goo Pit
					// 7 - Intake Pipe
					// 8 - Water Pool
					// 9 - Crusher Base
					// 10 - Door Button
					// 11 - Box Button
					// 12 - Box Spawn
					// 13 - Door
					// 14 - Boxes
					// 15 - Conveyor Belt
					// 16 - Claw
					// 17 - Fan

			int Channel = -1;
			if( TrapID < 14 )
			{
				DataStream.read( (char*)&Channel, sizeof(Channel) );
			}

			// make sure the trap id and data id are the same
			if (TrapID != DataID)
			{
				int x = 0;
			}

			switch( TrapID )
			{
				// Steam Pipe
			case 0:
				{
					IBaseObject* SteamPipe = CObjectManager::GetInstance()->CreateObject( OBJ_PIPE_STEAM , _worldMatrix, _levelId );
					CComponent_SteamPipeCollision* SteamCollision = (CComponent_SteamPipeCollision*)SteamPipe->GetComponent(ECOMP_COLLISION);

					// load in the steam pipe attribut data
					bool constant;
					int length;
					double duration;
					double freq;
					int direction;

					// load if the object is constant
					DataStream.read( (char*)&constant, sizeof(constant) );
					// load the length of the objects collision volume in pudge units
					DataStream.read( (char*)&length, sizeof(length) );
					// load the duration, how long it will stay on
					DataStream.read( (char*)&duration, sizeof(duration) );
					// load the frequency, how offten it will go (once per loaded seconds)
					DataStream.read( (char*)&freq, sizeof(freq) );
					// load the direction of the object
					DataStream.read( (char*)&direction, sizeof(direction) );
					// load what channel the object is on

					SteamCollision->SetConstant (constant);
					SteamCollision->SetChannel (Channel);
					SteamCollision->SetDuration ((float)duration);
					SteamCollision->SetFrequency ((float)freq);
					SteamCollision->SetLength (length);
					SteamCollision->BuildSteamAABB (length, direction);

					if (Channel != -1)
					{
						mpRecievers[Channel].push_back ( SteamPipe );
					}
				}
				break;
				// Spawn Pipe
			case 1:
				{
					IBaseObject* SpawnPipe = CObjectManager::GetInstance()->CreateObject( OBJ_PIPE_SPAWN , _worldMatrix, _levelId );
					CComponent_SpawnPipeCollision* SpawnCollision = (CComponent_SpawnPipeCollision*)SpawnPipe->GetComponent(ECOMP_COLLISION);

					bool corkable;
					int maxspawns;
					double freq;
					int direction;

					// load if the object is corkable
					DataStream.read( (char*)&corkable, sizeof(corkable) );
					// load the max spawns for this object
					DataStream.read( (char*)&maxspawns, sizeof(maxspawns) );
					// load the frequency, how offten it will go (once per loaded seconds)
					DataStream.read( (char*)&freq, sizeof(freq) );
					// load the direction of the object
					DataStream.read( (char*)&direction, sizeof(direction) );

					//SpawnCollision->SetCorkable (corkable);
					SpawnCollision->SetMaxSpawns (maxspawns);
					SpawnCollision->SetFrequency ((float)freq);
					SpawnCollision->BuildCorkableAABB (direction);
				}
				break;
				// Bridge
			case 2:
				{
					IBaseObject* Bridge = CObjectManager::GetInstance()->CreateObject( OBJ_BRIDGE, _worldMatrix, _levelId );
					CComponent_BridgeCollision* BridgeCollision = (CComponent_BridgeCollision*)Bridge->GetComponent(ECOMP_COLLISION);

					int Quadrant;
					bool RotateClockwise;

					// load the direction of the object
					DataStream.read( (char*)&Quadrant, sizeof(Quadrant) );
					// load if the object is on or off
					DataStream.read( (char*)&RotateClockwise, sizeof(RotateClockwise) );

					BridgeCollision->SetChannel (Channel);
					//BridgeCollision->SetQuadrant ( Quadrant );
					//BridgeCollision->SetRotateDirection ( RotateClockwise );

					if (Channel != -1)
					{
						mpRecievers[Channel].push_back ( Bridge );
					}
				}
				break;
				// Breakable Wall
			case 3:
				{
					IBaseObject* BreakableWall = CObjectManager::GetInstance()->CreateObject( OBJ_WALL_BREAKABLE, _worldMatrix, _levelId );
					CComponent_BreakableWallCollision* BreakableWallCollision = (CComponent_BreakableWallCollision*)BreakableWall->GetComponent(ECOMP_COLLISION);

					if( Channel > -1 )
					{
						mpSenders[Channel].push_back ( BreakableWallCollision );
						mpRecievers[Channel].push_back ( BreakableWall );
					}
				}
				break;
				// OutLet
			case 4:
				{
					IBaseObject* OutLet = CObjectManager::GetInstance()->CreateObject( OBJ_OUTLET, _worldMatrix, _levelId );
					CComponent_OutletCollision* OutLetCollision = (CComponent_OutletCollision*)OutLet->GetComponent(ECOMP_COLLISION);

					int direction;

					// load the direction of the object
					DataStream.read( (char*)&direction, sizeof(direction) );

					OutLetCollision->SetChannel ( Channel );
					OutLetCollision->SetDirection( direction );
					// Call the function to build the Outlets AABB.
					OutLetCollision->BuildOutletAABB();

					if( Channel > -1 )
					{
						mpSenders[Channel].push_back ( OutLetCollision );
					}
				}
				break;
				// Crusher
			case 5:
				{
					IBaseObject* Crusher = CObjectManager::GetInstance()->CreateObject( OBJ_CRUSHER, _worldMatrix, _levelId );
					CComponent_CrusherCollision* CrusherCollision = (CComponent_CrusherCollision*)Crusher->GetComponent(ECOMP_COLLISION);

					int length;
					double duration;
					double freq;
					int direction;

					// load the length of the objects collision volume in pudge units
					DataStream.read( (char*)&length, sizeof(length) );
					// load the duration, how long it will stay on
					DataStream.read( (char*)&duration, sizeof(duration) );
					// load the frequency, how offten it will go (once per loaded seconds)
					DataStream.read( (char*)&freq, sizeof(freq) );
					// load the direction of the object
					DataStream.read( (char*)&direction, sizeof(direction) );

					CrusherCollision->SetCrusherLength (length);
					CrusherCollision->SetCrusherDuration ((float)duration);
					CrusherCollision->SetCrusherFrequency ((float)freq);
					CrusherCollision->SetCrusherDirection (direction);

					CrusherCollision->BuildCrusherAABB();

				}
				break;
				// Goo Pit
			case 6:
				{
					IBaseObject* GooPit = CObjectManager::GetInstance()->CreateObject( OBJ_PIT, _worldMatrix, _levelId );
					CComponent_GooPitCollision* GooPitCollision = (CComponent_GooPitCollision*)GooPit->GetComponent(ECOMP_COLLISION);

					double damage;

					// load the damage for the object
					DataStream.read( (char*)&damage, sizeof(damage) );

					GooPitCollision->SetDamageAmount ((float)damage);
				}
				break;
				// IntakePipe
			case 7:
				{
					IBaseObject* IntakePipe = CObjectManager::GetInstance()->CreateObject(OBJ_PIPE_INTAKE, _worldMatrix, _levelId );
					CComponent_IntakePipeCollision* IntakePipeCollision = (CComponent_IntakePipeCollision*)IntakePipe->GetComponent(ECOMP_COLLISION);

					bool corkable;
					int direction;
					int FlowNameLength;
					char* FlowName = NULL;

					// load if the object is corkable
					DataStream.read( (char*)&corkable, sizeof(corkable) );
					// load the direction of the object
					DataStream.read( (char*)&direction, sizeof(direction) );
					// load in the flow pipes string length
					DataStream.read( (char*)&FlowNameLength, sizeof(FlowNameLength) );

					IntakePipeCollision->SetChannel (Channel);
					IntakePipeCollision->SetCorkable (corkable);
					IntakePipeCollision->SetDirection( direction );
					IntakePipeCollision->BuildCorkableAABB ();
					string flowName = "";
					if (FlowNameLength > 0)
					{
						FlowName = new char[FlowNameLength + 1];
						// load in the string
						DataStream.read( FlowName, sizeof(FlowName[0]) * FlowNameLength );
						FlowName[FlowNameLength] = '\0';
						//IntakePipeCollision->SetMeshName ();
						flowName = FlowName;
						delete FlowName;
					}

					IntakePipeCollision->SetFlowMeshName( flowName );

					if( Channel > -1 )
					{
						mpSenders[Channel].push_back ( IntakePipeCollision );
					}
				}
				break;
				// Water Pool
			case 8:
				{
					IBaseObject* WaterPool = CObjectManager::GetInstance()->CreateObject( OBJ_POOL, _worldMatrix, _levelId );
					CComponent_WaterPoolCollision* WaterCollision = (CComponent_WaterPoolCollision*)WaterPool->GetComponent(ECOMP_COLLISION);

					CComponent_Render* renderComp = (CComponent_Render*)WaterPool->GetComponent( ECOMP_RENDER );
					std::ostringstream oss;
					
					double healamount;
					int MeshNameLength;
					char* MeshName = NULL;

					// loads how much the object heals
					DataStream.read( (char*)&healamount, sizeof(healamount) );
					// load in the water pool mesh name string length
					DataStream.read( (char*)&MeshNameLength, sizeof(MeshNameLength) );

					// set the water pools heal amount
					WaterCollision->SetHealAmount ( (float)healamount );

					if (MeshNameLength > 0)
					{
						MeshName = new char[MeshNameLength + 1];
						// load in the string
						DataStream.read( MeshName, sizeof(MeshName[0]) * MeshNameLength );
						MeshName[MeshNameLength] = '\0';
						//WaterCollision->SetMeshName ();
						delete MeshName;
					}
					oss << Channel;
					string meshName = "WaterPool";
					meshName += oss.str();
					RenderNode* newRenderComp = GetRenderNodeCopy( meshName.c_str() );
					if( !newRenderComp )
						newRenderComp = GetRenderNodeMasterCopy( "WaterPool", meshName.c_str() );

					WaterCollision->SetMeshName( meshName );

					renderComp->SetRenderNode( newRenderComp );
					if( Channel > -1 )
					{
						mpSenders[Channel].push_back ( WaterCollision );
						mpRecievers[Channel].push_back ( WaterPool );
					}
				}
				break;
				// Crusher Base
			case 9:
				{
					IBaseObject* CrusherBase = CObjectManager::GetInstance()->CreateObject( OBJ_CRUSHERBASE, _worldMatrix, _levelId );
					//CComponent_CrusherBaseCollision* CrusherBaseCollision = (CComponent_CrusherBaseCollision*)CrusherBase->GetComponent(ECOMP_COLLISION);
				}
				break;
				// Door Button
			case 10:
				{
					IBaseObject* Button = CObjectManager::GetInstance()->CreateObject( OBJ_DOORBUTTON, _worldMatrix, _levelId );
					CComponent_ButtonCollision* ButtonCollision = (CComponent_ButtonCollision*)Button->GetComponent(ECOMP_COLLISION);

					int WireNameLength = 0;
					DataStream.read( (char*)&WireNameLength, sizeof(WireNameLength) );

					char* WireName = NULL;

					if (WireNameLength > 0)
					{
						WireName = new char[WireNameLength + 1];
						// load in the string
						DataStream.read( WireName, sizeof(WireName[0]) * WireNameLength );
						WireName[WireNameLength] = '\0';
						
						//Set the wire name here
						/////////////////
						ButtonCollision->SetWireMeshName( WireName );
						///////////

						delete WireName;
					}

					if( Channel > -1 )
					{
						mpSenders[Channel].push_back(ButtonCollision);
					}
				}
				break;
				// Box Button
			case 11:
				{
					IBaseObject* Button = CObjectManager::GetInstance()->CreateObject( OBJ_BOXBUTTON, _worldMatrix, _levelId );
					//CComponent_BoxButtonCollision* BoxButtonCollision = (CComponent_BoxButtonCollision*)BoxButton->GetComponent(ECOMP_COLLISION);
					CComponent_ButtonCollision* ButtonCollision = (CComponent_ButtonCollision*)Button->GetComponent(ECOMP_COLLISION);

					int WireNameLength = 0;
					DataStream.read( (char*)&WireNameLength, sizeof(WireNameLength) );

					char* WireName = NULL;

					if (WireNameLength > 0)
					{
						WireName = new char[WireNameLength + 1];
						// load in the string
						DataStream.read( WireName, sizeof(WireName[0]) * WireNameLength );
						WireName[WireNameLength] = '\0';
						
						//Set the wire name here
						/////////////////
						ButtonCollision->SetWireMeshName( WireName );
						///////////

						delete WireName;
					}

					CComponent_Render* renderComponent = (CComponent_Render*)Button->GetComponent( ECOMP_RENDER );

					renderComponent->GetRenderNode()->GetRenderData().m_uiTextureHandles[0] = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/T_BoxSpawn_D.png" );
					renderComponent->GetRenderNode()->GetRenderData().m_uiTextureHandles[2] = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/T_BoxSpawn_S.png" );

					if( Channel > -1 )
					{
						mpSenders[Channel].push_back(ButtonCollision);
					}
				}
				break;
				// Box Spawn Pipe
			case 12:
				{
					IBaseObject* BoxSpawn = CObjectManager::GetInstance()->CreateObject( OBJ_PIPE_BOX, _worldMatrix, _levelId );
					CComponent_BoxPipeCollision* BoxPipeCollision = (CComponent_BoxPipeCollision*)BoxSpawn->GetComponent(ECOMP_COLLISION);

					if (Channel != -1)
					{
						mpRecievers[Channel].push_back(BoxSpawn);
					}
				}
				break;
				// Door
			case 13:
				{
					IBaseObject* Door = CObjectManager::GetInstance()->CreateObject( OBJ_DOOR, _worldMatrix, _levelId );
					CComponent_DoorCollision* DoorCollision = (CComponent_DoorCollision*)Door->GetComponent(ECOMP_COLLISION);

					if (Channel != -1)
					{
						mpRecievers[Channel].push_back(Door);
						DoorCollision->SetHasAssociatedButton( true );
					}
				}
				break;
				//Boxes
			case 14:
				{
					CObjectManager::GetInstance()->CreateAnimatedBGObject( OBJ_ANIMATED_BG, _worldMatrix, "Boxes", AnimBoxesState::GetState(), _levelId );
				}
				break;
				// Conveyor Belt
			case 15:
				{
					CObjectManager::GetInstance()->CreateAnimatedBGObject( OBJ_ANIMATED_BG, _worldMatrix, "ConveyorBelt", AnimBeltState::GetState(), _levelId );
				}
				break;
				// Claw
			case 16:
				{
					CObjectManager::GetInstance()->CreateAnimatedBGObject( OBJ_ANIMATED_BG, _worldMatrix, "Claw", AnimClawState::GetState(), _levelId );
				}
				break;
				// Fan
			case 17:
				{
					CObjectManager::GetInstance()->CreateAnimatedBGObject( OBJ_ANIMATED_BG, _worldMatrix, "Blades", AnimFanState::GetState(), _levelId );
				}
				break;
			case 18:
				{
					CObjectManager::GetInstance()->CreateAnimatedBGObject( OBJ_ANIMATED_BG, _worldMatrix, "CrateClaw", AnimCrateClawState::GetState(), _levelId );
				}
				break;
			case 19:
				{
					CObjectManager::GetInstance()->CreateAnimatedBGObject( OBJ_ANIMATED_BG, _worldMatrix, "BossMachine", AnimBossState::GetState(), _levelId );
				}
				break;
			};
		}

		//D3DXMATRIX _worldMatrix;
		//D3DXMatrixIdentity (&_worldMatrix);
		//
		//double PosX, PosY, PosZ;
		//DataStream.read( (char*)&PosX, sizeof(PosX) );
		//DataStream.read( (char*)&PosY, sizeof(PosY) );
		//DataStream.read( (char*)&PosZ, sizeof(PosZ) );
		//
		//_worldMatrix._41 = (float)PosX;
		//_worldMatrix._42 = (float)PosY;
		//_worldMatrix._43 = (float)PosZ;
		//
		//IBaseObject* CheckPointObject = CObjectManager::GetInstance()->CreateObject( OBJ_CHECKPOINT, _worldMatrix );
		//CComponent_Checkpoint* CheckPoint = (CComponent_Checkpoint*)CheckPointObject->GetComponent(ECOMP_CHECKPOINT);
		//
		//
		//int NameLength = 0;
		//DataStream.read( (char*)&NameLength, sizeof(NameLength) );
		//
		//char* NextLevel = NULL;
		//
		//if (NameLength > 0)
		//{
		//	NextLevel = new char[NameLength + 1];
		//	DataStream.read( NextLevel, sizeof(NextLevel[0]) * NameLength );
		//	NextLevel[NameLength] = '\0';
		//	CheckPoint->SetNextLevel ( NextLevel );
		//	delete NextLevel;
		//}

		

		// HARD CODED INTAKE PIPE
		/*D3DXMATRIX _ObjectMatrix, rot;
		D3DXMatrixIdentity( &_ObjectMatrix );
		D3DXMatrixTranslation( &_ObjectMatrix, 1825.0f, 265.0f, 0.0f );
		mpSenders[0] = CObjectManager::GetInstance()->CreateObject(OBJ_PIPE_INTAKE, _ObjectMatrix )->GetComponent( ECOMP_COLLISION );*/
		//////////

		std::unordered_map< unsigned int, std::vector <IComponent*> >::iterator itrSenders = mpSenders.begin();

		while( itrSenders != mpSenders.end() )
		{
			for (unsigned int SenderIndex = 0; SenderIndex < (*itrSenders).second.size(); SenderIndex++)
			{
				for (unsigned int ReciverIndex = 0; ReciverIndex < mpRecievers[(*itrSenders).first].size(); ReciverIndex++ )
				{
					if ((*itrSenders).second[SenderIndex]->GetParent() == mpRecievers[(*itrSenders).first][ReciverIndex])
					{
						continue;
					}

					switch( (*itrSenders).second[SenderIndex]->GetType() )
					{
					case ECOMP_COLLISION:
						{
							switch( ((CComponent_Collision*)((*itrSenders).second[SenderIndex]))->GetCollType() )
							{
							case CCMP_STEAMPIPE:
								{
									((CComponent_IntakePipeCollision*)(*itrSenders).second[SenderIndex])->SetPartner( mpRecievers[(*itrSenders).first][ReciverIndex]);
									break;
								}
							case CCMP_OUTLET:
								{
									((CComponent_OutletCollision*)(*itrSenders).second[SenderIndex])->SetAssociatedObj( mpRecievers[(*itrSenders).first][ReciverIndex]);
									break;
								}
							case CCMP_BUTTON:
								{
									((CComponent_ButtonCollision*)(*itrSenders).second[SenderIndex])->SetPartner(mpRecievers[(*itrSenders).first][ReciverIndex]);
									break;
								}
							case CCMP_BREAKABLE:
								{
									((CComponent_BreakableWallCollision*)(*itrSenders).second[SenderIndex])->AddAssociatedObj( mpRecievers[(*itrSenders).first][ReciverIndex] );
									break;
								}
							case CCMP_POOL:
								{
									((CComponent_WaterPoolCollision*)(*itrSenders).second[SenderIndex])->AddAssociatedObj( mpRecievers[(*itrSenders).first][ReciverIndex] );
									break;
								}
							}

							break;
						}
					}
				}
			}

			itrSenders++;
		}

		return true;
	}
	return false;
}

bool MeshManager::LoadEnvironmentMoveableObjects( const char* szInfiniteHPLevelObjectsFile, const char* szInfiniteHPLevelObjectsFileData, char _levelId )
{
	fstream ObjectStream;
	ObjectStream.open( szInfiniteHPLevelObjectsFile, ios_base::in | ios_base::binary );

	fstream DataStream;
	DataStream.open ( szInfiniteHPLevelObjectsFileData, ios_base::in | ios_base::binary );

	// If the File is open begin loading in objects.
	if( ObjectStream.is_open() && DataStream.is_open() )
	{
		int NumberOfObjects;
		ObjectStream.read( (char*)&NumberOfObjects, sizeof(NumberOfObjects) );

		int NumDataObjects = 0;
		DataStream.read( (char*)&NumDataObjects, sizeof(NumDataObjects) );

		// Loop through Each Object.
		for( int i = 0; i < NumDataObjects; i++)
		{
			unsigned char ObjID;
			D3DXMATRIX _worldMatrix;
			D3DXMatrixIdentity (&_worldMatrix);

			// Read in Object ID - char
			ObjectStream.read( (char*)&ObjID, sizeof(ObjID) );

			int DataID;
			DataStream.read( (char*)&DataID, sizeof(DataID) );

			// Read in the World Matrix - D3DXMATRIX
			ObjectStream.read( (char*)&_worldMatrix, sizeof(_worldMatrix) );

			// After the Object data is read in,
			// Create the Object using the Object Factory.
			// Object Values are the Following:
					// 0 - Box
					// 1 - Cork
			double PosX, PosY, PosZ;
			DataStream.read( (char*)&PosX, sizeof(PosX) );
			DataStream.read( (char*)&PosY, sizeof(PosY) );
			DataStream.read( (char*)&PosZ, sizeof(PosZ) );

			_worldMatrix._41 = (float)PosX;
			_worldMatrix._42 = (float)PosY;
			_worldMatrix._43 = (float)PosZ;

			switch( DataID )
			{
			case 0:
				{
					CObjectManager::GetInstance()->CreateObject( OBJ_CRATE , _worldMatrix, _levelId );
				}
				break;
			case 1:
				{
					CObjectManager::GetInstance()->CreateObject( OBJ_CORK , _worldMatrix, _levelId );
				}
				break;
			case 2:
				{
					CObjectManager::GetInstance()->CreateObject( OBJ_PLUG , _worldMatrix, _levelId );
				}
				break;
			case 3:
				{
					//static bool onlyOne = false;

					//if (!onlyOne)
					//{
					//	onlyOne = true;
					//	_worldMatrix._41 = (float)17500;
					//	_worldMatrix._42 = (float)PosY + 300.0f;
					//	_worldMatrix._43 = (float)PosZ;
					//}

					IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
					IBaseObject* playerArm = CObjectManager::GetInstance()->GetPlayerArm();

					// Set the levels spawn position to the position loaded up anyways.
					// Set the Current Levels Spawn Position.
					vec3f Position = vec3f( _worldMatrix._41, _worldMatrix._42, _worldMatrix._43 );
					LevelManager::GetInstance()->GetCurrentLevel()->SetPlayerSpawnPos( Position );

					if( !player )
						// Create the Player
						CObjectManager::GetInstance()->CreateObject( OBJ_PLAYER , _worldMatrix, -1 );

					if( !playerArm )
					{
						// Create the Player Arm.
						D3DXMatrixIdentity (&_worldMatrix);
						CObjectManager::GetInstance()->CreateObject(OBJ_PLAYER_ARM, _worldMatrix, -1 );
					}
				}
				break;
			case 4:
				{
					CObjectManager::GetInstance()->CreateObject(OBJ_MONSTER, _worldMatrix, _levelId);
				}
				break;
			}
		}

		return true;
	}

	return false;
}

bool MeshManager::LoadCollisionEnvironment(const char* szInfiniteHPLevelName, char _levelID)
{
	fstream in;
	in.open (szInfiniteHPLevelName, std::ios_base::in | std::ios_base::binary);


	if (!in.is_open ())
		return false;

	int numAABBs = 0;
	in.read ((char*)&numAABBs, sizeof (int));

	
	for (int aabbIndex = 0; aabbIndex < numAABBs; aabbIndex++)
	{
		int type = 0;
		in.read ((char*)&type, sizeof (int));
		vec3f min, max;
		in.read ((char*)&min, sizeof(vec3f));
		in.read ((char*)&max, sizeof(vec3f));
		AABB aabb;
		CCollisionPoint Min, Max;
		Min.Set3DCentorid (min);
		Max.Set3DCentorid (max);
		aabb.SetMaxPoint (Max);
		aabb.SetMinPoint (Min);

		vec3f temp = (Max.Get3DCentorid() - Min.Get3DCentorid()) * 0.5f;
		temp = Min.Get3DCentorid() + temp;
		aabb.SetCenterPoint ( temp );

		aabb.SetBodyType (BT_STATIC);
		aabb.SetVolumeType (VMT_AABB);

		IBaseObject* object = CObjectManager::GetInstance()->AddObject (&aabb, OBJ_WORLD_COLLISION);
		object->SetLevelID( _levelID );
		/*object->SetCollidableObject (&aabb);
		object->SetType (OBJ_WORLD_COLLISION);*/

		//CObjectManager::GetInstance()->AddObject (object);
		//CObjectManager::GetInstance()->m_AABBs.push_back (aabb);
	}

	return true;
}

unsigned int MeshManager::_LoadBoundingVolumes( const char* InfiniteHPMeshData, BaseMesh** ToFill)
{
	CCollisionVolume* temp = nullptr;
	unsigned int returnValue = 0;
	unsigned char bvCount;
	int bvType;
	// Read in the bounding volume count
	memcpy( &bvCount, InfiniteHPMeshData, 1);
	// Make sure BVCount is valid
	if( bvCount > 0 && bvCount < 5)
	{
		returnValue++;
		memcpy( &bvType, &InfiniteHPMeshData[returnValue], 4);
		returnValue+= 4;

		// Depending on the BV Type read in the data....
		switch(bvType)
		{
		case VMT_AABB:
			{
				// Allocate memory
				temp = new AABB();
				// The + is for the offset to not read in the type.
				//memcpy((void*)((char*)temp + sizeof(VT_AABB) + sizeof (BodyType)), &InfiniteHPMeshData[returnValue], sizeof(AABB) - sizeof(VT_AABB) + sizeof (BodyType));
				vec3f max, min;
				memcpy ((char*)&min, &InfiniteHPMeshData[returnValue], sizeof(min));
				returnValue += sizeof(min);
				memcpy (&max, &InfiniteHPMeshData[returnValue], sizeof(max));
				returnValue += sizeof (max);

				((AABB*)temp)->SetMaxPoint (max);
				((AABB*)temp)->SetMinPoint (min);
				// Don't forget to subtract it because you did not read it.
				//returnValue += sizeof(AABB) - sizeof(VT_AABB) - sizeof (BodyType);
				// Remember to store the type
				/*vec3f Direction = (((AABB*)temp)->GetMaxPoint3D() - ((AABB*)temp)->GetMinPoint3D()) * 0.5f;
				((AABB*)temp)->SetWorldPoint ( ((AABB*)temp)->GetMinPoint3D() + Direction);*/
				temp->SetBodyType (BT_DYNAMIC);
				temp->SetVolumeType ((VolumeType)bvType);
				// Deallocate memory.
				//delete temp;
			}
			break;
		case VMT_SPHERE:
			{
				temp = new Sphere();
				// The + is for the offset to not read in the type.
				//memcpy((void*)((char*)temp + sizeof(VT_AABB) + sizeof (BodyType)), &InfiniteHPMeshData[returnValue], sizeof(AABB) - sizeof(VT_AABB) + sizeof (BodyType));
				vec3f Center;
				float fRadius;
				memcpy ((char*)&Center, &InfiniteHPMeshData[returnValue], sizeof(Center));
				returnValue += sizeof(Center);
				memcpy ((char*)&fRadius, &InfiniteHPMeshData[returnValue], sizeof(fRadius));
				returnValue += sizeof (fRadius);

				((Sphere*)temp)->SetCenter (Center);
				((Sphere*)temp)->SetRadius (fRadius);
				// Don't forget to subtract it because you did not read it.
				//returnValue += sizeof(AABB) - sizeof(VT_AABB) - sizeof (BodyType);
				// Remember to store the type
				/*vec3f Direction = (((AABB*)temp)->GetMaxPoint3D() - ((AABB*)temp)->GetMinPoint3D()) * 0.5f;
				((AABB*)temp)->SetWorldPoint ( ((AABB*)temp)->GetMinPoint3D() + Direction);*/
				temp->SetBodyType (BT_DYNAMIC);
				temp->SetVolumeType ((VolumeType)bvType);
			}
		};
		if( m_vpLoadedNodes[m_vpLoadedNodes.size()-1].m_pColVolume )
			delete m_vpLoadedNodes[m_vpLoadedNodes.size()-1].m_pColVolume;
		m_vpLoadedNodes[m_vpLoadedNodes.size()-1].m_pColVolume = temp;
	}

	

	return returnValue;
}

void MeshManager::LoadAnimatedMesh(const char* szInfiniteHPMeshName, D3DXMATRIX transformMat )
{
	// Load Mesh from file.

	//if( GetInstance() == NULL )
		// Error Manager not yet initialized.
	//if( szInfiniteHPMeshName == NULL )
		// Error Mesh name passed is not valid.

	int len = (strlen(szInfiniteHPMeshName) + 1) + MESH_DIR_LEN + FILE_EXT_LEN;
	char* MeshFileName = new char[len];
	sprintf_s(MeshFileName, len, "%s%s%s", MESH_DIR, szInfiniteHPMeshName, FILE_EXT);

	// Declare the stream for read/writing.
	fstream MeshStream;
	// Open the file
	MeshStream.open( MeshFileName, ios_base::in | ios_base::binary);

	// If file is not open then directory/name was wrong.
	if( !MeshStream.is_open() )
	{
		// Log Error.
	}
	else
	{
		// The file was opened correctly so lets grab the data.
		MeshStream.seekg(0,ios_base::end);

		int filesize = (int)MeshStream.tellg();

		// Allocate enough data to hold the file in memory.
		char* fileData = new char[filesize];

		// Put the file data into the local data.
		MeshStream.seekg(0, ios_base::beg);
		MeshStream.read(fileData, filesize);

		int index = 0;
		LoadAnimatedMeshFromFile(fileData, index, transformMat); 

		delete[] fileData;
	}

	delete MeshFileName;
	MeshStream.close();
}

void MeshManager::LoadAnimatedMeshFromFile( const char* InfiniteHPMeshData, int& pos, D3DXMATRIX transformMat)
{
	//if( GetInstance() == NULL )
		// Log Error and return NULL.

	AnimatedMesh* newMesh = NULL;
	pos = 0;
	unsigned int vertCount, tricount, triSize, vertAttrib = 0;
	int textureHandles[MAX_TEXTURES] = {0};
	for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
	{
#if DEFERREDRENDER
		textureHandles[i] = 0;
#else
		textureHandles[i] = 0;
#endif
	}
	if( LoadCommonMeshData(InfiniteHPMeshData, (BaseMesh **)&newMesh, pos, textureHandles, vertAttrib) || newMesh == NULL){}
		//return newMesh or handle;

	unsigned char InfluenceCount = 0;
	VERTEX_POSNORMTANUVINF VertexToFill;
	// Grab number of verts.
	vertCount = newMesh->m_vVerts.size();
	newMesh->m_vVerts.resize(0);

	for( unsigned int i = 0; i < vertCount; ++i )
	{
		memcpy( &VertexToFill, &InfiniteHPMeshData[pos], sizeof(VERTEX_POSNORMTANUV) );
		pos += sizeof(VERTEX_POSNORMTANUV);
		memcpy( &InfluenceCount, &InfiniteHPMeshData[pos], 1 );
		pos += 1;
		int index;

		for( int j = 0; j < 4; j++ )
		{
			if( j < InfluenceCount )
			{
				memcpy( &index, &InfiniteHPMeshData[pos], 4);
				VertexToFill.m_JointIndices[j] = (float)index;
				pos += 4;
				memcpy(&VertexToFill.m_JointWeights[j], &InfiniteHPMeshData[pos], 4 );
				pos += 4;
			}
			else
			{
				VertexToFill.m_JointIndices[j] = -1;
				VertexToFill.m_JointWeights[j] = 0.0f;
			}
		}
		newMesh->m_vVerts.push_back(VertexToFill);
	}

	
	memcpy( &tricount, &InfiniteHPMeshData[pos], sizeof(tricount));
	pos += sizeof(tricount);

	// Resize the mesh triangles to the tri count.
	newMesh->m_vTris.resize(tricount);
	triSize = sizeof(Triangle) * tricount;
	memcpy( &newMesh->m_vTris[0], &InfiniteHPMeshData[pos], triSize);
	pos += triSize;

	if( IsMeshAlreadyLoaded( newMesh->GetMeshName() ) == -1 )
	{
		RenderNode* node = new RenderNode;	
		loadedMeshData toVector;
		*toVector.m_unBitFlags = vertAttrib;
		node->m_unBitFlags = toVector.m_unBitFlags;
		node->Render = RenderEngine::DrawSmoothSkinnedMesh;
		node->FillAnimRenderData( (void*)&newMesh->m_vVerts[0], newMesh->m_vVerts.size(), sizeof(VERTEX_POSNORMTANUVINF), (void*)&newMesh->m_vTris[0], tricount * 3 );
		node->renderData.m_d3dVertDecl = RenderEngine::GetPosNormTanUVInfDecl();
		node->AddFlags( RE_ANIMATED );// = true;
		//node->renderData.m_vTransform.push_back(transformMat);
		for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
			node->renderData.m_uiTextureHandles[i] = textureHandles[i];

		toVector.m_pNode = node;
		toVector.m_sMeshName = newMesh->GetMeshName();

		m_vpLoadedNodes.push_back( toVector );
		_LoadBoundingVolumesForAnimatedMeshes( newMesh, m_vpLoadedNodes.size()-1, transformMat );

		RenderEngine::AddToObjectRenderSet( node,  transformMat );

		pos += _LoadBoundingVolumes( &InfiniteHPMeshData[pos], (BaseMesh**)&newMesh);
	}
	//else
	//{
	//	m_vpLoadedNodes[nCheckResult].m_pNode->renderData.m_vTransform.push_back(transformMat);
	//}
	// Load Bounding Volumes

			// Read in a BVcount which is a unsigned char
			// If bvCount is a valid value...
				// read in bvType also a unsigned char
				// Read in appropriate data for
					// AABB - 0
					// Sphere - 1
					// Capsule - 2;
			// Do not forget to increment the pos in the data.
	//pos += _LoadBoundingVolumes( &InfiniteHPMeshData[pos], (BaseMesh**)&newMesh);

	int _result = IsMeshAlreadyLoaded( newMesh->GetMeshName() );
	// After bounding volume loaded load up matrix.
	delete newMesh;
	// Push back mesh onto list of "Loaded" Meshes.
	// return mesh or handle.
	//return _result;
}

int MeshManager::LoadEnvironmentMeshFromFile( const char* InfiniteHPMeshData, int& pos, Mesh** newMesh)
{
	//if( GetInstance() == NULL )
		// Log Error and return NULL.

	pos = 0;
	unsigned int vertSize, tricount, triSize, vertAttrib = 0;
	int textureHandles[MAX_TEXTURES] = {0};
	for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
	{
#if DEFERREDRENDER
		textureHandles[i] = 0;
#else
		textureHandles[i] = 0;
#endif
	}
	if( LoadCommonMeshData(InfiniteHPMeshData, (BaseMesh **)newMesh, pos, textureHandles, vertAttrib) || newMesh == NULL){}
		//return newMesh or handle;
		
	vertSize = sizeof(VERTEX_POSNORMTANUV) * (*newMesh)->m_vVerts.size();
	memcpy( &(*newMesh)->m_vVerts[0], &InfiniteHPMeshData[pos], vertSize);
	pos += vertSize;
	memcpy( &tricount, &InfiniteHPMeshData[pos], sizeof(tricount));
	pos += sizeof(tricount);

	// Resize the mesh triangles to the tri count.
	(*newMesh)->m_vTris.resize(tricount);
	triSize = sizeof(Triangle) * tricount;
	memcpy( &(*newMesh)->m_vTris[0], &InfiniteHPMeshData[pos], triSize);
	pos += triSize;

	if( IsMeshAlreadyLoaded( (*newMesh)->GetMeshName() ) == -1 )
	{
		RenderNode* node = new RenderNode;
		loadedMeshData toVector;
		*toVector.m_unBitFlags = vertAttrib;
		node->m_unBitFlags = toVector.m_unBitFlags;
		//Sector2_PS_M_AstheticForegroundPipe1
		//bool test = false;
		if( strcmp( (*newMesh)->GetMeshName(), "sky" ) == 0 )
		{
			node->SetExtraData( 0.0625f );
		}
		if( strcmp( (*newMesh)->GetMeshName(), "rain" ) == 0 )
		{
			node->SetExtraData( 0.25f );
		}

		node->Render = RenderEngine::DrawTexturedEnvironmentMesh;
		node->FillRenderData( (void*)&(*newMesh)->m_vVerts[0], (*newMesh)->m_vVerts.size(), sizeof(VERTEX_POSNORMTANUV), (void*)&(*newMesh)->m_vTris[0], tricount * 3 );
		node->renderData.m_d3dVertDecl = RenderEngine::GetPosNormTanUVDecl();
		for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
			node->renderData.m_uiTextureHandles[i] = textureHandles[i];
		//RenderEngine::AddToEnvironmentRenderSet( node );

		
		toVector.m_pNode = node;
		toVector.m_sMeshName = (*newMesh)->GetMeshName();

		m_vpLoadedNodes.push_back( toVector );

	}
	// No longer loading up Bounding Volumes we are generating them now.
	//pos += _LoadBoundingVolumes( &InfiniteHPMeshData[pos], (BaseMesh**)&newMesh);

	int _result = IsMeshAlreadyLoaded( (*newMesh)->GetMeshName() );
	// Push back mesh onto list of "Loaded" Meshes.
	// return mesh or handle.
	return _result;
}

void MeshManager::LoadHazard(const char* szInfiniteHPMeshName, CCollisionAABB& volume, D3DXMATRIX transformMat )
{
	//if( GetInstance() == NULL )
		// Error Manager not yet initialized.
	//if( szInfiniteHPMeshName == NULL )
		// Error Mesh name passed is not valid.

	int len = (strlen(szInfiniteHPMeshName) + 1) + MESH_DIR_LEN + FILE_EXT_LEN;
	char* MeshFileName = new char[len];
	sprintf_s(MeshFileName, len, "%s%s%s", MESH_DIR, szInfiniteHPMeshName, FILE_EXT);

	// Declare the stream for read/writing.
	fstream MeshStream;
	// Open the file
	MeshStream.open( MeshFileName, ios_base::in | ios_base::binary);

	// If file is not open then directory/name was wrong.
	if( !MeshStream.is_open() )
	{
		// Log Error.
	}
	else
	{
		// The file was opened correctly so lets grab the data.
		MeshStream.seekg(0,ios_base::end);

		int filesize = (int)MeshStream.tellg();

		// Allocate enough data to hold the file in memory.
		char* fileData = new char[filesize];

		// Put the file data into the local data.
		MeshStream.seekg(0, ios_base::beg);
		MeshStream.read(fileData, filesize);

		Mesh* newMesh = NULL;
		LoadHazardMeshFromFile( fileData, &newMesh );
		volume = _GenerateVolumesForHazards( newMesh, transformMat );

		delete[] fileData;
		delete newMesh;
	}

	delete MeshFileName;
	MeshStream.close();
}

int MeshManager::LoadHazardMeshFromFile( const char* InfiniteHPMeshData, Mesh** newMesh )
{
	//if( GetInstance() == NULL )
		// Log Error and return NULL.

	int pos = 0;
	unsigned int vertSize, tricount, triSize, vertAttrib = 0;
	int textureHandles[MAX_TEXTURES] = {0};
	for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
	{
#if DEFERREDRENDER
		textureHandles[i] = 0;
#else
		textureHandles[i] = 0;
#endif
	}
	if( LoadCommonMeshData(InfiniteHPMeshData, (BaseMesh **)newMesh, pos, textureHandles, vertAttrib) || newMesh == NULL)
		{}
		//return newMesh or handle;
		
	vertSize = sizeof(VERTEX_POSNORMTANUV) * (*newMesh)->m_vVerts.size();
	memcpy( &(*newMesh)->m_vVerts[0], &InfiniteHPMeshData[pos], vertSize);
	pos += vertSize;
	memcpy( &tricount, &InfiniteHPMeshData[pos], sizeof(tricount));
	pos += sizeof(tricount);

	// Resize the mesh triangles to the tri count.
	(*newMesh)->m_vTris.resize(tricount);
	triSize = sizeof(Triangle) * tricount;
	memcpy( &(*newMesh)->m_vTris[0], &InfiniteHPMeshData[pos], triSize);
	pos += triSize;

	if( IsMeshAlreadyLoaded( (*newMesh)->GetMeshName() ) == -1 )
	{
		RenderNode* node = new RenderNode;	
		loadedMeshData toVector;
		*toVector.m_unBitFlags = vertAttrib;
		node->m_unBitFlags = toVector.m_unBitFlags;
		node->Render = RenderEngine::DrawTexturedEnvironmentMesh;
		node->FillRenderData( (void*)&(*newMesh)->m_vVerts[0], (*newMesh)->m_vVerts.size(), sizeof(VERTEX_POSNORMTANUV), (void*)&(*newMesh)->m_vTris[0], tricount * 3 );
		node->renderData.m_d3dVertDecl = RenderEngine::GetPosNormTanUVDecl();
		for( unsigned int i = 0u; i < MAX_TEXTURES; ++i )
			node->renderData.m_uiTextureHandles[i] = textureHandles[i];
		/*RenderEngine::AddToEnvironmentRenderSet( node );*/

		
		toVector.m_pNode = node;
		toVector.m_sMeshName = (*newMesh)->GetMeshName();

		m_vpLoadedNodes.push_back( toVector );
	}
	// No longer loading up Bounding Volumes we are generating them now.

	int _result = IsMeshAlreadyLoaded( (*newMesh)->GetMeshName() );
	// Push back mesh onto list of "Loaded" Meshes.
	// return mesh or handle.
	return _result;
}


void MeshManager::_LoadBoundingVolumesForEnvironment( Mesh* ToFill, int _meshIndex, D3DXMATRIX meshMatrix )
{
	Mesh* tempMesh = (Mesh*)ToFill;
	vector<VERTEX_POSNORMTANUV> pVerts = tempMesh->GetVerts();

	// Set default values to find the Min and Max.
	float Min[3], Max[3];
	Min[0] = Min[1] = Min[2] = FLT_MAX;
	Max[0] = Max[1] = Max[2] = -FLT_MAX;

	CCollisionAABB boundingVolume;
	boundingVolume.SetBodyType( BT_STATIC );
	// Loop through all verts to generate an AABB for each environmental object.
	for( unsigned int i = 0; i < pVerts.size(); i++)
	{
		VERTEX_POSNORMTANUV currentVertex = pVerts[i];
		D3DXVECTOR3 pos = currentVertex.m_Position;

		// Multiply the Position by the Matrix to calculate bounding box in world space.
		D3DXVec3TransformCoord( &pos, &pos, &meshMatrix);

		// If the current values are greater than the Max then this is the new Max.
		if( pos.x > Max[0] )
			Max[0] = pos.x;
		if( pos.y > Max[1] )
			Max[1] = pos.y;
		if( pos.z > Max[2] )
			Max[2] = pos.z;

		// If the current values are less than the Min then this is the new Min.
		if( pos.x < Min[0] )
			Min[0] = pos.x;
		if( pos.y < Min[1] )
			Min[1] = pos.y;
		if( pos.z < Min[2] )
			Min[2] = pos.z;
	}

	// We should now have the new Min and Max's so lets set up the new AABB.
	boundingVolume.SetMaxPoint( (*(vec3f*)&Max) );
	boundingVolume.SetMinPoint( (*(vec3f*)&Min) );
	boundingVolume.SetVolumeType( VMT_AABB );

	m_vpLoadedNodes[_meshIndex].m_pNode->renderData.m_vCollisions.push_back( boundingVolume );
}

void MeshManager::_LoadBoundingVolumesForAnimatedMeshes( AnimatedMesh* ToFill, int _meshIndex, D3DXMATRIX meshMatrix )
{
	AnimatedMesh* tempMesh = (AnimatedMesh*)ToFill;
	vector<VERTEX_POSNORMTANUVINF> pVerts = tempMesh->GetVerts();

	// Set default values to find the Min and Max.
	float Min[3], Max[3];
	Min[0] = Min[1] = Min[2] = FLT_MAX;
	Max[0] = Max[1] = Max[2] = -FLT_MAX;

	CCollisionAABB boundingVolume;
	boundingVolume.SetBodyType( BT_DYNAMIC );
	// Loop through all verts to generate an AABB for each environmental object.
	for( unsigned int i = 0; i < pVerts.size(); i++)
	{
		VERTEX_POSNORMTANUVINF currentVertex = pVerts[i];
		D3DXVECTOR3 pos = currentVertex.m_Position;

		// Multiply the Position by the Matrix to calculate bounding box in world space.
		D3DXVec3TransformCoord( &pos, &pos, &meshMatrix);

		// If the current values are greater than the Max then this is the new Max.
		if( pos.x > Max[0] )
			Max[0] = pos.x;
		if( pos.y > Max[1] )
			Max[1] = pos.y;
		if( pos.z > Max[2] )
			Max[2] = pos.z;

		// If the current values are less than the Min then this is the new Min.
		if( pos.x < Min[0] )
			Min[0] = pos.x;
		if( pos.y < Min[1] )
			Min[1] = pos.y;
		if( pos.z < Min[2] )
			Min[2] = pos.z;
	}

	// We should now have the new Min and Max's so lets set up the new AABB.
	boundingVolume.SetMaxPoint( (*(vec3f*)&Max) );
	boundingVolume.SetMinPoint( (*(vec3f*)&Min) );
	boundingVolume.SetVolumeType( VMT_AABB );

	if( _strcmpi(ToFill->GetMeshName(), "CrateClaw") == 0 )
	{
		boundingVolume.SetMaxPoint( boundingVolume.GetMaxPoint3D() + vec3f( 250.0f, 0.0f, 100.0f));
		boundingVolume.SetMinPoint( boundingVolume.GetMinPoint3D() + vec3f( -250.0f, -400.0f, -100.0f) );
	}

	m_vpLoadedNodes[_meshIndex].m_pNode->renderData.m_vCollisions.push_back( boundingVolume );
}

CCollisionAABB MeshManager::_GenerateVolumesForHazards( Mesh* ToFill, D3DXMATRIX meshMatrix )
{
	meshMatrix._41 = meshMatrix._42 = meshMatrix._43 = 0.0f;
	Mesh* tempMesh = ToFill;
	vector<VERTEX_POSNORMTANUV> pVerts = tempMesh->GetVerts();

	// Set default values to find the Min and Max.
	float Min[3], Max[3];
	Min[0] = Min[1] = Min[2] = FLT_MAX;
	Max[0] = Max[1] = Max[2] = -FLT_MAX;

	CCollisionAABB boundingVolume;
	boundingVolume.SetBodyType( BT_DYNAMIC );
	// Loop through all verts to generate an AABB for each environmental object.
	for( unsigned int i = 0; i < pVerts.size(); i++)
	{
		VERTEX_POSNORMTANUV currentVertex = pVerts[i];
		D3DXVECTOR3 pos = currentVertex.m_Position;

		// Multiply the Position by the Matrix to calculate bounding box in world space.
		D3DXVec3TransformCoord( &pos, &pos, &meshMatrix);

		// If the current values are greater than the Max then this is the new Max.
		if( pos.x > Max[0] )
			Max[0] = pos.x;
		if( pos.y > Max[1] )
			Max[1] = pos.y;
		if( pos.z > Max[2] )
			Max[2] = pos.z;

		// If the current values are less than the Min then this is the new Min.
		if( pos.x < Min[0] )
			Min[0] = pos.x;
		if( pos.y < Min[1] )
			Min[1] = pos.y;
		if( pos.z < Min[2] )
			Min[2] = pos.z;
	}

	// We should now have the new Min and Max's so lets set up the new AABB.
	boundingVolume.SetMaxPoint( (*(vec3f*)&Max) );
	boundingVolume.SetMinPoint( (*(vec3f*)&Min) );
	boundingVolume.SetVolumeType( VMT_AABB );
	//boundingVolume.SetWorldPoint ( );

	return boundingVolume;
}

