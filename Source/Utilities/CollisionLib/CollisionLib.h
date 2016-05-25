#ifndef collisionlib_h_
#define collisionlib_h_

#include "../../ps/config.h"

#include "../mathlib/vec3.h"
#include "../mathlib/vec2.h"
#include "../mathlib/matrix4.h"
#include "collisionshapes.h"

//#include "../../objectlib/collidableobject.h"

#include "../kdtree/kdtree.h"
#include "../kdtree/kdtreenodes.h"
#include "../../RenderEngine/DebugShapes.h"


///////////////////////////////////////////////////////////////////////////////////
// namespace name: collisionlib
// purpose: a namespace which holds all basic and advance collision checks
//			(this does not handle collision just checks it)
// original author: josh fields
// creation date: 2/6/2012
// last modification by:	charles breakwell
// last modification date:	2/9/2012
///////////////////////////////////////////////////////////////////////////////////
namespace CollisionLib
{




#pragma region halfspacetests

	///////////////////////////////////////////////////////////////////////////////////
	// function name: positivehalfspacetest - done
	// purpose:	tests if a point is on the positive side or on the of a triangle
	// original author: josh fields
	// creation date: 2/7/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	//static bool positivehalfspacetest ( vec3f vpoint, ttriangle tra )
	//{
	//	// calculate the distance the point is from the triangle
	//	float fdistance = dot_product ((vpoint - tra.vverts[0]), tra.vnormal);
	//	// if the distance is less then 0 the point is on the negative side of the triangle
	//	if (fdistance < 0.0f)
	//	{
	//		return false;
	//	}
	//	return true;
	//}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: positivehalfspacetest - done
	// purpose:	takes in a point on the plane (vvert), the normal for the plane (vnormal)
	//			and a test point (vpoint). tests if a point is on the positive side of a plane.
	// original author: josh fields
	// creation date: 2/7/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static bool positivehalfspacetest (vec3f vvert, vec3f vnormal, vec3f vpoint)
	{
		// calculate a vector from the test point to the point on the plane
		vec3f vpointtoplane = vpoint - vvert;
		// calculate the distance the point is from the plane
		float fdistance = dot_product (vnormal, vpointtoplane);
		// if the distance is less then 0 the point is on the negative side of the plane
		if (fdistance < 0.0f)
		{
			return false;
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: positivehalfspacetest - done
	// purpose:	tests if a point is on the negative side of a triangle
	// original author: josh fields
	// creation date: 2/7/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	//static bool negativehalfspacetest ( vec3f vpointa, ttriangle tra )
	//{
	//	// calculate the distance the point is from the triangle
	//	float fdistance = dot_product ((vpointa - tra.vverts[0]), tra.vnormal);
	//	// if the distance is less then 0 the point is on the negative side of the triangle
	//	if (fdistance < 0.0f)
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: negativehalfspacetest - done
	// purpose:	takes in a point on the plane (vvert), the normal for the plane (vnormal)
	//			and a test point (vpoint). tests if a point is on the negative side of a plane.
	// original author: josh fields
	// creation date: 2/7/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static bool negativehalfspacetest (vec3f vvert, vec3f vnormal, vec3f vpoint)
	{
		// calculate a vector from the test point to the point on the plane
		vec3f vpointtoplane = vpoint - vvert;
		// calculate the distance the point is from the plane
		float fdistance = dot_product (vnormal, vpointtoplane);
		// if the distance is less then 0 the point is on the negative side of the plane
		if (fdistance < 0.0f)
		{
			return true;
		}
		return false;
	}

#pragma endregion


#pragma region threedcollisiontests


	///////////////////////////////////////////////////////////////////////////////////
	// function name: linetoline - this does not work and needs to  be changed
	// purpose: takes in two lines (lna, lnb) and two output vector3f (lnacp, lnbcp).
	//			finds the closest points on both lines and fills out the each output
	//			vector3f with the correct collision point.
	// original author: josh fields
	// creation date: 2/4/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	//static void linetoline( tline lna, tline lnb, vec3f& cpa, vec3f& cpb )
	//{
	//	vec3f vbcp1;
	//	vec3f vbcp2;

	//	// find the closest point on lineb using linea's start point
	//	closestpointonaline ( lna.vstart, lnb, vbcp1);
	//	// find the closest point on lineb using linea's end point
	//	closestpointonaline ( lna.vend, lnb, vbcp2);

	//	// calculate the distance from the first closest point and linea's start point
	//	float fdistance1 = dot_product (vbcp1 - lna.vstart, vbcp1 - lna.vstart);
	//	// calculate the distance from the second closest point and linea's end point
	//	float fdistance2 = dot_product (vbcp2 - lna.vend, vbcp2 - lna.vend);

	//	// if the distance from the first closest point and linea's start point is closer
	//	// then use the first closest point found else use the second.
	//	if (fdistance1 < fdistance2)
	//	{
	//		cpb = vbcp1;
	//	}
	//	else
	//	{
	//		cpb = vbcp2;
	//	}

	//	// find the closest point on linea using the temp closest point on lineb
	//	closestpointonaline ( cpb, lna, cpa);
	//	// find the closest point on lineb using the closest point on linea
	//	closestpointonaline ( cpa, lnb, cpb);
	//}



	/////////////////////////////////////////////////////////////////////////////////////
	//// function name: linetotriangle - done
	//// purpose: takes in a line and triangle (lna, tra) and an output vector3f (vcp).
	////			checks collision between the line and triangle and returns true
	////			if they are colliding and puts the collision point in the output vector3f.
	//// original author: josh fields
	//// creation date: 2/7/2012
	//// last modification by:	
	//// last modification date:	
	/////////////////////////////////////////////////////////////////////////////////////
	//static bool linetotriangle (tline lna, ttriangle tra, vec3f& vcp)
	//{
	//	// this is the same as the raytotriangle from ed2 it is just using a line for the ray and clamping 
	//	// the closest point between the start and end of the line
	//	tray rya;
	//	rya.vstart = lna.vstart;
	//	rya.vnormal = lna.vend - lna.vstart;

	//	vec3f vnormala = rya.vstart - tra.vverts[0];
	//	vec3f vnormalb = rya.vstart - tra.vverts[1];
	//	vec3f vnormalc = rya.vstart - tra.vverts[2];

	//	vec3f n1, n2, n3;
	//	cross_product (n1, vnormalb, vnormalc);
	//	cross_product (n2, vnormalc, vnormala);
	//	cross_product (n3, vnormala, vnormalb);

	//	float fdistance0 = dot_product (rya.vnormal, n1);
	//	float fdistance1 = dot_product (rya.vnormal, n2);
	//	float fdistance2 = dot_product (rya.vnormal, n3);

	//	if (abs(fdistance0) <= flt_epsilon && abs(fdistance1) <= flt_epsilon && abs(fdistance2) <= flt_epsilon)
	//	{
	//		vcp = rya.vstart;
	//		return true;
	//	}

	//	bool samesign1 = false, samesign2 = false;

	//	if (samesign (fdistance0, fdistance1) || (fdistance0 == 0) || (fdistance1 == 0))
	//		samesign1 = true;
	//	if (samesign (fdistance1, fdistance2) || (fdistance1 == 0) || (fdistance2 == 0))
	//		samesign2 = true;

	//	if (samesign1 && samesign2)
	//	{
	//		float planeoffset = dot_product (tra.vverts[0], tra.vnormal);
	//		float ftime = (planeoffset - dot_product (rya.vstart, tra.vnormal)) / dot_product (tra.vnormal, rya.vnormal);

	//		if (ftime < 0.0f)
	//		{
	//			return false;
	//		}

	//		vcp = rya.vstart + rya.vnormal * ftime;

	//		if (dot_product (vcp - lna.vend, vcp - lna.vend) > 0.0f)
	//		{

	//		}

	//		return true;
	//	}

	//	return false;
	//}






	/////////////////////////////////////////////////////////////////////////////////////
	//// function name: raytotriangle - done
	//// purpose: takes in a ray and triangle (rya, tra) and an output vector3f (vcp).
	////			checks collision between the ray and triangle and returns true
	////			if they are colliding and puts the collision point in the output vector3f.
	//// original author: josh fields
	//// creation date: 2/7/2012
	//// last modification by:	
	//// last modification date:	
	/////////////////////////////////////////////////////////////////////////////////////
	//static bool raytotriangle (tray rya, ttriangle tra, vec3f& vcp)
	//{
	//	// this came form the ed2 collision library
	//	vec3f vnormala = rya.vstart - tra.vverts[0];
	//	vec3f vnormalb = rya.vstart - tra.vverts[1];
	//	vec3f vnormalc = rya.vstart - tra.vverts[2];

	//	vec3f n1, n2, n3;
	//	cross_product (n1, vnormalb, vnormalc);
	//	cross_product (n2, vnormalc, vnormala);
	//	cross_product (n3, vnormala, vnormalb);

	//	float fdistance0 = dot_product (rya.vnormal, n1);
	//	float fdistance1 = dot_product (rya.vnormal, n2);
	//	float fdistance2 = dot_product (rya.vnormal, n3);

	//	if (abs(fdistance0) <= flt_epsilon && abs(fdistance1) <= flt_epsilon && abs(fdistance2) <= flt_epsilon)
	//	{
	//		vcp = rya.vstart;
	//		return true;
	//	}

	//	bool samesign1 = false, samesign2 = false;

	//	if (samesign (fdistance0, fdistance1) || (fdistance0 == 0) || (fdistance1 == 0))
	//		samesign1 = true;
	//	if (samesign (fdistance1, fdistance2) || (fdistance1 == 0) || (fdistance2 == 0))
	//		samesign2 = true;

	//	if (samesign1 && samesign2)
	//	{
	//		float planeoffset = dot_product (tra.vverts[0], tra.vnormal);
	//		float ftime = (planeoffset - dot_product (rya.vstart, tra.vnormal)) / dot_product (tra.vnormal, rya.vnormal);

	//		if (ftime < 0.0f)
	//		{
	//			return false;
	//		}
	//		vcp = rya.vstart + rya.vnormal * ftime;
	//		return true;
	//	}

	//	return false;
	//}








	///////////////////////////////////////////////////////////////////////////////////
	// function name: pointtotriangle
	// purpose: takes in a point and a triangle (vpoint, tra) and an output vector3f (cpa).
	//			this function will test if the point is colliding with a triangle and 
	//			will return true if it is and will place the closest point of collision
	//			in the output vector3f.
	// original author: josh fields
	// creation date: 2/29/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	//static bool pointtotriangle (vec3f vpoint, ttriangle tra, vec3f& cpa)
	//{
	//	// calculate the test point onto the triangles plane
	//	vec3f projectedpt = vpoint - (tra.vnormal * (dot_product (vpoint - tra.vverts[0], tra.vnormal)));
	//
	//	// calculate all the triangle edges (this is most likely in ogl needs to be dx)
	//	vec3f edges[3];
	//	edges[0] = tra.vverts[1] - tra.vverts[0];
	//	edges[1] = tra.vverts[2] - tra.vverts[1];
	//	edges[2] = tra.vverts[0] - tra.vverts[2];
	//
	//	// calculate all the triangle's edge's normal (this should be moved to the load so it only happens once since we dont have deforming terrain)
	//	vec3f normals[3];
	//	cross_product (normals[0], edges[0], tra.vnormal);
	//	cross_product (normals[1], edges[1], tra.vnormal);
	//	cross_product (normals[2], edges[2], tra.vnormal);
	//
	//	// test is the projected point is on the positive side of any of the triangles edges
	//	// meaning that the closest point is on the edge and not inside the triangle
	//	vec3f closestpoint = projectedpt;
	//	if (positivehalfspacetest (tra.vverts[0],normals[0], projectedpt))
	//	{
	//		tline line;
	//		line.vstart = tra.vverts[0];
	//		line.vend = tra.vverts[1];
	//		
	//		closestpointonaline (projectedpt, line, closestpoint);
	//
	//	} else if (positivehalfspacetest (tra.vverts[1],normals[1], projectedpt))
	//	{
	//		tline line;
	//		line.vstart = tra.vverts[1];
	//		line.vend = tra.vverts[2];
	//		
	//		closestpointonaline (projectedpt, line, closestpoint);
	//
	//	} else if (positivehalfspacetest (tra.vverts[2],normals[2], projectedpt))
	//	{
	//		tline line;
	//		line.vstart = tra.vverts[2];
	//		line.vend = tra.vverts[0];
	//		
	//		closestpointonaline (projectedpt, line, closestpoint);
	//	}
	//
	//	// if the distance from the closest point and the test point is less then 0
	//	// then there is a collsion and return the closest point
	//	if (dot_product (closestpoint - vpoint, closestpoint - vpoint) < 0.0f)
	//	{
	//		cpa = closestpoint;
	//		return true;
	//	}
	//
	//	return false;
	//}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: spheretotriangle
	// purpose: takes in a sphere and a triangle (spa, tra) and an output vector3f (cpa).
	//			this function will test if the sphere is colliding with a triangle and 
	//			will return true if it is and will place the closest point of collision
	//			in the output vector3f.
	// original author: josh fields
	// creation date: 2/29/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	//static bool spheretotriangle (tsphere spa, ttriangle tra, vec3f& cpa)
	//{
	//	// calculate the sphere's center onto the triangles plane
	//	vec3f projectedpt = spa.vcenter - (tra.vnormal * (dot_product (spa.vcenter - tra.vverts[0], tra.vnormal)));
	//
	//	// calculate all the triangle edges (this is most likely in ogl needs to be dx)
	//	vec3f edges[3];
	//	edges[0] = tra.vverts[1] - tra.vverts[0];
	//	edges[1] = tra.vverts[2] - tra.vverts[1];
	//	edges[2] = tra.vverts[0] - tra.vverts[2];
	//
	//	// calculate all the triangle's edge's normal (this should be moved to the load so it only happens once since we dont have deforming terrain)
	//	vec3f normals[3];
	//	cross_product (normals[0], edges[0], tra.vnormal);
	//	cross_product (normals[1], edges[1], tra.vnormal);
	//	cross_product (normals[2], edges[2], tra.vnormal);
	//
	//	vec3f closestpoint = projectedpt;
	//	// test is the projected point is on the positive side of any of the triangles edges
	//	// meaning that the closest point is on the edge and not inside the triangle
	//	if (positivehalfspacetest (tra.vverts[0],normals[0], projectedpt))
	//	{
	//		tline line;
	//		line.vstart = tra.vverts[0];
	//		line.vend = tra.vverts[1];
	//		
	//		closestpointonaline (projectedpt, line, closestpoint);
	//
	//	} else if (positivehalfspacetest (tra.vverts[1],normals[1], projectedpt))
	//	{
	//		tline line;
	//		line.vstart = tra.vverts[1];
	//		line.vend = tra.vverts[2];
	//		
	//		closestpointonaline (projectedpt, line, closestpoint);
	//
	//	} else if (positivehalfspacetest (tra.vverts[2],normals[2], projectedpt))
	//	{
	//		tline line;
	//		line.vstart = tra.vverts[2];
	//		line.vend = tra.vverts[0];
	//		
	//		closestpointonaline (projectedpt, line, closestpoint);
	//	}
	//
	//	// if the distance from the closest point and the sphere's center is less then the sphere's radius squared
	//	// then there is a collsion and return the closest point
	//	if (dot_product (closestpoint - spa.vcenter, closestpoint - spa.vcenter) < spa.fradius * spa.fradius)
	//	{
	//		cpa = closestpoint;
	//		return true;
	//	}
	//	return false;
	//}



	///////////////////////////////////////////////////////////////////////////////////
	// function name: linesegmenttotriangle
	// purpose: takes in a vector<csceneobject*> and a line (vtris, lna) and three output
	//			vector3f, unsigned int, and a vector<csceneobject*> (vpoint, triindex, walltris).
	//			will loop through vtris (must be a vector of sceneobjects with triangle bounding
	//			volumes) and checks which triangle the line is passing through and gives back
	//			the point of collision, and the triangle index into vtris. this function will also
	//			fill out walltris with triangles whos normals y value is less then a 45 degree
	//			angle and is greater then 0.
	// original author: josh fields
	// creation date: 2/29/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	/*static bool linesegmenttotriangle(std::vector <csceneobject*>& vtris, tline lna, vec3f& vpoint, unsigned int& triindex, std::vector<csceneobject*>& walltris)
	{
	// return false if there are no triangles to test
	if (vtris.size() < 1)
	{
	return false;
	}

	vec3f voldend = lna.vend;
	// loop through all triangles passed in
	for (size_t untriindex = 0; untriindex < vtris.size(); untriindex++)
	{
	ttriangle* tra = (ttriangle*)(vtris[untriindex]->getcollidableobject()->getboundingvolume(0));
	vec3f vtrinormal = tra->vnormal;

	// check if line start is below the first vert
	if (dot_product (lna.vstart, vtrinormal) - dot_product (tra->vverts[0], vtrinormal) < 0) 
	{
	if (vtrinormal.y <  0.707f && vtrinormal.y >= 0.0f)
	{
	walltris.push_back(vtris[untriindex]);
	}

	continue;
	}
	// check if the line end is above the first vert
	if (dot_product (voldend, vtrinormal) - dot_product (tra->vverts[0], vtrinormal) > 0)
	{
	if (vtrinormal.y <  0.707f && vtrinormal.y >= 0.0f)
	{
	walltris.push_back(vtris[untriindex]);
	}
	continue;
	}

	// calculate the distance the line start from the origin in the direction of the triangle normal
	float d0 = dot_product (lna.vstart, vtrinormal);
	// calculate the distacne the first triangle vert is from the origin in the direction of the triangle normal
	float d1 = dot_product (tra->vverts[0], vtrinormal);
	// calculate the difference of the two distances
	float d2 = d0 - d1;

	// create the line's vector
	vec3f line = voldend - lna.vstart;
	// calculate the distance of the line vector in the direction of the triangle normal
	float d3 = dot_product (vtrinormal, line);

	// if the length of the line is 0 dont use it
	if (d3 == 0.0f)
	{
	continue;
	}

	// calculate the overall distance by dividing the distance calculated earlier by the line distance and negate it 
	float df = -(d2 / d3);

	// calculate the closest point on the line by adding the line start point to the line vector times the overall distance
	vec3f cp = lna.vstart + line * df;

	// calculate all 3 edges for the triangle
	vec3f edges[3];

	edges[0] = tra->vverts[1] - tra->vverts[0];
	edges[1] = tra->vverts[2] - tra->vverts[1];
	edges[2] = tra->vverts[0] - tra->vverts[2];

	// calculate the normals for all 3 edges
	vec3f vnormals[3];
	cross_product (vnormals[0], edges[0], vtrinormal);
	cross_product (vnormals[1], edges[1], vtrinormal);
	cross_product (vnormals[2], edges[2], vtrinormal);

	vnormals[0].normalize();
	vnormals[1].normalize();
	vnormals[2].normalize();

	// loop through all 3 edges and check if they are on the positive side of the triangle
	int edgeindex = 0;
	for (; edgeindex < 3; edgeindex++)
	{
	vec3f v = cp - tra->vverts[edgeindex];
	float d = dot_product (vnormals[edgeindex], v);
	// if the distance from the edge is greater then 0 then the closest point is on the positive side
	// of the edge and we need to leave
	if (d > 0.0f)
	{
	break;
	}
	}

	// if we left before all 3 edges were checked we should continue
	if (edgeindex < 3)
	{
	continue;
	}

	// reset the end of the line to the new closest point
	voldend = cp;
	// this should be deleted unless we really need to know which triangle we collided with
	triindex = untriindex;
	}

	// if the temp end point is at the same spot as the lines end point then nothing happened and we return false
	if (voldend.x == lna.vend.x &&
	voldend.y == lna.vend.y &&
	voldend.z == lna.vend.z)
	return false;

	// else we return true and return the new end point
	vpoint = voldend;
	return true;
	}*/

#pragma endregion


	// these tests are for the kd tree and probably will get moved or maybe not
#pragma region aaplanetests

	///////////////////////////////////////////////////////////////////////////////////
	// function name: aabbtoaaplane - done
	// purpose: takes in an aabb, an integer and a float (aabb, naxis, fplaneoffset). returns 
	//			a short based on where the aabb is on the aaplane.
	// original author: josh fields
	// creation date: 2/6/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static unsigned short AABBToAAPlane (AABB aabb, int nAxis, float fPlaneOffset)
	{
		if (aabb.GetMax3DTransformed().v[nAxis] < fPlaneOffset)
		{
			return 0;
		}

		if (aabb.GetMin3DTransformed().v[nAxis] > fPlaneOffset)
		{
			return 1;
		}

		return 2;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: BoxToAAPlane - done
	// purpose: takes in an Box, an integer and a float (box, naxis, fplaneoffset). returns 
	//			a short based on where the Box is on the aaplane.
	// original author: josh fields
	// creation date: 2/6/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static unsigned short BoxToAAPlane (Box box, int nAxis, float fPlaneOffset)
	{
		vec2f Transform2D;
		vec3f Transform3D[4];

		Transform2D = box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[0].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
		Transform3D[0] = vec3f (Transform2D.x, Transform2D.y, 0.0f);
		Transform2D = box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[1].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
		Transform3D[1] = vec3f (Transform2D.x, Transform2D.y, 0.0f);
		Transform2D = box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[2].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
		Transform3D[2] = vec3f (Transform2D.x, Transform2D.y, 0.0f);
		Transform2D = box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[3].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
		Transform3D[3] = vec3f (Transform2D.x, Transform2D.y, 0.0f);

		if (Transform3D[0].v[nAxis] < fPlaneOffset &&
			Transform3D[1].v[nAxis] < fPlaneOffset &&
			Transform3D[2].v[nAxis] < fPlaneOffset &&
			Transform3D[3].v[nAxis] < fPlaneOffset )
		{
			return 0;
		}

		if (Transform3D[0].v[nAxis] > fPlaneOffset &&
			Transform3D[1].v[nAxis] > fPlaneOffset &&
			Transform3D[2].v[nAxis] > fPlaneOffset &&
			Transform3D[3].v[nAxis] > fPlaneOffset )
		{
			return 1;
		}

		return 2;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: spheretoaaplane
	// purpose: takes in an sphere, an integer and a float (sphere, naxis, fplaneoffset). returns 
	//			a short based on where the sphere is on the aaplane.
	// original author: josh fields
	// creation date: 2/6/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static unsigned short SphereToAAPlane (Sphere& sphere, int nAxis, float fPlaneOffset)
	{
		if (sphere.GetCenter3D().v[nAxis] + sphere.GetRadius() < fPlaneOffset)
		{
			return 0;
		}

		if (sphere.GetCenter3D().v[nAxis] - sphere.GetRadius() > fPlaneOffset)
		{
			return 1;
		}

		return 2;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: capsuletoaaplane
	// purpose: takes in an capsule, an integer and a float(capsule, naxis, fplaneoffset). returns 
	//			a short based on where the capsule is on the aaplane.
	// original author: josh fields
	// creation date: 2/6/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static unsigned short CapsuleToAAPlane (Capsule& capsule, int nAxis, float fPlaneOffset)
	{
		if (capsule.GetCenterLineStart3D().v[nAxis] + capsule.GetRadius() < fPlaneOffset &&
			capsule.GetCenterLineEnd3D().v[nAxis] + capsule.GetRadius() < fPlaneOffset)
		{
			return 0;
		}

		if (capsule.GetCenterLineStart3D().v[nAxis] - capsule.GetRadius() > fPlaneOffset &&
			capsule.GetCenterLineEnd3D().v[nAxis] - capsule.GetRadius() > fPlaneOffset)
		{
			return 1;
		}

		return 2;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: pointtoaaplane
	// purpose: takes in an point, an integer and a float(vpoint, naxis, fplaneoffset). returns 
	//			a short based on where the point is on the aaplane.
	// original author: josh fields
	// creation date: 2/6/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static unsigned short PointToAAPlane (Point& vPoint, int nAxis, float fPlaneOffset)
	{
		if (vPoint.Get3DCentorid().v[nAxis] < fPlaneOffset)
		{
			return 0;
		}

		if (vPoint.Get3DCentorid().v[nAxis] > fPlaneOffset)
		{
			return 1;
		}

		return 2;
	}

	///////////////////////////////////////////////////////////////////////////////////
	// function name: linetoaaplane
	// purpose: takes in an line, an integer and a float(lna, naxis, fplaneoffset). returns 
	//			a short based on where the point is on the aaplane.
	// original author: josh fields
	// creation date: 2/6/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	static unsigned short LineToAAPlane (Line& lna, int nAxis, float fPlaneOffset)
	{
		if (lna.GetStartPoint3D().v[nAxis] < fPlaneOffset &&
			lna.GetEndPoint3D().v[nAxis] < fPlaneOffset)
		{
			return 0;
		}

		if (lna.GetStartPoint3D().v[nAxis] > fPlaneOffset &&
			lna.GetEndPoint3D().v[nAxis] > fPlaneOffset)
		{
			return 1;
		}

		return 2;
	}
#pragma endregion


	///////////////////////////////////////////////////////////////////////////////////
	// function name: clampspheretoground
	// purpose: takes in a sceneobject (pobject) and an output vector3f and an unsigned
	//			int (voutpos, triindex). this function will clamp the passed in sceneobject
	//			to the ground and push it out of any walls that are near the object.
	// original author: josh fields
	// creation date: 2/29/2012
	// last modification by:	
	// last modification date:	
	///////////////////////////////////////////////////////////////////////////////////
	/*static bool clampspheretoground (csceneobject& pobject, vec3f& voutpos, unsigned int& triindex)
	{
	// make sure the object coming in has a bounding volume of a sphere
	if (pobject.getcollidableobject()->getboundingvolumetype() != bvt_sphere)
	{
	return false;
	}	

	// create a temp sphere to hold the objects bv
	tsphere spa = *(tsphere*)pobject.getcollidableobject()->getboundingvolume(0);

	// look in the kd tree for triangles around the object passed in
	short sreturnpram = (1<<rflag_triangle);
	std::vector <csceneobject*> vtris;
	ckdtree::getnearobjects (&pobject, psflag_sphere, sreturnpram,  vtris);

	// create a line that starts at the objects center
	tline lna;
	lna.vstart = lna.vend = spa.vcenter;

	// move the start and end point of the line away from the ceter of the object along the y-axis
	lna.vstart.y += 5.0f;
	lna.vend.y -= 5.0f;

	vec3f vnewpos;
	std::vector<csceneobject*> walltris;

	// if the line intersects a triangle continue with the clamping
	if (linesegmenttotriangle (vtris, lna, vnewpos, triindex, walltris))
	{
	// move the temp spheres center to the new pos and offset its y by its radius
	spa.vcenter = vnewpos;
	// this is a temp move and needs to be undone.  this was only done so that the sphere
	// does not go under any triangles that are in the air (ex: the chains)
	spa.vcenter.y += spa.fradius;

	// if we got a wall triangles back from linesegmenttotriangle loop through them to move away from walls
	for (size_t triindex = 0; triindex < walltris.size (); ++triindex)
	{
	vec3f vcpa;
	// if the temp sphere is colliding with a wall do something about it
	if (collisionlib::spheretotriangle (spa, *((ttriangle*)walltris[triindex]->getcollidableobject()->getboundingvolume(0)), vcpa))
	{
	// calculate the direction we want to move (away from the triangle)
	vec3f movedir = (spa.vcenter - vcpa);
	// find the distance we need to move
	float movedistance = movedir.magnitude ();
	// calculate the normal of the direction we are moving
	movedir /= movedistance;

	// add the temp sphere's center to the move direction times the diference of the sphere's radius and the move distance
	spa.vcenter = spa.vcenter + movedir * (spa.fradius - movedistance);
	}
	}

	// give the new sphere pos back but under the temp move we made
	voutpos = spa.vcenter;
	voutpos.y -= spa.fradius;
	return true;
	}
	return false;
	}
	*/

	static float linespace( float3 pt, float3 p1, float3 p2 )
	{
		float3 ray = p2 - p1; // find direction from p1 to p2
		float3 rel = pt - p1; // find position relative to p1
		float3 n = ray.normalize(); // create ray normal
		//return n.dotProduct( rel ); // calculate dot
		return dot_product( n, rel );
	}

	static float axisViolation( float3 _start, float3 _end, float3* _points, unsigned int count, float3& _violator  )
	{
		float3 ray = _end - _start;
		float raymag = ray.magnitude();

		bool isViolated = false;
		float projmin = linespace( _points[0], _start, _end );
		float projmax = projmin;
		float nearest = FLT_MAX;

		for( unsigned int i = 0; i < count; ++i )
		{
			float projection = linespace( _points[i], _start, _end );

			if( projection > projmax )
				projmax = projection;

			if( projection < projmin )
				projmin = projection;
		}

		if( projmin < 0.0f )
			projmin = 0.0f;
		if( projmin > raymag )
			projmin = raymag;

		if( projmax > raymag )
			projmax = raymag;
		if( projmax < 0.0f )
			projmax = 0.0f;

		float fPositive = projmax - 0.0f;
		float fNegative = projmin - raymag;

		if( abs( fPositive ) < abs( fNegative ) )
		{
			return fPositive;
		}
		else
		{
			return fNegative;
		}
	}

	bool AxisTest( float3 _start, float3 _end, float3* _points, unsigned int count, float& _violation, float3& _violator  );

	bool CheckCubeCollision( vec3f* _first, vec3f* _second, float2& _fOut, float2& _fDistOut );

	bool CheckCubeCollision2( vec3f* _first, unsigned int _firstCount, vec3f* _second, unsigned int _secondCount, float3& _fPoint, float3& _fOffset );

	};

#endif