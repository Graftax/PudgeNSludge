#ifndef CollisionFrustum_H
#define CollisionFrustum_H

#include "CollisionVolume.h"
#include "CollisionPoint.h"
#include "CollisionPlane.h"
#include "CollisionSphere.h"
#include "CollisionCapsule.h"
#include "../MathLib/matrix4.h"

//class CCollisionSphere;
//class CCollisionCapsule;
//class CCollisionPlane;

class CCollisionFrustum : public CCollisionVolume
{
public:
	enum frustumcorners{ ftl = 0, fbl, fbr, ftr, ntl, ntr, nbr, nbl };
	enum frustumplanes{ near_plane = 0, far_plane, left_plane, right_plane, top_plane, bottom_plane };

private:
	CCollisionPlane* m_cplPlanes;
	CCollisionPoint* m_cptCorners;

	unsigned int m_unMaxPlanes;
	unsigned int m_unMaxCorners;

public:
	CCollisionFrustum ();
	~CCollisionFrustum ();

	CCollisionPlane* GetPlanes () { return m_cplPlanes; }
	void SetPlanes ( CCollisionPlane* _cplPlanes ) { m_cplPlanes = _cplPlanes; }

	CCollisionPoint* GetCorners () { return m_cptCorners; }
	void SetCorners ( CCollisionPoint* _cptCorners ) { m_cptCorners = _cptCorners; }

	CCollisionPlane* GetPlane ( unsigned int _unIndex ) { return _unIndex < m_unMaxPlanes ? &m_cplPlanes[_unIndex] : 0; }
	void SetPlane ( CCollisionPlane _cplPlane, unsigned int _unIndex ) { _unIndex < m_unMaxPlanes ? m_cplPlanes[_unIndex] = _cplPlane : m_cplPlanes [0]; }

	CCollisionPoint* GetCorner ( unsigned int _unIndex ) { return _unIndex < m_unMaxCorners ? &m_cptCorners[_unIndex] : 0; }
	void SetCorner ( CCollisionPoint _cptCorner, unsigned int _unIndex ) { _unIndex < m_unMaxCorners ? m_cptCorners[_unIndex] = _cptCorner : m_cptCorners [0]; }

	bool infrustum( CCollisionSphere spa );
	bool infrustum( CCollisionCapsule cpa );
	bool infrustum( CCollisionAABB aabb );

	void Build (float fov, float neardist, float fardist, float ratio, const matrix4f camxform );
};
#endif