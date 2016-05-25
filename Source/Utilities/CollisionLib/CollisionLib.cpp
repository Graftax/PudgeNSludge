#include "CollisionLib.h"

namespace CollisionLib
{
	bool CheckCubeCollision( vec3f* _first, vec3f* _second, float2& _fOut, float2& _fDistOut )
	{
		float cube1Planes[2];
		float cube2Planes[2];

		float3 cube1Violators[2];
		float3 cube2Violators[2];

		float3 violatingAxis1;
		float3 violatingAxis2;

		float3 blah;

		float3 cubePoints[4];
		cubePoints[0] = _second[0];
		cubePoints[1] = _second[1];
		cubePoints[2] = _second[2];
		cubePoints[3] = _second[3];

		cube1Planes[0] = axisViolation( _first[0],  _first[1], cubePoints, 4, cube1Violators[0] );
		if( cube1Planes[0] == 0.0f )
			return false;
		else
		{

		}

		cube1Planes[1] = axisViolation( _first[1], _first[2], cubePoints, 4, cube1Violators[1] );
		if( cube1Planes[1] == 0.0f )
			return false;
		else
		{

		}

		cubePoints[0] = _first[0];
		cubePoints[1] = _first[1];
		cubePoints[2] = _first[2];
		cubePoints[3] = _first[3];

		cube2Planes[0] = axisViolation( _second[ 0 ],  _second[ 1 ], cubePoints, 4, cube2Violators[0] );
		if( cube2Planes[0] == 0.0f )
			return false;
		else
		{

		}

		cube2Planes[1] = axisViolation( _second[ 1 ], _second[ 2 ], cubePoints, 4, cube2Violators[1] );
		if( cube2Planes[1] == 0.0f )
			return false;
		else
		{

		}

		bool cube1 = true;
		float lowestval = FLT_MAX;
		unsigned int index = INT_MAX;
		float3 violatingPoint;

		for( int i = 0; i < 2; ++i )
		{
			float stuff = abs( cube1Planes[i] );
			if( abs( cube1Planes[i] ) < abs( lowestval ) )
			{
				lowestval = cube1Planes[i];
				index = i;
				violatingPoint = cube1Violators[i];
			}
		}

		for( int i = 0; i < 2; ++i )
		{
			if( abs( cube2Planes[i] ) < abs( lowestval ) )
			{
				lowestval = cube2Planes[i];
				index = i;
				cube1 = false;
				violatingPoint = cube2Violators[i];
			}
		}

		float3 violatingvector;
		float violatinglength;

		if( cube1 )
		{
			violatingAxis1 = _first[index];
			violatingAxis2 = _first[index+1];

			//if( ( violatingAxis1 - cube1Violators[index] ).magnitude() > ( violatingAxis2 - cube1Violators[index] ).magnitude()  )
			//{
			//	swap( violatingAxis1, violatingAxis2 );
			//}

			//blah = cube1Violators[index];
		}
		else
		{
			violatingAxis1 = _second[index];
			violatingAxis2 = _second[index+1];

			//if( ( violatingAxis1 - cube2Violators[index] ).magnitude() > ( violatingAxis2 - cube2Violators[index] ).magnitude()  )
			//{
			//	swap( violatingAxis1, violatingAxis2 );
			//}

			//blah = cube2Violators[index];
		}


		violatinglength = lowestval;
		violatingvector = violatingAxis1 - violatingAxis2;

		_fOut = vec2f( violatingPoint.x, violatingPoint.y ); 

		if( !cube1 )
		{
			violatingvector.negate();
		}

		float3 stuff;
		stuff.x = violatingvector.x;
		stuff.y = violatingvector.y;
		stuff.z = 0.0f;

		stuff.normalize();
		stuff *= violatinglength;

		_fDistOut = vec2f( stuff.x, stuff.y );
		return true;
	}

	bool AxisTest( float3 _start, float3 _end, float3* _points, unsigned int count, float& _violation, float3& _violator  )
	{
		float3 fLine = _end - _start;
		float fRayMag = fLine.magnitude();

		if( fRayMag < FLT_EPSILON )
		{
			unsigned int broked = 0;
			return false;
		}

		float fLinespaceMin = fRayMag;
		float fLinespaceMax = 0.0f;
		vec3f fMinPoint = vec3f( 0.0f, 0.0f, 0.0f );
		vec3f fMaxPoint = vec3f( 0.0f, 0.0f, 0.0f );

		for( unsigned int i = 0; i < count; ++i )
		{
			float projection = linespace( _points[i], _start, _end );

			if( projection > fLinespaceMax )
			{
				fLinespaceMax = projection;
				fMaxPoint = _points[i];
			}

			if( projection < fLinespaceMin )
			{
				fLinespaceMin = projection;
				fMinPoint = _points[i];
			}
		}

		if( !( fLinespaceMax > FLT_EPSILON && fLinespaceMin < fRayMag - FLT_EPSILON ) )
			return false;

		if( fLinespaceMax > fRayMag )
			fLinespaceMax = fRayMag;
		if( fLinespaceMin < 0.0f )
			fLinespaceMin = 0.0f;

		float fPositive = fLinespaceMax;
		float fNegative = fRayMag - fLinespaceMin;

		if( abs( fPositive ) < abs( fNegative ) )
		{
			_violation = fPositive;
			_violator = fMaxPoint;
		}
		else
		{
			_violation = -fNegative;
			_violator = fMinPoint;
		}

		return true;
	}

	bool CheckCubeCollision2( vec3f* _first, unsigned int _firstCount, vec3f* _second, unsigned int _secondCount, float3& _fPoint, float3& _fOffset )
	{
		// Local structure for recording collision stuffs.
		struct AxisInfo
		{
			bool m_bSecond;
			float m_fViolation;
			vec3f m_fCollisionPoint;
			vec3f m_fStart;
			vec3f m_fEnd;
		};

		static AxisInfo AxisBuffer[16];

		// Project each of them against their axies and record the value;
		float fViolationRecord;
		float3 fViolatingPoint;

		for( unsigned int i = 0; i < _firstCount; ++i )
		{
			unsigned int unEndIndex = i + 1;
			if( unEndIndex >= _firstCount )
				unEndIndex = 0;
			if( !AxisTest( _first[ i ], _first[ unEndIndex ], _second, _firstCount, fViolationRecord, fViolatingPoint ) )
			{
				return false;
			}
			else
			{
				AxisBuffer[i].m_bSecond = false;
				AxisBuffer[i].m_fStart = _first[ i ];
				AxisBuffer[i].m_fEnd = _first[ unEndIndex ];
				AxisBuffer[i].m_fViolation = fViolationRecord;
			}

		}

		for( unsigned int i = 0; i < _secondCount; ++i )
		{
			unsigned int unEndIndex = i + 1;
			if( unEndIndex >= _secondCount )
				unEndIndex = 0;
			if( !AxisTest( _second[ i ], _second[ unEndIndex ], _first, _firstCount, fViolationRecord, fViolatingPoint ) )
			{
				return false;
			}
			else
			{	AxisBuffer[_firstCount + i].m_bSecond = true;
				AxisBuffer[_firstCount + i].m_fStart = _second[ i ];
				AxisBuffer[_firstCount + i].m_fEnd = _second[ unEndIndex ];
				AxisBuffer[_firstCount + i].m_fViolation = fViolationRecord;
			}
		}

		AxisInfo* pViolatedAxis = AxisBuffer;
		
		for( unsigned int i = 0; i < _firstCount + _secondCount; ++i )
		{
			if( abs( AxisBuffer[i].m_fViolation ) < abs( pViolatedAxis->m_fViolation ) )
			{
				pViolatedAxis = &AxisBuffer[i];
			}
		}

		if( !pViolatedAxis->m_bSecond )
			pViolatedAxis->m_fViolation *= -1.0f;

		vec3f _fLine = ( pViolatedAxis->m_fStart - pViolatedAxis->m_fEnd ).normalize();
		_fOffset = ( _fLine ) * pViolatedAxis->m_fViolation;

		/*D3DXMATRIX mat;

		D3DXMatrixIdentity (&mat);
		mat._41 = pViolatedAxis->m_fStart.x + _fOffset.x;
		mat._42 = pViolatedAxis->m_fStart.y + _fOffset.y;
		mat._43 = -400.0f;
		DebugShapes::RenderSphere (mat);

		D3DXMatrixIdentity (&mat);
		mat._41 = pViolatedAxis->m_fEnd.x + _fOffset.x;
		mat._42 = pViolatedAxis->m_fEnd.y + _fOffset.y;
		mat._43 = -400.0f;
		DebugShapes::RenderSphere (mat);*/

		return true;
	}
}
