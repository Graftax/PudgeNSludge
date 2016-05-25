//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionLine.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Line
//////////////////////////////////////////////////////////////////////////////////////
#include "CollisionLine.h"
#include "CollisionShapes.h"
#include "../../RenderEngine/DebugShapes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionLine():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionLine::CCollisionLine ()
{
	m_btType = BT_NULL;
	m_vtType = VMT_LINE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CCollisionLine():	Default Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionLine::~CCollisionLine ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClosestPointOnA2DLine():	Takes in a vec2f Point (vPointA) and an output vec2f (ptA).
//							Finds the closets 2D point on the line then returns that point in the output pramater.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionLine::ClosestPointOnA2DLine ( vec2f vPointA, vec2f& CPA )
{	
	// calculate the vector of the line
	vec2f LineVec = m_cptEnd.Get2DCentorid() - m_cptStart.Get2DCentorid();
	// calculate the normal of the vector to find the direction its pointing
	vec2f LineNorm = LineVec;
	LineNorm.normalize();
	
	// calculate a vector from the point to the start of a line
	vec2f PointToStart = vPointA - m_cptStart.Get2DCentorid();
	
	// calculate the dot product of the line normal and the point to line start
	float fDot = dot_product(LineNorm, PointToStart);
	// if the dot product is less then 0 the start of the line is the closest point
	if(fDot < 0)
	{
		CPA = m_cptStart.Get2DCentorid();
		return;
	}
	// if the dot product is greater then the length of the line the closest point is the end of the line
	else if(fDot > abs(LineVec.magnitude()))
	{
		CPA = m_cptEnd.Get2DCentorid();
		return;
	}
	
	// calculate the closest point from the start of the line if the above if statements didnt return
	CPA = m_cptStart.Get2DCentorid() + LineNorm * fDot;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToCircle():	Takes in a Circle (cirA) and an output vec2f (ptA).
//					Checks collision between the Line and Circle and returns true
//					if they are colliding and puts the point in the output vec2f.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionLine::LineToCircle(CCollisionCircle cirA, vec2f& vlnCP )
{
	vec2f vPoint;
	// find the point on the line that is closest to the ceter of the circle
	ClosestPointOnA2DLine (cirA.GetCenter2D(), vPoint);
	
	// calculate the distance from the circles center to the point on the line
	float fDistance = dot_product ((cirA.GetCenter2D() - vPoint), (cirA.GetCenter2D() - vPoint));
	
	// if the distance is less then the cirlces radius squared
	// there was a collision and return the point on the line that was found
	if (fDistance < cirA.GetRadius() * cirA.GetRadius())
	{
		vlnCP = vPoint;
		return true;
	}
	
	return false;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClosestPointOnALine():	Takes in a vec3f Point (vPointA) and an output vec3f (ClosestPoint).
//							Finds the closets point on the line then returns that point in the output
//							pramater.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionLine::ClosestPointOnALine( vec3f vPointA, vec3f& ClosestPoint )
{
	// create a vector pointing from the start of the line to the end of the line
	vec3f LineVec = m_cptEnd.Get3DCentorid() - m_cptStart.Get3DCentorid();
	// calculate the normal for that vector
	vec3f LineNorm = LineVec;
	LineNorm.normalize();
	
	// create a vector from the start of the line to the test point
	vec3f StartToPoint = vPointA - m_cptStart.Get3DCentorid();
	
	// calculate the dot product of the line normal and the test point vector
	float fDot = dot_product(LineNorm, StartToPoint);
	
	// if the dot product is less then 0 the closest point is the line start
	if(fDot < 0)
	{
		ClosestPoint = m_cptStart.Get3DCentorid();
		return;
	}
	// else if the dot product is greater then the ABS line vectors magnitued then 
	// the closest point is the line end
	else if(fDot > abs(LineVec.magnitude()))
	{
		ClosestPoint = m_cptEnd.Get3DCentorid();
		return;
	}
	
	// scale the line normal by the dot product of the line normal and the test vector
	LineNorm *= fDot;
	
	// add the scaled line normal to the start point to get the closest point on the line
	ClosestPoint = m_cptStart.Get3DCentorid() + LineNorm;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToSphere():	Takes in a Sphere (spA) and an output vec3f (ptA).
//					Finds the cloests point on the line then returns that point in the output pramater.
//					And returns true if that point is inside the sphere.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionLine::LineToSphere( CCollisionSphere spa, vec3f& pta )
{
	vec3f vPoint = vec3f (0,0,0);

	// find the closest point on the line using the spheres center
	ClosestPointOnALine (spa.GetCenter3D(), vPoint);

	// find the squared distance between the spheres center and the closest point
	float fdistance = dot_product ((spa.GetCenter3D() - vPoint), (spa.GetCenter3D() - vPoint));

	// if the distance is less then the spheres radius squared there is a collision
	if (fdistance < spa.GetRadius() * spa.GetRadius())
	{
		// return the closest point
		pta = vPoint;
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToCapsule():	This function is broken and should not be used! 
//					Takes in a Capsule (cpA) and two output vec3f (CPA, CPB).
//					Checks collision between the line and the capsule. Returns true if they are
//					colliding and fills out the output vec3f with correct collision points. CPA is the closest point
//					on the Line and CPB is the closest point on the capsule.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionLine::LineToCapsule ( CCollisionCapsule cpa, vec3f& CPA, vec3f& cpb )
{
	vec3f vlinecp = vec3f (0,0,0);
	vec3f vcapsulecp = vec3f (0,0,0);

	// find the closest point on both lines
	//linetoline (lna, cpa.lncenter, vlinecp, vcapsulecp);

	// find the squared distance between both points
	float fdistance = dot_product (vlinecp - vcapsulecp, vlinecp - vcapsulecp);

	// if the distance is less then the squared capsules radius then there is a collision
	if (fdistance < cpa.GetRadius() * cpa.GetRadius())
	{
		// return the closest point on the line and the capsule.
		CPA = vlinecp;
		cpb = vcapsulecp;
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToBox():	Takes in a Box (box) and an output vec3f (CPA).
//				Checks collision between the line and  the box. Returns true if the line is colliding with the box and
//				fills out the output vec3f correct collision points. CPA is the closest point on the line.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionLine::LineToBox (CCollisionBox& box, vec3f& CPA)
{
	// create a line for each edge on the box in a clockwise motion.
	CCollisionLine Lines[4];
	vec2f trasformedVert;
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[0].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[3].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[1].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[3].SetStartPoint (trasformedVert);

	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[1].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[2].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[2].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[2].SetStartPoint (trasformedVert);

	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[2].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[1].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[3].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[1].SetStartPoint (trasformedVert);

	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[3].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[0].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[0].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[0].SetStartPoint (trasformedVert);

	float BestDistance = FLT_MAX;
	bool ReturnBool = false;
	// loop thorugh each edge line
	for (int i = 0; i < 4; i++)
	{
		// find the closest points on the line and a box edge.
		vec2f closestPointA, closestPointB;
		float distance = box.LineToLineClosestPt (*this, Lines[i], closestPointA, closestPointB);

		// if the distance between the two points is less then a small value then there is a collision
		if (distance <= 0.01f)
		{
			float ToStartDistance = dot_product ( (closestPointA - m_cptStart.Get2DCentorid()), (closestPointA - m_cptStart.Get2DCentorid()) );
			if (ToStartDistance < BestDistance)
			{
				BestDistance = ToStartDistance;
				// return the closest point on the box.
				CPA = vec3f ( closestPointA.x , closestPointA.y, 0.0f );
				ReturnBool = true;
			}
		}
	}

	return ReturnBool;
}

bool CCollisionLine::LineToAABB1D (float Point, float Dir, float Min, float Max, float& EnScale)
{
	if (abs (Dir) < FLT_EPSILON)
	{
		return (Point >= Min && Point <= Max);
	}

	float inter0, inter1;
	inter0 = (Min - Point) / Dir;
	inter1 = (Max - Point) / Dir;

	if (inter0 > inter1)
	{
		swap (inter0, inter1);
	}

	if (inter0 > 1.0f || inter1 < EnScale)
	{
		return false;
	}

	if (inter0 > EnScale)
	{
		EnScale = inter0;
	}

	return true;
}

bool CCollisionLine::LineToAABB(CCollisionAABB& aabb, vec3f& CPA, vec3f& CPB)
{
	vec3f CPStart = vec3f ( 0.0f, 0.0f, 0.0f );
	vec3f CPEnd = vec3f ( 0.0f, 0.0f, 0.0f );

	vec3f Dir =  m_cptEnd.Get3DCentorid() - m_cptStart.Get3DCentorid();
	//Dir.normalize();

	float EnterScale = 0.0f;

	if ( !LineToAABB1D ( m_cptStart.Get3DCentorid().x, Dir.x, aabb.GetMin3DTransformed().x, aabb.GetMax3DTransformed().x, EnterScale ) )
	{
		return false;
	}

	if ( !LineToAABB1D ( m_cptStart.Get3DCentorid().y, Dir.y, aabb.GetMin3DTransformed().y, aabb.GetMax3DTransformed().y, EnterScale ) )
	{
		return false;
	}

	CPA = m_cptStart.Get3DCentorid() + Dir * EnterScale;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToAABB():Takes in an AABB and an output vec3f (CPA).
//				Checks collision between the line and  the AABB. Returns true if the line is colliding with the AABB and
//				fills out the output vec3f correct collision points. CPA is the closest point on the line.
//
// Returns:		bool
//
// Mod. Name:	JM
// Mod. Date:	6/7/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionLine::LineToAABB(CCollisionAABB& aabb, vec3f& CPA)
{
	//make lines out of the AABB points
	CCollisionLine lines[4];
	
	lines[0].SetStartPoint(vec3f(aabb.GetMin3DTransformed().x, aabb.GetMin3DTransformed().y, 0.0f));
	lines[0].SetEndPoint(vec3f(aabb.GetMax3DTransformed().x, aabb.GetMin3DTransformed().y, 0.0f));
	
	lines[1].SetStartPoint(vec3f(aabb.GetMax3DTransformed().x, aabb.GetMin3DTransformed().y, 0.0f));
	lines[1].SetEndPoint(vec3f(aabb.GetMax3DTransformed().x, aabb.GetMax3DTransformed().y, 0.0f));
	
	lines[2].SetStartPoint(vec3f(aabb.GetMax3DTransformed().x, aabb.GetMax3DTransformed().y, 0.0f));
	lines[2].SetEndPoint(vec3f(aabb.GetMin3DTransformed().x, aabb.GetMax3DTransformed().y, 0.0f));
	
	lines[3].SetStartPoint(vec3f(aabb.GetMin3DTransformed().x, aabb.GetMax3DTransformed().y, 0.0f));
	lines[3].SetEndPoint(vec3f(aabb.GetMin3DTransformed().x, aabb.GetMin3DTransformed().y, 0.0f));
	
	//perform line segment to line segment collision for all the lines and 
	// see which point is the closest.
	float CPD = FLT_MAX;
	bool bIntersected = false;
	
	for(unsigned int i = 0; i < 4; ++i)
	{
		//vec3f thisLine = GetEndPoint3D() - GetStartPoint3D();
		//vec3f aabbLine = lines[i].GetEndPoint3D() - lines[i].GetStartPoint3D();
		//
		//float D = dot_product(thisLine, aabbLine);
		//
		//if(D == 0)
		//{
		//	return false;
		//}
		//
		//vec3f lineToAABB = lines[i].GetStartPoint3D() - GetStartPoint3D();
		//float T = (lineToAABB.x * aabbLine.y - lineToAABB.y * aabbLine.x) / D;
		//if(T < 0 || T > 1)
		//{
		//	return false;
		//}
		//
		//float U = (lineToAABB.x * aabbLine.y - lineToAABB.y * aabbLine.x) / D;
		//if(U < 0 || U > 1)
		//{
		//	return false;
		//}
		//
	
		//check for intersection
		float dist;
		vec2f cp1, cp2;
		dist = LineToLine(lines[i], cp1, cp2);
		if(dist > FLT_EPSILON)
		{
			continue;
		}
	
		//get the distance from the start of the line to the intersection.
		float lineDist = (cp1 - GetStartPoint2D()).magnitude();
	
		//if I'm closer than my predecessor, I'm now to be returned.
		if(lineDist < CPD)
		{
			CPD = lineDist;
			CPA = vec3f(cp1.x, cp1.y, 0.0f);
			bIntersected = true;
		}
	}
	
	return bIntersected;

	//vec3f c = (aabb.GetMinPoint3D() + aabb.GetMaxPoint3D() ) * 0.5f;
	//vec3f e = aabb.GetMaxPoint3D() - aabb.GetMinPoint3D();
	//vec3f d = GetEndPoint3D() - GetStartPoint3D();
	//vec3f m = (GetStartPoint3D() + GetEndPoint3D()) - (aabb.GetMinPoint3D() - aabb.GetMaxPoint3D());
	//
	//float adx = abs(d.x);
	//if( abs(m.x) > e.x + adx ) return false;
	//
	//float ady = abs(d.y);
	//if( abs(m.y) > e.y + ady ) return false;
	//
	//float adz = abs(d.z);
	//if( abs(m.z) > e.z + adz ) return false;
	//
	//adx += FLT_EPSILON;
	//ady += FLT_EPSILON;
	//adz += FLT_EPSILON;
	//
	//if( abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady ) return false;
	//if( abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx ) return false;
	//if( abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx ) return false;
	//
	//CPA = vec3f(adx, ady, adz);
	//
	//return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToAABB():Takes in an line and an output vec3f (CPA).
//				Checks collision between the line and the line. Returns true if the line is colliding with the line and
//				fills out the output vec3f correct collision points. CPA is the closest point on the line.
//
// Returns:		float
//
// Mod. Name:	JM
// Mod. Date:	6/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float CCollisionLine::LineToLine(CCollisionLine& lineIn, vec2f& c1, vec2f& c2)
{
	//float x1 = GetStartPoint2D().x, y1 = GetStartPoint2D().y; //the start point
	//float x2 = GetEndPoint2D().x, y2 = GetEndPoint2D().y; //the end point
	//float x3 = lineIn.GetStartPoint2D().x, y3 = GetStartPoint2D().y; // lineIn's start point
	//float x4 = lineIn.GetEndPoint2D().x, y4 = GetEndPoint2D().y; // lineIn's end point
	//
	//float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	//
	//// if d is 0 then there's no collision
	//if(d == 0)
	//{
	//	return false;
	//}
	//
	////get the intersect points
	//float pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
	//float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	//float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;
	//
	////check if the points are within both lines
	//if(x < std::min(x1, x2) || x > std::max(x1, x2) || 
	//	x < std::min(x3, x4) || x > std::max(x3, x4))
	//{
	//	return false;
	//}
	//
	//if(y < std::min(y1, y2) || y > std::max(y1, y2) || 
	//	y < std::min(y3, y4) || y > std::max(y3, y4))
	//{
	//	return false;
	//}
	//
	////set the out vec to the intersect point
	//CPA.x = x;
	//CPA.y = y;
	//CPA.z = 0.0f;
	//
	//
	//
	//return true;

	//set up the points
	vec2f p1 = GetStartPoint2D();
	vec2f q1 = GetEndPoint2D();
	vec2f p2 = lineIn.GetEndPoint2D();
	vec2f q2 = lineIn.GetStartPoint2D();

	//get the line between points
	vec2f d1 = q1 - p1;
	vec2f d2 = q2 - p2;

	//get the line between the start points
	vec2f r = p1 - p2;
	float t, s;

	//get the length of the lines
	float a = dot_product( d1, d1 );
	float e = dot_product( d2, d2 );

	//find the length of r in the direction of d2
	float f = dot_product( d2, r );

	//if the lengths are smaller than epsilon, set the closest points to the start points
	if( a <= FLT_EPSILON && e <= FLT_EPSILON )
	{
		s = t = 0.0f;
		c1 = p1; 
		c2 = p2;

		//return the length of the line between the points
		return dot_product( c1 - c2, c1 - c2 );
	}

	//if the first line's length is less than epsilon
	if( a <= FLT_EPSILON )
	{
		s = 0.0f;
		t = f / e;
		t = Clamp( t, 0.0f, 1.0f );
	}
	else
	{
		//find the length of r in the direction of d1
		float c = dot_product( d1, r );

		//if e is less than epsilon clamp s to -c/a, to get the intersect point
		if( e <= FLT_EPSILON )
		{
			t = 0.0f;
			s = Clamp( -c / a, 0.0f, 1.0f );
		}
		else
		{
			//find the length of d2 in the direction of d1
			float b = dot_product( d1, d2 );

			//get the denominator
			float denom = a*e - b*b;
			
			//if the denom is not 0 then clamp s to find the intersect point, otherwise s = 0
			if( denom != 0.0f )
			{
				s = Clamp( (b*f - c*e) / denom, 0.0f, 1.0f );
			}
			else
				s = 0.0f;

			t = (b*s + f) / e;

			//if t is less than 0, set it to 0 and clamp s to -c/a, otherwise, if t > 1,
			//set t to 1 and clamp s to (b-c)/a, to find the interesect point
			if( t < 0.0f )
			{
				t = 0.0f;
				s = Clamp( -c / a, 0.0f, 1.0f );
			}
			else if( t > 1.0f )
			{
				t = 1.0f;
				s = Clamp( (b - c)/a, 0.0f, 1.0f );
			}
		}
	}

	//set the closest points to the start points + their length * the intersect point
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;

	//return the length of the line between the points
	return dot_product( c1 - c2, c1 - c2 );
}