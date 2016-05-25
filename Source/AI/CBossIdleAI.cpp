#include "CBossIdleAI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../RenderEngine/DebugShapes.h"
#include "../ComponentManager/CComponent_BossCollision.h"
#include "CBossAttackAI.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

#define HOVER_WAIT 1.0f
#define FLY_WAIT 6.0f

BossIdleAI::BossIdleAI (IBaseObject* _pParent) : IBaseAI (_pParent)
{

}

BossIdleAI::~BossIdleAI()
{

}

void BossIdleAI::Init ()
{
	vec3f ParentPos = m_pParentObject->GetLocalMat()->axis_pos;

	IBaseAI::Init ();
	m_FloatSphere.SetRadius (500.0f);
	m_FloatSphere.SetCenter (ParentPos);
	m_v3MoveDir = vec3f (-1.0f, 0.0f, 0.0f);

	m_LineToCeling.SetStartPoint ( ParentPos );
	m_LineToCeling.SetEndPoint ( vec2f (ParentPos.x, ParentPos.y + m_FloatSphere.GetRadius() * 0.6f ) );

	m_LineToRight.SetStartPoint ( m_pParentObject->GetLocalMat()->axis_pos );
	m_LineToRight.SetEndPoint ( vec2f (ParentPos.x +  m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_LineToLeft.SetStartPoint ( ParentPos );
	m_LineToLeft.SetEndPoint ( vec2f (ParentPos.x - m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_AltLineToLeft.SetStartPoint ( ParentPos );
	m_AltLineToLeft.SetEndPoint ( vec2f (ParentPos.x - 1500.0f, ParentPos.y ) );

	m_AltLineToRight.SetStartPoint ( ParentPos );
	m_AltLineToRight.SetEndPoint ( vec2f (ParentPos.x + 1500.0f, ParentPos.y ) );

	DebugShapes::CreateSphere (30.0f, 5, 5);
	m_fHoverTime = 0.0f;
	m_fFlyTime = 0.0f;
	m_fElapsedTime = 0.0f;

	m_bMaxHeightReached = false;
}

void BossIdleAI::Shutdown ()
{

}

bool BossIdleAI::LineOfSight ()
{
	IBaseObject* pPlayer = CObjectManager::GetInstance()->GetPlayer();

	Line SightLine;
	SightLine.SetStartPoint(m_pParentObject->GetWorldPos());
	
	if (pPlayer)
	{
		SightLine.SetEndPoint(pPlayer->GetWorldPos());
	}
	else
	{
		return false;
	}

	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_WALL_BREAKABLE) | (1<<OBJ_DOOR) | (1<<OBJ_PLAYER) | (1<<OBJ_PIT);

	// call the kd tree and get the near objects
	CSceneObject _LineObj;
	_LineObj.SetCollidableObject (&SightLine);

	CKdTree::GetNearObjects ( &_LineObj, PSFLAG_LINE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_LineObj.SetCollidableObject (NULL);

	bool _bInSight = false;

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());

		vec3f CPA, CPB;
		if (SightLine.LineToAABB (objAABB, CPA, CPA))
		{
			SightLine.SetEndPoint (CPA);
			if (_pObject->GetType() == OBJ_PLAYER)
			{
				int x = 0;
				_bInSight = true;
			}
			else
			{
				int x = 0;
				_bInSight = false;
			}
		}
	}
	return _bInSight;
}

void BossIdleAI::Update (float _fElapsedTime)
{
	if (LineOfSight())
	{
		m_pParentAIcmp->SwitchAI (AI_ATTACK);
		BossAttackAI* AttackAI = ((BossAttackAI*)m_pParentAIcmp->GetLoadedAI(AI_ATTACK));

		if (AttackAI)
		{
			AttackAI->SetMoveDir (m_v3MoveDir);
		}
		return;
	}

	m_fElapsedTime = _fElapsedTime;
	m_bMaxHeightReached = false;
	
	m_fFlyTime += _fElapsedTime;

	vec3f ParentPos = m_pParentCollcmp->GetPos();

	m_FloatSphere.SetCenter ( ParentPos );

	m_LineToCeling.SetStartPoint ( ParentPos );
	m_LineToCeling.SetEndPoint ( vec2f (ParentPos.x, ParentPos.y + m_FloatSphere.GetRadius() * 0.6f ) );

	m_LineToRight.SetStartPoint ( m_pParentObject->GetLocalMat()->axis_pos );
	m_LineToRight.SetEndPoint ( vec2f (ParentPos.x +  m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_LineToLeft.SetStartPoint ( ParentPos );
	m_LineToLeft.SetEndPoint ( vec2f (ParentPos.x - m_FloatSphere.GetRadius() * 0.25f, ParentPos.y ) );

	m_AltLineToLeft.SetStartPoint ( ParentPos );
	m_AltLineToLeft.SetEndPoint ( vec2f (ParentPos.x - 1500.0f, ParentPos.y ) );

	m_AltLineToRight.SetStartPoint ( ParentPos );
	m_AltLineToRight.SetEndPoint ( vec2f (ParentPos.x + 1500.0f, ParentPos.y ) );


	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_WALL_BREAKABLE) | (1<<OBJ_DOOR);

	// call the kd tree and get the near objects
	CSceneObject _FloatSceneObj;
	_FloatSceneObj.SetCollidableObject (&m_FloatSphere);

	CKdTree::GetNearObjects ( &_FloatSceneObj, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	_FloatSceneObj.SetCollidableObject (NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = (IBaseObject*)NearestObjects[objindex];
		AABB objAABB = *((AABB*)_pObject->GetCollidableObject());
		
		ChecKCelingCollision ( objAABB );
		CheckWallCollision ( objAABB );
	}
	
	if (m_bMaxHeightReached)
	{
		m_pParentCollcmp->CleanForce ( vec2f (m_v3MoveDir.x, 0.0f) * 10.0f * _fElapsedTime );
	}
}

void BossIdleAI::ChecKCelingCollision ( AABB _Volume )
{
	// check if the celing line has collided with a static aabb
	vec3f CPA, CPB;

	if ( m_LineToCeling.LineToAABB ( _Volume, CPA, CPB ) )
	{
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToCeling.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_bMaxHeightReached = true;
			// move the ship over time so it doesnt seem like it just poped
			m_pParentCollcmp->CleanOffset (PushOut * m_fElapsedTime);
		
			// remove the ships velocity
			m_pParentCollcmp->GetVel ().y = 0.0f;
		}
	}
}

void BossIdleAI::CheckWallCollision ( AABB _Volume )
{
	// check if the celing line has collided with a static aabb
	vec3f CPA, CPB;

	if ( m_LineToRight.LineToAABB ( _Volume, CPA, CPB ) )
	{
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToCeling.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_v3MoveDir.x = -1.0f;
			m_pParentCollcmp->GetVel ().x += m_v3MoveDir.x * 0.4f;
		}
	}

	if ( m_LineToLeft.LineToAABB ( _Volume, CPA, CPB ) )
	{
		// find the direction to push out from end of the line to the closest point on the aabb
		vec2f PushOut = vec2f (CPA.x, CPA.y) - m_LineToCeling.GetEndPoint2D();
		
		// make sure the push out isnt 0
		if (PushOut.magnitude())
		{
			m_v3MoveDir.x = 1.0f;
			m_pParentCollcmp->GetVel ().x += m_v3MoveDir.x * 0.4f;

			//m_pParentAIcmp->SwitchAI (AI_ATTACK);
			//BossAttackAI* AttackAI = ((BossAttackAI*)m_pParentAIcmp->GetLoadedAI(AI_ATTACK));

			//if (AttackAI)
			//{
			//	AttackAI->SetMoveDir (m_v3MoveDir);
			//}
		}
	}
}