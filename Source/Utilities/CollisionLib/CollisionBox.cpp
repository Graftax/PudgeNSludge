#include "CollisionBox.h"
#include "CollisionShapes.h"
#include "../../RenderEngine/DebugShapes.h"
#include "CollisionLib.h"
#include "../../RenderEngine/DebugShapes.h"

CCollisionBox::CCollisionBox ()
{
	m_btType = BT_NULL;
	m_vtType = VMT_BOX;

	m_unVertCount = 4;
	m_fAngle = 0.0f;
}

CCollisionBox::~CCollisionBox ()
{

}

CCollisionBox::CCollisionBox ( const CCollisionBox& box )
{
	m_btType = box.m_btType;
	m_vtType = VMT_BOX;

	m_cptCentorid = box.m_cptCentorid;

	m_unVertCount = 4;
	m_fAngle = 0.0f;

	m_cptVertices[0] = box.m_cptVertices[0];
	m_cptVertices[1] = box.m_cptVertices[1];
	m_cptVertices[2] = box.m_cptVertices[2];
	m_cptVertices[3] = box.m_cptVertices[3];

	m_cptWorld = box.m_cptWorld;
}

CCollisionBox::CCollisionBox ( CCollisionAABB& aabb )
{
	m_btType = aabb.GetBodyType();
	m_vtType = VMT_BOX;

	m_cptCentorid.Set3DCentorid (aabb.GetCenterPoint3D());

	m_unVertCount = 4;
	m_fAngle = 0.0f;

	m_cptVertices[0].Set2DCentorid (aabb.GetMaxPoint2D());
	m_cptVertices[1].Set2DCentorid (vec2f (aabb.GetMaxPoint2D().x, aabb.GetMinPoint2D().y));
	m_cptVertices[2].Set2DCentorid (aabb.GetMinPoint2D());
	m_cptVertices[3].Set2DCentorid (vec2f (aabb.GetMinPoint2D().x, aabb.GetMaxPoint2D().y));

	m_cptWorld.Set3DCentorid (aabb.GetWorldPoint3D ());
}

CCollisionBox::CCollisionBox( CCollisionSphere& _sphere )
{
	m_btType = _sphere.GetBodyType();
	m_vtType = VMT_BOX;

	m_cptCentorid.Set3DCentorid( _sphere.GetCenter3D() );

	m_unVertCount = 4;
	m_fAngle = 0.0f;

	vec2f _fPoint = vec2f( 0.0f, 0.0f );
	_fPoint.x = _sphere.GetCenter2D().x + _sphere.GetRadius();
	_fPoint.y = _sphere.GetCenter2D().y + _sphere.GetRadius();
	m_cptVertices[0].Set2DCentorid( _fPoint);

	_fPoint = vec2f( 0.0f, 0.0f );
	_fPoint.x = _sphere.GetCenter2D().x + _sphere.GetRadius();
	_fPoint.y = _sphere.GetCenter2D().y - _sphere.GetRadius();
	m_cptVertices[1].Set2DCentorid( _fPoint );

	_fPoint = vec2f( 0.0f, 0.0f );
	_fPoint.x = _sphere.GetCenter2D().x - _sphere.GetRadius();
	_fPoint.y = _sphere.GetCenter2D().y - _sphere.GetRadius();
	m_cptVertices[2].Set2DCentorid( _fPoint );

	_fPoint = vec2f( 0.0f, 0.0f );
	_fPoint.x = _sphere.GetCenter2D().x - _sphere.GetRadius();
	_fPoint.y = _sphere.GetCenter2D().y + _sphere.GetRadius();
	m_cptVertices[3].Set2DCentorid( _fPoint );

	m_cptWorld.Set3DCentorid( _sphere.GetCenter3D() );
}

CCollisionBox& CCollisionBox::operator= (CCollisionBox box)
{
	//m_cptCentorid.Set3DCentorid ( box.GetCentorid3D());
	//m_Trans = box.GetTransform();
	//this->m_Sweep = box.GetSweep ();
	//memcpy (m_cptVertices, box.GetVertices (), sizeof (CCollisionPoint)*4);
	//memcpy (m_cptNormals, box.GetNormals (), sizeof (CCollisionPoint) * 4);

	return *this;
}

bool CCollisionBox::BoxToAABBWorld (CCollisionAABB aabb, vec2f& CPA, vec2f& dir)
{
	bool ReturnValue = false;

	vec2f trasformedVerts[4];
	trasformedVerts[0] =  Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[0].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	trasformedVerts[1] =  Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[1].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	trasformedVerts[2] =  Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[2].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	trasformedVerts[3] =  Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[3].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());

	vec2f buffer[4];
	buffer[0] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[0].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[1] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[1].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[2] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[2].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[3] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[3].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());

	vec3f CubeOne[4];
	CubeOne[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f );
	CubeOne[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f );
	CubeOne[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f );
	CubeOne[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f );

	buffer[0] = aabb.GetMaxPoint2D();
	buffer[1] = vec2f( aabb.GetMaxPoint2D().x, aabb.GetMinPoint2D().y );
	buffer[2] = aabb.GetMinPoint2D();
	buffer[3] = vec2f( aabb.GetMinPoint2D().x, aabb.GetMaxPoint2D().y );

	vec3f CubeTwo[4];
	CubeTwo[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f ); 
	CubeTwo[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f ); 
	CubeTwo[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f ); 
	CubeTwo[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f ); 

	vec3f garbage;
	vec3f fOffset;
	bool bResult = CollisionLib::CheckCubeCollision2( CubeOne, 4,  CubeTwo, 4, garbage, fOffset );
	dir = vec2f( fOffset.x, fOffset.y );
	return bResult;
}

bool CCollisionBox::BoxToAABB (CCollisionAABB aabb, vec2f& CPA, vec2f& dir)
{
	vec2f buffer[4];
	buffer[0] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[0].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[1] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[1].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[2] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[2].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[3] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[3].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());

	vec3f CubeOne[4];
	CubeOne[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f );
	CubeOne[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f );
	CubeOne[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f );
	CubeOne[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f );

	buffer[0] = aabb.GetMaxPoint2D() + aabb.GetWorldPoint2D();
	buffer[1] = vec2f( aabb.GetMaxPoint2D().x, aabb.GetMinPoint2D().y ) + aabb.GetWorldPoint2D();
	buffer[2] = aabb.GetMinPoint2D() + aabb.GetWorldPoint2D();
	buffer[3] = vec2f( aabb.GetMinPoint2D().x, aabb.GetMaxPoint2D().y ) + aabb.GetWorldPoint2D();

	vec3f CubeTwo[4];
	CubeTwo[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f ); 
	CubeTwo[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f ); 
	CubeTwo[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f ); 
	CubeTwo[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f ); 

	vec3f garbage;
	vec3f fOffset;
	bool bResult = CollisionLib::CheckCubeCollision2( CubeOne, 4,  CubeTwo, 4, garbage, fOffset );
	dir = vec2f( fOffset.x, fOffset.y );
	return bResult;
}

vec2f CCollisionBox::Rotate (float sine, float cosine, vec2f pos, vec2f origin)
{
	vec2f retVec;
	retVec.x = cosine * (pos.x - origin.x) - sine * (pos.y - origin.y) + origin.x;
	retVec.y = sine * (pos.x - origin.x) + cosine * (pos.y - origin.y) + origin.y;

	//p'x = cos(theta) * (px-ox) - sin(theta) * (py-oy) + ox
	//p'y = sin(theta) * (px-ox) + cos(theta) * (py-oy) + oy

	return retVec;
}

bool CCollisionBox::BoxToBox (CCollisionBox* boxB, vec2f* ContactPoints, unsigned int& NumCollisionPoints, vec2f& Direction)
{
	NumCollisionPoints = 0;

	vec2f buffer[4];
	buffer[0] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[0].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[1] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[1].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[2] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[2].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());
	buffer[3] = Rotate ((float)sin(m_fAngle), (float)cos(m_fAngle), m_cptVertices[3].Get2DCentorid() + m_cptWorld.Get2DCentorid(), m_cptWorld.Get2DCentorid());

	vec3f CubeOne[4];
	CubeOne[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f );
	CubeOne[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f );
	CubeOne[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f );
	CubeOne[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f );

	buffer[0] = Rotate ((float)sin(boxB->GetAngle()), (float)cos(boxB->GetAngle()), boxB->GetVertices()[0].Get2DCentorid() +  boxB->GetWorld().Get2DCentorid(), boxB->GetWorld().Get2DCentorid());
	buffer[1] = Rotate ((float)sin(boxB->GetAngle()), (float)cos(boxB->GetAngle()), boxB->GetVertices()[1].Get2DCentorid() +  boxB->GetWorld().Get2DCentorid(), boxB->GetWorld().Get2DCentorid());
	buffer[2] = Rotate ((float)sin(boxB->GetAngle()), (float)cos(boxB->GetAngle()), boxB->GetVertices()[2].Get2DCentorid() +  boxB->GetWorld().Get2DCentorid(), boxB->GetWorld().Get2DCentorid());
	buffer[3] = Rotate ((float)sin(boxB->GetAngle()), (float)cos(boxB->GetAngle()), boxB->GetVertices()[3].Get2DCentorid() +  boxB->GetWorld().Get2DCentorid(), boxB->GetWorld().Get2DCentorid());

	vec3f CubeTwo[4];
	CubeTwo[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f ); 
	CubeTwo[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f ); 
	CubeTwo[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f ); 
	CubeTwo[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f ); 

	vec3f garbage;
	vec3f fOffset;
	bool bResult = CollisionLib::CheckCubeCollision2( CubeOne, 4,  CubeTwo, 4, garbage, fOffset );
	Direction = vec2f( fOffset.x, fOffset.y );
	return bResult;
}

void CCollisionBox::ClosestPointOnALine( vec3f vPointA, CCollisionLine Line, vec3f& ClosestPoint )
{
	// create a vector pointing from the start of the line to the end of the line
	vec3f LineVec = Line.GetEndPoint3D() - Line.GetStartPoint3D();
	// calculate the normal for that vector
	vec3f LineNorm = LineVec;
	LineNorm.normalize();

	// create a vector from the start of the line to the test point
	vec3f StartToPoint = vPointA - Line.GetStartPoint3D();

	// calculate the dot product of the line normal and the test point vector
	float fDot = dot_product(LineNorm, StartToPoint);

	// if the dot product is less then 0 the closest point is the line start
	if(fDot < 0)
	{
		ClosestPoint = Line.GetStartPoint3D();
		return;
	}
	// else if the dot product is greater then the ABS line vectors magnitued then 
	// the closest point is the line end
	else if(fDot > abs(LineVec.magnitude()))
	{
		ClosestPoint = Line.GetEndPoint3D();
		return;
	}

	// scale the line normal by the dot product of the line normal and the test vector
	LineNorm *= fDot;

	// add the scaled line normal to the start point to get the closest point on the line
	ClosestPoint = Line.GetStartPoint3D() + LineNorm;
}

float CCollisionBox::LineToLineClosestPt (CCollisionLine& lnA, CCollisionLine& lnB, vec2f& vCPA, vec2f& vCPB)
{
	float fScaleA = 0.0f;
	float fScaleB = 0.0f;
	vec2f vDirectionA = lnA.GetStartPoint2D() - lnA.GetEndPoint2D();
	vec2f vDirectionB = lnB.GetEndPoint2D() - lnB.GetStartPoint2D();
	vec2f vAToB = lnA.GetEndPoint2D() - lnB.GetStartPoint2D();
	float fCapDistance = dot_product( vDirectionA, vDirectionA );
	float fTriDistance = dot_product( vDirectionB, vDirectionB );
	float fTriToCapDistance = dot_product( vDirectionB, vAToB );

	if( fCapDistance <= sqrt(FLT_EPSILON) && fTriDistance <= sqrt(FLT_EPSILON) )
	{
		fScaleA = fScaleB = 0.0f;
		vCPA = lnA.GetStartPoint2D(); 
		vCPB = lnB.GetStartPoint2D();
		return dot_product( vCPA - vCPB, vCPA - vCPB );
	}

	if( fCapDistance <= sqrt(FLT_EPSILON) )
	{
		fScaleA = 0.0f;
		fScaleB = fTriToCapDistance / fTriDistance;
		fScaleB = ClampFloat( fScaleB, 0.0f, 1.0f );
	}
	else
	{
		float c = dot_product( vDirectionA, vAToB );
		if( fTriDistance <= sqrt(FLT_EPSILON) )
		{
			fScaleB = 0.0f;
			fScaleA = ClampFloat( -c / fCapDistance, 0.0f, 1.0f );
		}
		else
		{
			float b = dot_product( vDirectionA, vDirectionB );
			float denom = fCapDistance*fTriDistance - b*b;

			if( denom != 0.0f )
			{
				fScaleA = ClampFloat( (b*fTriToCapDistance - c*fTriDistance) / denom, 0.0f, 1.0f );
			}
			else
				fScaleA = 0.0f;

			fScaleB = (b*fScaleA + fTriToCapDistance) / fTriDistance;

			if( fScaleB < 0.0f )
			{
				fScaleB = 0.0f;
				fScaleA = ClampFloat( -c / fCapDistance, 0.0f, 1.0f );
			}
			else if( fScaleB > 1.0f )
			{
				fScaleB = 1.0f;
				fScaleA = ClampFloat( (b - c)/fCapDistance, 0.0f, 1.0f );
			}
		}
	}

	vCPA = lnA.GetEndPoint2D() + vDirectionA * fScaleA;
	vCPB = lnB.GetStartPoint2D() + vDirectionB * fScaleB;
	return dot_product( vCPA - vCPB, vCPA - vCPB );
}

float CCollisionBox::ClampFloat (float fValue, float fMin, float fMax)
{
	if (fValue < fMin)
	{
		return fMin;
	}
	if (fValue > fMax)
	{
		return fMax;
	}
	return fValue;
}