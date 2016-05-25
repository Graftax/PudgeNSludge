#ifndef MESH_H_
#define MESH_H_


#include <vector>
using namespace std;
#include "MeshManager.h"

struct Triangle
{
	unsigned int Index0, Index1, Index2;
};

//enum BoundingVolumes { BV_INVALID = 0, BV_AABB, BV_SPHERE, BV_CAPSULE, BV_TRIANGLE };
//
//struct BoundingVolume
//{
//	BoundingVolumes type;
//	D3DXVECTOR3 one;
//};

//struct AABB : public BoundingVolume
//{
//	D3DXVECTOR3 max;
//};

//struct Sphere : public BoundingVolume
//{
//	float radius;
//};
//
//struct Capsule : public BoundingVolume
//{
//	D3DXVECTOR3 End;
//	float radius;
//};
////////////////////////////////////////////////////////////////
// Stores the Mesh data
////////////////////////////////////////////////////////////////

__declspec(align(32))
class BaseMesh
{
	friend class MeshManager;

private:
	char*								m_Name;				// Name of the Mesh.
	vector<Triangle>					m_vTris;			// Vector of Indices.
	D3DXMATRIX							m_WorldMatrix;		// The World Matrix of the mesh.
	vector<int>							m_vTexture;			// A vector of handles to the texture.

public:
	vector<Triangle>				&GetTris(void)	{ return m_vTris; }
	vector<int>						&GetTextureHandles(void){ return m_vTexture; }
	D3DXMATRIX						&GetWorldMat(void){ return m_WorldMatrix; }
	char							*GetMeshName(void){ return m_Name; }

};

__declspec(align(32))
class Mesh : public BaseMesh
{
	friend class MeshManager;

protected:
	vector<VERTEX_POSNORMTANUV>						m_vVerts;			// Vector of Vertices.

public:

	Mesh(){}
	~Mesh(){ delete GetMeshName();}

	vector<VERTEX_POSNORMTANUV>						&GetVerts(void){ return m_vVerts; }
};

__declspec(align(32))
class AnimatedMesh : public BaseMesh
{
	friend class MeshManager;

protected:
	vector<VERTEX_POSNORMTANUVINF>				m_vVerts;			// Vector of Vertices.

public:
	
	AnimatedMesh(){}
	~AnimatedMesh(){delete GetMeshName();}

	vector<VERTEX_POSNORMTANUVINF>						&GetVerts(void){ return m_vVerts; }

};

#endif