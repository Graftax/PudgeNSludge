#include "PhysicsEnvironment.h"

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            PhysicsEnvironment.cpp
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             This contains and updates all of the 
//						physics points and constraints in the game.
//////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetCentroid():		Returns the center of the physics object.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec3f tPhyObject::GetCentroid()
{
	return ( m_pPoints[0]->m_CurrPos + m_pPoints[1]->m_CurrPos + m_pPoints[2]->m_CurrPos ) / 3.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetPrevCentroid():	Returns the center of the physics object, last frame.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec3f tPhyObject::GetPrevCentroid()
{
	return ( m_pPoints[0]->m_PrevPos + m_pPoints[1]->m_PrevPos + m_pPoints[2]->m_PrevPos ) / 3.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetXAxis():	Returns the X Axis( Clockwise ) from the matrix of the object.
//				Note: It will probably have to be negated.				
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec3f tPhyObject::GetXAxis()
{
	return ( m_pPoints[1]->m_CurrPos - m_pPoints[2]->m_CurrPos ).normalize();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetYAxis():			Returns the Y axis.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec3f tPhyObject::GetYAxis()
{
	vec3f fBottomCenter = ( m_pPoints[1]->m_CurrPos + m_pPoints[2]->m_CurrPos ) / 2.0f;
	vec3f toReturn = m_pPoints[0]->m_CurrPos - fBottomCenter;
	toReturn.normalize();
	return toReturn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplyUniformForce():	Applies an equal force to the entire object.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tPhyObject::ApplyUniformForce( vec3f _fDirection, float _fMagnitude )
{
	m_pPoints[0]->m_PrevPos -= _fDirection * _fMagnitude;
	m_pPoints[1]->m_PrevPos -= _fDirection * _fMagnitude;
	m_pPoints[2]->m_PrevPos -= _fDirection * _fMagnitude;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NegateVelocity():	Destroys the velocity of the object.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tPhyObject::NegateVelocity()
{
	m_pPoints[0]->m_PrevPos = m_pPoints[0]->m_CurrPos;
	m_pPoints[1]->m_PrevPos = m_pPoints[1]->m_CurrPos;
	m_pPoints[2]->m_PrevPos = m_pPoints[2]->m_CurrPos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SetPosition():	Sets the position of the object, in world space.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tPhyObject::SetPosition( vec3f _pos )
{
	vec3f fCenter = GetCentroid();

	tPhyPoint offset0;
	offset0.m_CurrPos = m_pPoints[0]->m_CurrPos - fCenter;
	offset0.m_PrevPos = m_pPoints[0]->m_PrevPos - fCenter;

	tPhyPoint offset1;
	offset1.m_CurrPos = m_pPoints[1]->m_CurrPos - fCenter;
	offset1.m_PrevPos = m_pPoints[1]->m_PrevPos - fCenter;

	tPhyPoint offset2;
	offset2.m_CurrPos = m_pPoints[2]->m_CurrPos - fCenter;
	offset2.m_PrevPos = m_pPoints[2]->m_PrevPos - fCenter;

	m_pPoints[0]->m_CurrPos = _pos + offset0.m_CurrPos;
	m_pPoints[1]->m_CurrPos = _pos + offset1.m_CurrPos;
	m_pPoints[2]->m_CurrPos = _pos + offset2.m_CurrPos;
	m_pPoints[0]->m_PrevPos = _pos + offset0.m_PrevPos;
	m_pPoints[1]->m_PrevPos = _pos + offset1.m_PrevPos;
	m_pPoints[2]->m_PrevPos = _pos + offset2.m_PrevPos;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Offset():			Pops the object in a direction, regardless of collision.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tPhyObject::Offset( vec3f _direction )
{
	m_pPoints[0]->m_CurrPos += _direction;
	m_pPoints[1]->m_CurrPos += _direction;
	m_pPoints[2]->m_CurrPos += _direction;
	m_pPoints[0]->m_PrevPos += _direction;
	m_pPoints[1]->m_PrevPos += _direction;
	m_pPoints[2]->m_PrevPos += _direction;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddPoint():		Creates a point in the physics environment.
//
// Returns:			tPhyPoint* - Returns a pointer to the point that was just created.                               
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tPhyPoint* CPhysicsEnvironment::AddPoint( vec3f _fPos )
{
	tPhyPoint* pToAdd = new tPhyPoint();
	pToAdd->m_PrevPos = pToAdd->m_CurrPos = _fPos;
	m_pPointList.push_back( pToAdd );
	return pToAdd;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddConstraint():	Creates a contraint between two points.
//
// Returns:			tPhyConstraint* - Returns a pointer to the constraint that was just created in the environment.                              
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tPhyConstraint* CPhysicsEnvironment::AddConstraint( float _fDistance, tPhyPoint* _first, tPhyPoint* _second )
{
	tPhyConstraint* pToAdd = new tPhyConstraint();
	pToAdd->m_pPoint1 = _first;
	pToAdd->m_pPoint2 = _second;
	pToAdd->m_fDist = _fDistance;
	m_pConstList.push_back( pToAdd );
	return pToAdd;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RemovePoint():	Removes a point from the physics environemnt, includes cleanup.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::RemovePoint( tPhyPoint* _toRemove )
{
	m_pPointList.remove( _toRemove );
	delete _toRemove;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveConstraint():	Removes a constraint from the physics environment, includes cleanup.
//
// Returns:				Void                              
//
// Mod. Name:			EP
// Mod. Date:			5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::RemoveConstraint( tPhyConstraint* _pToRemove )
{
	m_pConstList.remove( _pToRemove );
	delete _pToRemove;
}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():		Updates the physics environment, stepping all of the points and constraints forward.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::Update( float fDT )
{
	// Setup universal forces.
	vec3f worldForces = vec3f( 0.0f, 0.0f, 0.0f );
	worldForces += m_fGravity;

	// Loop through all of the points.
	std::list< tPhyPoint* >::iterator itrPoints = m_pPointList.begin();
	while( itrPoints != m_pPointList.end() )
	{
		// Move their position based on their old.
		//float3 fTrans = ( (*itrPoints)->m_CurrPos - (*itrPoints)->m_PrevPos ) * fDT;
		float3 newPos = (*itrPoints)->m_CurrPos * 2.0f - (*itrPoints)->m_PrevPos + worldForces * fDT;
		(*itrPoints)->m_PrevPos = (*itrPoints)->m_CurrPos;
		(*itrPoints)->m_CurrPos = newPos;
		//(*itrPoints)->m_CurrPos += fTrans + worldForces * fDT;
		//(*itrPoints)->m_PrevPos += fTrans;

		itrPoints++;
	}

	// Relaxing
	std::list< tPhyConstraint* >::iterator itrConst;

	// Currently iterating through constraints 3 times.
	for( int loop = 0; loop < 3; ++loop )
	{
		itrConst = m_pConstList.begin();

		while( itrConst != m_pConstList.end() )
		{
			tPhyPoint* m_pParticleA = (*itrConst)->m_pPoint1;
			tPhyPoint* m_pParticleB = (*itrConst)->m_pPoint2;

			vec3f delta = m_pParticleB->m_CurrPos - m_pParticleA->m_CurrPos;

			float distSquared = (*itrConst)->m_fDist * (*itrConst)->m_fDist;
			float delDotDel = dot_product( delta, delta );

			delta *= distSquared / ( delDotDel + distSquared ) - 0.5f; 

			m_pParticleA->m_CurrPos -= delta;
			m_pParticleB->m_CurrPos += delta;

			itrConst++;
		}
	}

	std::list< tPhyVelocityConstraint* >::iterator itrVelConst = m_pVelConstList.begin();

	// Velocity constraints are expensive!
	while( itrVelConst != m_pVelConstList.end() )
	{
		vec3f fDiff = (*itrVelConst)->m_pTarget->m_CurrPos - (*itrVelConst)->m_pTarget->m_PrevPos;
		float fMag = fDiff.magnitude();

		// If the velocity is higher, than limit it.
		if( fMag > (*itrVelConst)->m_fLimit )
		{
			(*itrVelConst)->m_pTarget->m_PrevPos += fDiff.normalize() * ( fMag - (*itrVelConst)->m_fLimit );
		}

		itrVelConst++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Clear():			Removes all points, constraints, and boxes from the physics environment. Includes cleanup.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::Clear()
{
	std::list< tPhyPoint* >::iterator itrPoints = m_pPointList.begin();

	while( itrPoints != m_pPointList.end() )
	{
		delete (*itrPoints);
		itrPoints++;
	}

	m_pPointList.clear();

	std::list< tPhyConstraint* >::iterator itrConst = m_pConstList.begin();

	while( itrConst != m_pConstList.end() )
	{
		delete (*itrConst);
		itrConst++;
	}

	m_pConstList.clear();

	std::list< tPhyObject* >::iterator itrBox = m_pBoxList.begin();

	while( itrBox != m_pBoxList.end() )
	{
		delete (*itrBox);
		itrBox++;
	}

	m_pBoxList.clear();

	std::list< tPhyVelocityConstraint* >::iterator itrVelConst = m_pVelConstList.begin();

	while( itrVelConst != m_pVelConstList.end() )
	{
		delete (*itrVelConst);
		itrVelConst++;
	}

	m_pVelConstList.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddBox():		Adds a box constructed from constraints and points to the physics environment.
//
// Returns:			tPhyBox* - A pointer to the box that was created.                              
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tPhyObject* CPhysicsEnvironment::AddPhyObject( vec3f _centeroid, float _fRadius )
{
	tPhyObject* pToAdd = new tPhyObject();

	pToAdd->m_pPoints[0] = AddPoint( vec3f( 0.0f, _fRadius, 0.0f ) + _centeroid );
	pToAdd->m_pPoints[1] = AddPoint( vec3f( _fRadius, -_fRadius / 2.0f, 0.0f ) + _centeroid );
	pToAdd->m_pPoints[2] = AddPoint( vec3f( -_fRadius, -_fRadius / 2.0f, 0.0f ) + _centeroid );


	pToAdd->m_pConstraints[ 0 ] = AddConstraint( _fRadius,	pToAdd->m_pPoints[0], pToAdd->m_pPoints[1] );
	pToAdd->m_pConstraints[ 1 ] = AddConstraint( _fRadius, pToAdd->m_pPoints[1], pToAdd->m_pPoints[2] );
	pToAdd->m_pConstraints[ 2 ] = AddConstraint( _fRadius,	pToAdd->m_pPoints[2], pToAdd->m_pPoints[0] );

	m_pBoxList.push_back( pToAdd );

	return pToAdd;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RemoveBox():		Removes a box from the physics environment. Includes cleanup.
//
// Returns:			Void                              
//
// Mod. Name:       EP
// Mod. Date:       5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::RemovePhyObject( tPhyObject* _pToRemove )
{
	RemovePoint( _pToRemove->m_pPoints[0] );
	RemovePoint( _pToRemove->m_pPoints[1] );
	RemovePoint( _pToRemove->m_pPoints[2] );

	RemoveConstraint( _pToRemove->m_pConstraints[0] );
	RemoveConstraint( _pToRemove->m_pConstraints[1] );
	RemoveConstraint( _pToRemove->m_pConstraints[2] );

	std::list< tPhyVelocityConstraint* >::iterator itrVelConst = _pToRemove->m_pVelConstraints.begin();

	while( itrVelConst != _pToRemove->m_pVelConstraints.end() )
	{
		m_pVelConstList.remove( (*itrVelConst) );
		delete (*itrVelConst);
		itrVelConst++;
	}

	_pToRemove->m_pVelConstraints.clear();

	m_pBoxList.remove( _pToRemove );
	delete _pToRemove;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplyUniformForce():		Applies a force to all of the points that make up a box.
//							Warning: This is VERY potent. Values larger than 1.0 can be unstable.
//
// Returns:					Void                              
//
// Mod. Name:				EP
// Mod. Date:				5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::ApplyUniformForce( vec3f _fDirection, float _fMagnitude, tPhyObject* _target )
{
	_target->m_pPoints[0]->m_CurrPos += _fDirection * _fMagnitude;
	_target->m_pPoints[1]->m_CurrPos += _fDirection * _fMagnitude;
	_target->m_pPoints[2]->m_CurrPos += _fDirection * _fMagnitude;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ApplyPointForce():	This isnt written yet, so don't call it.
//
// Returns:				Void                              
//
// Mod. Name:			EP
// Mod. Date:			5/10/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::ApplyPointForce( vec3f _position, vec3f _fDirection, vec3f _Magnitude, tPhyObject* _target )
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AddVelConstraint():	Adds a velocity constraint that prevents the 
//						object from accelerating to a velocity faster 
//						than the limit.
//
// Returns:				VOID                             
//
// Mod. Name:			EP
// Mod. Date:			5/12/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPhysicsEnvironment::AddVelConstraint( float _fLimit, tPhyObject* _target  )
{
	tPhyVelocityConstraint* pVelConst = new tPhyVelocityConstraint();
	pVelConst->m_fLimit = _fLimit;
	pVelConst->m_pTarget = _target->m_pPoints[0];
	_target->m_pVelConstraints.push_back( pVelConst );
	m_pVelConstList.push_back( pVelConst ); 

	pVelConst = new tPhyVelocityConstraint();
	pVelConst->m_fLimit = _fLimit;
	pVelConst->m_pTarget = _target->m_pPoints[1];
	_target->m_pVelConstraints.push_back( pVelConst );
	m_pVelConstList.push_back( pVelConst ); 

	pVelConst = new tPhyVelocityConstraint();
	pVelConst->m_fLimit = _fLimit;
	pVelConst->m_pTarget = _target->m_pPoints[2];
	_target->m_pVelConstraints.push_back( pVelConst );
	m_pVelConstList.push_back( pVelConst ); 
}

void CPhysicsEnvironment::SimulateObject( tPhyObject* _toSimulate, float fDT )
{
	// Setup universal forces.
	vec3f worldForces = vec3f( 0.0f, 0.0f, 0.0f );
	worldForces += m_fGravity;

	for( unsigned int i = 0; i < 3; ++i )
	{
		float3 newPos = _toSimulate->m_pPoints[i]->m_CurrPos * 2.0f - _toSimulate->m_pPoints[i]->m_PrevPos + worldForces * fDT;
		_toSimulate->m_pPoints[i]->m_PrevPos = _toSimulate->m_pPoints[i]->m_CurrPos;
		_toSimulate->m_pPoints[i]->m_CurrPos = newPos;
	}

	// Currently iterating through constraints 3 times.
	for( int loop = 0; loop < 1; ++loop )
	{
		for( unsigned int i = 0; i < 3; ++i )
		{
			tPhyPoint* m_pParticleA = _toSimulate->m_pConstraints[i]->m_pPoint1;
			tPhyPoint* m_pParticleB = _toSimulate->m_pConstraints[i]->m_pPoint2;

			vec3f delta = m_pParticleB->m_CurrPos - m_pParticleA->m_CurrPos;

			float distSquared = _toSimulate->m_pConstraints[i]->m_fDist * _toSimulate->m_pConstraints[i]->m_fDist;
			float delDotDel = dot_product( delta, delta );

			delta *= distSquared / ( delDotDel + distSquared ) - 0.5f; 

			m_pParticleA->m_CurrPos -= delta;
			m_pParticleB->m_CurrPos += delta;
		}
	}
}