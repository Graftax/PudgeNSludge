//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/CWinGameState.h"

#define IF(x) if(x)(x)

float3 CComponent_Collision::prjPoint2Line( float3 pt, float3 p1, float3 p2 )
{
	float3 ray = p2 - p1; // find direction from p1 to p2
	float3 rel = pt - p1; // find position relative to p1
	float3 n = float3( 1.0f, 0.0f, 0.0f );
	if( ray.magnitude() > FLT_EPSILON )
	{
		n = ray.normalize(); // create ray normal
	}

	float l = dot_product( n, rel ); // calculate dot
	return p1 + n * l; // convert back into world space
}

CComponent_Collision::CComponent_Collision() : IComponent( ECOMP_COLLISION )
{
	m_nCollType = CCMP_NULL;
	m_pPhysicsBox = nullptr;	
	m_fPos = vec3f( 0.0f, 0.0f, 0.0f );
	m_fVel = vec3f( 0.0f, 0.0f, 0.0f );
	m_bCollidingGround = false;
	m_bCollidingGroundLast = false;
	m_bCollidingWall = false;

	this->CleanRemoveVelocity();
}

CComponent_Collision::~CComponent_Collision(void)
{
	Shutdown();
}

bool CComponent_Collision::Initialize (/*CCollisionVolume* _cvVolume*/)
{
	/*if (_cvVolume)
	{
		switch (_cvVolume->GetVolumeType ())
		{
			case VMT_POINT:
				break;

			case VMT_PLANE:
				break;

			case VMT_LINE:
				break;

			case VMT_CIRCLE:
				break;
				
			case VMT_SPHERE:
				m_cvVolume = new Sphere (*(Sphere*)_cvVolume);
				break;

			case VMT_CAPSULE:
				break;

			case VMT_BOX:
				m_cvVolume = new Box (*(Box*)_cvVolume);
				break;

			case VMT_CUBE:
				break;

			case VMT_AABB:
				m_cvVolume = new AABB (*(AABB*)_cvVolume);
			break;
		}
	}
	else
	{
		m_cvVolume = NULL;
	}*/

	//m_GooPit.SetMaxPoint(vec3f( 122.0f, 58.0f, 121.0f ) + vec3f( 850.0f, 50.0f, 0.0f ));
	//m_GooPit.SetMinPoint(vec3f( -122.0f, -63.0f, -121.0f ) + vec3f( 850.0f, 50.0f, 0.0f ));

// 	m_CheckPoint.SetMaxPoint(vec3f( 3495.0f, 358.0f, 382.0f ) /*+ vec3f( 850.0f, 0.0f, 0.0f )*/);
// 	m_CheckPoint.SetMinPoint(vec3f( 2983.0f, 94.0f, 167.0f ) /*+ vec3f( 850.0f, 0.0f, 0.0f )*/);
	m_nCollType = CCMP_NULL;
	m_pPhysicsBox = nullptr;	
	m_fPos = vec3f( 0.0f, 0.0f, 0.0f );
	m_fVel = vec3f( 0.0f, 0.0f, 0.0f );
	m_bCollidingGround = false;
	m_bCollidingGroundLast = false;
	m_bCollidingWall = false;

	this->CleanRemoveVelocity();

	return true;
}

vec2f CComponent_Collision::GetPos2DClean()
{
	if( m_pPhysicsBox )
		m_fPos = m_pPhysicsBox->GetCentroid();

	return vec2f( m_fPos.x, m_fPos.y );
}

vec3f& CComponent_Collision::GetVel()
{
	if( m_pPhysicsBox )
		m_fVel = m_pPhysicsBox->GetCentroid() - m_pPhysicsBox->GetPrevCentroid();

	return m_fVel;
}

void CComponent_Collision::CleanForce( vec2f _fForce )
{
	if( m_pPhysicsBox )
	{
		m_pPhysicsBox->ApplyUniformForce( vec3f( _fForce.x, _fForce.y, 0.0f ), 1.0f );
		m_fVel = m_pPhysicsBox->GetCentroid() - m_pPhysicsBox->GetPrevCentroid();
	}
	else
	{
		m_fVel += vec3f( _fForce.x, _fForce.y, 0.0f );
	}
}

void CComponent_Collision::CleanOffset( vec2f _fOffset )
{
	if( m_pPhysicsBox )
	{
		m_pPhysicsBox->Offset( vec3f( _fOffset.x, _fOffset.y, 0.0f ) );
		m_fPos = m_pPhysicsBox->GetCentroid();
	}
	else
	{
		m_fPos += vec3f( _fOffset.x, _fOffset.y, 0.0f );
	}

	if( m_pParent )
		m_pParent->SetLocalPos( &m_fPos );
}

void CComponent_Collision::Update( float fDT )
{

}

void CComponent_Collision::Shutdown(void)
{

}

void CComponent_Collision::CleanRemoveVelocity()
{
	IF( m_pPhysicsBox )->NegateVelocity();
	m_fVel = vec3f( 0.0f, 0.0f, 0.0f );
}