#include "CBossIdleAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../RenderEngine/DebugShapes.h"
#include "../ComponentManager/CComponent_BossCollision.h"
#include "BossMainAttackAI.h"

#define HOVER_WAIT 1.0f
#define FLY_WAIT 6.0f

BossIdleAI::BossIdleAI ()
{

}

BossIdleAI::~BossIdleAI()
{

}

void BossIdleAI::Init (CComponent_Collision* _pParent)
{
	IBaseAI::Init (_pParent);
	m_FloatSphere.SetRadius (400.0f);
	m_FloatSphere.SetCenter (_pParent->GetPos());
	m_v3MoveDir = vec3f (1.0f, 0.0f, 0.0f);

	DebugShapes::CreateSphere (30.0f, 5, 5);
	m_fHoverTime = 0.0f;
	m_fFlyTime = 0.0f;
}

void BossIdleAI::Shutdown ()
{

}

void BossIdleAI::Update (float _fElapsedTime)
{
	BossAttackAI asdf;
	if ( ((BossAttackAI*)(((CComponent_BossCollision*)m_pParentCollcmp)->GetAttackAI()))->GetNumSpawnedGoos () == 0 )
	{
		((CComponent_BossCollision*)m_pParentCollcmp)->SetAI ( ((CComponent_BossCollision*)m_pParentCollcmp)->GetAttackAI());
		return;
	}
	
	m_fFlyTime += _fElapsedTime;

	m_FloatSphere.SetCenter (m_pParentCollcmp->GetPos());

	//D3DXMATRIX mat;
	//D3DXMatrixIdentity (&mat);
	//mat._41 = m_FloatSphere.GetCenter2D().x;
	//mat._42 = m_FloatSphere.GetCenter2D().y;
	//mat._43 = 0.0f;
	//DebugShapes::RenderSphere (mat);

	//D3DXMatrixIdentity (&mat);
	//mat._41 = m_FloatSphere.GetCenter2D().x;
	//mat._42 = m_FloatSphere.GetCenter2D().y - m_FloatSphere.GetRadius();
	//mat._43 = 0.0f;
	//DebugShapes::RenderSphere (mat);

	//D3DXMatrixIdentity (&mat);
	//mat._41 = m_FloatSphere.GetCenter2D().x + m_FloatSphere.GetRadius();
	//mat._42 = m_FloatSphere.GetCenter2D().y;
	//mat._43 = 0.0f;
	//DebugShapes::RenderSphere (mat);

	//D3DXMatrixIdentity (&mat);
	//mat._41 = m_FloatSphere.GetCenter2D().x - m_FloatSphere.GetRadius();
	//mat._42 = m_FloatSphere.GetCenter2D().y;
	//mat._43 = 0.0f;
	//DebugShapes::RenderSphere (mat);

	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION);

	// call the kd tree and get the near objects
	CSceneObject _FloatSceneObj;
	_FloatSceneObj.SetCollidableObject (&m_FloatSphere);

	CKdTree::GetNearObjects ( &_FloatSceneObj, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	//_FloatSceneObj.SetCollidableObject (NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		vec2f PushOut;
		vec2f CP;
		if ( m_FloatSphere.SphereToAABB (objAABB, PushOut, CP) )
		{
			/*D3DXMatrixIdentity (&mat);
			mat._41 = CP.x;
			mat._42 = CP.y;
			mat._43 = 0.0f;
			DebugShapes::RenderSphere (mat);*/

			vec3f Direction = vec3f (m_FloatSphere.GetCenter2D().x - CP.x, m_FloatSphere.GetCenter2D().y - CP.y, 0.0f);
			vec3f Normal = Direction;
			Normal.normalize();

			float distance = Direction.magnitude();

			// check for a zero distance
			if (distance > m_FloatSphere.GetRadius() || (distance < FLT_EPSILON && distance > -FLT_EPSILON))
			{
				continue;
			}

			if (Direction.x)
			{
				if (distance < m_FloatSphere.GetRadius() * 0.4f)
				{
					m_v3MoveDir.x = m_FloatSphere.GetCenter2D().x - CP.x;
					m_v3MoveDir.normalize();
					m_pParentCollcmp->GetVel ().x += m_v3MoveDir.x * 0.6f;
				}
				continue;
			}

			if (Normal.magnitude())
			{
				m_pParentCollcmp->CleanOffset (PushOut * _fElapsedTime);
				m_pParentCollcmp->GetVel ().y = 0.0f;
				//m_pParentCollcmp->CleanForce ( vec2f (0.0f, Normal.y) * (520.0f) * _fElapsedTime );
			}
		}
	}
	
	if (m_fHoverTime > 0.0f && m_fFlyTime >= FLY_WAIT)
	{
		m_fHoverTime -= _fElapsedTime;
		m_fFlyTime = 0.0f;
		m_pParentCollcmp->CleanRemoveVelocity();
		return;
	}

	if (m_fFlyTime <= FLY_WAIT)
	{
		m_fHoverTime = HOVER_WAIT;
		m_pParentCollcmp->CleanForce ( vec2f (m_v3MoveDir.x, m_v3MoveDir.y) * 3.0f * _fElapsedTime );
	}
}