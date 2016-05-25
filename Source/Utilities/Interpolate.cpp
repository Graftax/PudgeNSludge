#include "Interpolate.h"

D3DXMATRIX Interpolate( const D3DXMATRIX& MatrixA, const D3DXMATRIX& MatrixB, float lamda)
{
	D3DXMATRIX iMat = MatrixA;
	D3DXMATRIX result = MatrixB;

	// Inverse of MatrixA
	FLOAT determinant = D3DXMatrixDeterminant(&iMat);
	D3DXMatrixInverse(&iMat, &determinant, &iMat);

	// Remove MatrixA's transformation from MatrixB
	result *= iMat;

	// iMat is now the intermediary transformation from MatrixA to MatrixB
	// ie: iMat * MatrixA = MatrixB
	iMat = result;

	// The trace of our matrix
	float trace = 1.0f + iMat._11 + iMat._22 + iMat._33;

	float quatResult[4];

	// Calculate the quaternion of iMat
	// If trace is greater than 0, but consider small values that
	// might result in 0 when operated upon due to floating point error
	if( trace > 0.00000001 )
	{
		float S = sqrt(trace)*2;
		quatResult[0] = (iMat._32 - iMat._23) / S;
		quatResult[1] = (iMat._13 - iMat._31) / S;
		quatResult[2] = (iMat._21 - iMat._12) / S;
		quatResult[3] = 0.25f * S;
	}
	else
	{
		if( iMat._11 > iMat._22 && iMat._11 > iMat._33 )
		{
			float S = float(sqrt( 1.0 + iMat._11 - iMat._22 - iMat._33 ) * 2);
			quatResult[0] = 0.25f * S;
			quatResult[1] = (iMat._21 + iMat._12) / S;
			quatResult[2] = (iMat._13 + iMat._31) / S;
			quatResult[3] = (iMat._32 - iMat._23) / S;
		}
		else if( iMat._22 > iMat._33 )
		{
			float S = float(sqrt( 1.0 + iMat._22 - iMat._11 - iMat._33 ) * 2);
			quatResult[0] = (iMat._21 + iMat._12) / S;
			quatResult[1] = 0.25f * S;
			quatResult[2] = (iMat._32 + iMat._23) / S;
			quatResult[3] = (iMat._13 - iMat._31) / S;
		}
		else
		{
			float S = float(sqrt( 1.0 + iMat._33 - iMat._11 - iMat._22 ) * 2);
			quatResult[0] = (iMat._13 + iMat._31) / S;
			quatResult[1] = (iMat._32 + iMat._23) / S;
			quatResult[2] = 0.25f * S;
			quatResult[3] = (iMat._21 - iMat._12) / S;
		}
	}

	// Get the magnitude of our quaternion
	float quatMagnitude = sqrt( quatResult[0]*quatResult[0] + quatResult[1]*quatResult[1] + quatResult[2]*quatResult[2] + quatResult[3]*quatResult[3] );

	// Normalize our quaternion
	float quatNormalized[4] = { quatResult[0]/quatMagnitude, quatResult[1]/quatMagnitude, quatResult[2]/quatMagnitude, quatResult[3]/quatMagnitude }; 

	// Calculate the angles relevant to our quaternion
	float cos_a = quatNormalized[3];
	float angle = acos( cos_a ) * 2;
	float sin_a = float(sqrt( 1.0 - cos_a * cos_a ));
	
	// If there was no rotation between matrices, calculation
	// of the rotation matrix will end badly. So just do the linear
	// interpolation of the translation component and return
	if( angle == 0.0 )
	{
		result = MatrixA;

		result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0])*lamda);
		result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1])*lamda);
		result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2])*lamda);

		return result;
	}


	// Our axis of abitrary rotation
	D3DXVECTOR3 axis;

	if( fabs( sin_a ) < 0.0005 )
		sin_a = 1;

	axis.x = quatNormalized[0] / sin_a;
	axis.y = quatNormalized[1] / sin_a;
	axis.z = quatNormalized[2] / sin_a;

	// Get the portion of the angle to rotate by
	angle *= lamda;

	D3DXVec3Normalize(&axis, &axis);

	// Calculate the quaternion for our new (partial) angle of rotation
	sin_a = sin( angle / 2 );
	cos_a = cos( angle / 2 );
	quatNormalized[0] = axis.x * sin_a;
	quatNormalized[1] = axis.y * sin_a;
	quatNormalized[2] = axis.z * sin_a;
	quatNormalized[3] = cos_a;

	quatMagnitude = sqrt( quatNormalized[0]*quatNormalized[0] + quatNormalized[1]*quatNormalized[1] + quatNormalized[2]*quatNormalized[2] + quatNormalized[3]*quatNormalized[3] );		
	quatNormalized[0] /= quatMagnitude;
	quatNormalized[1] /= quatMagnitude;
	quatNormalized[2] /= quatMagnitude;
	quatNormalized[3] /= quatMagnitude;

	// Calculate our partial rotation matrix
	float xx      = quatNormalized[0] * quatNormalized[0];
	float xy      = quatNormalized[0] * quatNormalized[1];
	float xz      = quatNormalized[0] * quatNormalized[2];
	float xw      = quatNormalized[0] * quatNormalized[3];
	float yy      = quatNormalized[1] * quatNormalized[1];
	float yz      = quatNormalized[1] * quatNormalized[2];
	float yw      = quatNormalized[1] * quatNormalized[3];
	float zz      = quatNormalized[2] * quatNormalized[2];
	float zw      = quatNormalized[2] * quatNormalized[3];

	result._11  = 1 - 2 * ( yy + zz );
	result._12  =     2 * ( xy - zw );
	result._13  =     2 * ( xz + yw );
	result._21  =     2 * ( xy + zw );
	result._22  = 1 - 2 * ( xx + zz );
	result._23  =     2 * ( yz - xw );
	result._31  =     2 * ( xz - yw );
	result._32  =     2 * ( yz + xw );
	result._33 = 1 - 2 * ( xx + yy );
	result._14  = result._24 = result._34 = result._41 = result._42 = result._43 = 0;
	result._44 = 1;

	// Combine our partial rotation with MatrixA
	result *= MatrixA;

	// Linear interpolation of the translation components of the matrices
	result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0])*lamda);
	result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1])*lamda);
	result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2])*lamda);

	return result;
}