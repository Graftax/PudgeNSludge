#ifndef CollisionBox_H
#define CollisionBox_H

#include "CollisionPolygone.h"
//#include "../MathLib/vec2.h"
#include "CollisionLine.h"
#include "CollisionAABB.h"

class CCollisionSphere;

class CCollisionBox : public CCollisionPolygone
{
public:

public:

	CCollisionBox ();
	CCollisionBox ( const CCollisionBox& box );
	CCollisionBox ( CCollisionAABB& aabb );
	CCollisionBox ( CCollisionSphere& _sphere );
	~CCollisionBox ();

	CCollisionBox& operator = (CCollisionBox box);

	bool BoxToBox (CCollisionBox* boxB, vec2f* ContactPoints, unsigned int& NumCollisionPoints, vec2f& Direction);
	float LineToLineClosestPt (CCollisionLine& lnA, CCollisionLine& lnB, vec2f& vCPA, vec2f& vCPB);
	bool BoxToAABB ( CCollisionAABB aabb, vec2f& CPA, vec2f& dir);
	bool BoxToAABBWorld ( CCollisionAABB aabb, vec2f& CPA, vec2f& dir);

private:

	//bool BoxToBox (CCollisionBox* boxB, vec2f newpositions[2], vec2f* contactpoints, unsigned int& numcollisionpoints);

	float ClampFloat (float fValue, float fMin, float fMax);
	
	void ClosestPointOnALine( vec3f vPointA, CCollisionLine Line, vec3f& ClosestPoint );

public:

	vec2f Rotate (float sine, float cosine, vec2f pos, vec2f origin);

};
#endif