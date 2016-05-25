#include "CollisionFrustum.h"
#include "CollisionShapes.h"

CCollisionFrustum::CCollisionFrustum ()
{
	m_unMaxPlanes = 6;
	m_unMaxCorners = 8;

	m_cplPlanes = new CCollisionPlane [m_unMaxPlanes];
	m_cptCorners = new CCollisionPoint [m_unMaxCorners];
}

CCollisionFrustum::~CCollisionFrustum ()
{
	delete[] m_cplPlanes;
	delete[] m_cptCorners;
}

///////////////////////////////////////////////////////////////////////////////////
// function name: infrustum
// purpose: takes in a frustum and a sphere (frustum, spa). returns true if the sphere
//			is in the view frustum.
// original author: josh fields
// creation date: 2/4/2012
// last modification by:	
// last modification date:	
///////////////////////////////////////////////////////////////////////////////////
bool CCollisionFrustum::infrustum( CCollisionSphere spa )
{
	for (size_t unplaneindex = 0; unplaneindex < 6; ++unplaneindex)
	{
		if (!m_cplPlanes[unplaneindex].SphereToPlaneHalfSpaceTest (spa))
		{
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// function name: infrustum
// purpose: takes in a frustum and a capsule (frustum, cpa). returns true if the capsule
//			is in the view frustum.
// original author: josh fields
// creation date: 2/4/2012
// last modification by:	
// last modification date:	
///////////////////////////////////////////////////////////////////////////////////
bool CCollisionFrustum::infrustum( CCollisionCapsule cpa )
{
	for (size_t unplaneindex = 0; unplaneindex < 6; ++unplaneindex)
	{
		if (!m_cplPlanes[unplaneindex].CapsuleToPlaneHalfSpaceTest (cpa))
		{
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// function name: infrustum
// purpose: takes in a frustum and a AABB (frustum, AABB). returns true if the AABB
//			is in the view frustum.
// original author: josh fields
// creation date: 2/4/2012
// last modification by:	
// last modification date:	
///////////////////////////////////////////////////////////////////////////////////
bool CCollisionFrustum::infrustum( CCollisionAABB aabb )
{
	for( unsigned int unPlaneIndex = 0; unPlaneIndex < 6; unPlaneIndex++)
	{
		// If the result returns 2 then the AABB is behind one of the planes and 
		// failed the frustum check.
		if( m_cplPlanes[unPlaneIndex].ClassifyAABBToPlane( aabb ) == 2 )
			return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////
// function name: buildfrustum
// purpose: takes in four floats, and a matrix4x4 (fov, neardist,
//			fardist, ratio, camxform)and an output pramater frustum (frustum). 
//			this functions computes the planes that make the frustum and places the 
//			new frustum in the frustum output.
// original author: josh fields
// creation date: 2/11/2012
// last modification by:	
// last modification date:	
///////////////////////////////////////////////////////////////////////////////////
void CCollisionFrustum::Build (float fov, float neardist, float fardist, float ratio, const matrix4f camxform )
{
	vec3f nearcenter = camxform.axis_w + camxform.axis_z * neardist;
	vec3f farcenter = camxform.axis_w + camxform.axis_z * fardist;
	
	float hnear = 2 * tan( fov / 2 ) * neardist;
	float hfar = 2 * tan( fov / 2 ) * fardist;
	float wnear = hnear * ratio;
	float wfar = hfar * ratio;

	m_cptCorners[ftl].Set3DCentorid (farcenter + camxform.axis_y * (hfar*0.5f) - camxform.axis_x * (wfar*0.5f));
	m_cptCorners[ftr].Set3DCentorid (farcenter + camxform.axis_y * (hfar*0.5f) + camxform.axis_x * (wfar*0.5f));
	m_cptCorners[fbl].Set3DCentorid (farcenter - camxform.axis_y * (hfar*0.5f) - camxform.axis_x * (wfar*0.5f));
	m_cptCorners[fbr].Set3DCentorid (farcenter - camxform.axis_y * (hfar*0.5f) + camxform.axis_x * (wfar*0.5f));

	m_cptCorners[ntl].Set3DCentorid (nearcenter + camxform.axis_y * (hnear*0.5f) - camxform.axis_x * (wnear*0.5f));
	m_cptCorners[ntr].Set3DCentorid (nearcenter + camxform.axis_y * (hnear*0.5f) + camxform.axis_x * (wnear*0.5f));
	m_cptCorners[nbl].Set3DCentorid (nearcenter - camxform.axis_y * (hnear*0.5f) - camxform.axis_x * (wnear*0.5f));
	m_cptCorners[nbr].Set3DCentorid (nearcenter - camxform.axis_y * (hnear*0.5f) + camxform.axis_x * (wnear*0.5f));


	m_cplPlanes[near_plane].ComputePlane (m_cptCorners[nbl], m_cptCorners[ntl], m_cptCorners[ntr]);
	m_cplPlanes[far_plane].ComputePlane (m_cptCorners[fbl], m_cptCorners[fbr], m_cptCorners[ftr]);
	m_cplPlanes[left_plane].ComputePlane ( m_cptCorners[fbl], m_cptCorners[ftl], m_cptCorners[nbl]);
	m_cplPlanes[right_plane].ComputePlane (m_cptCorners[fbr], m_cptCorners[nbr], m_cptCorners[ftr]);
	m_cplPlanes[top_plane].ComputePlane ( m_cptCorners[ftl], m_cptCorners[ftr], m_cptCorners[ntr]);
	m_cplPlanes[bottom_plane].ComputePlane (m_cptCorners[nbl], m_cptCorners[nbr], m_cptCorners[fbr]);
}