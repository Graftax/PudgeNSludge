#include "CComponent_Item.h"
#include "CComponent_Collision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"

bool CComponent_Item::Initialize(void)
{
	m_pTarget = NULL;
	m_bAttached = false;

	return true;
}

void CComponent_Item::Update( float fDT )
{
	if( m_pTarget )
	{
		CComponent_Input* PlayerInputComponent = (CComponent_Input*)m_pTarget->GetComponent( ECOMP_INPUT );
		if( PlayerInputComponent->_IsObjectPulled() )
		{
			m_pTarget = NULL;
			return;
		}

		CComponent_Collision* pCollider = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
		if(!m_bAttached)
		{
			vec3f TargetPos = m_pTarget->GetWorldPos();
			TargetPos.y += 50.0f;

			//CComponent_Input* targetInput = (CComponent_Input*)m_pTarget->GetComponent (ECOMP_INPUT);

			//if (targetInput)
			//{
			//	if (targetInput->GetRotatedRight())
			//	{
			//		TargetPos.x += 80.0f;
			//	}
			//	else
			//	{
			//		TargetPos.x -= 80.0f;
			//	}	
			//			
			//}

			vec3f fTempVel = pCollider->GetVel();
			fTempVel.negate();
			fTempVel *= 0.2f;
			pCollider->CleanForce( vec2f( fTempVel.x, fTempVel.y ) );

			vec3f fTowards = (TargetPos - m_pParent->GetWorldPos()).normalize();
			pCollider->CleanForce( vec2f( fTowards.x, fTowards.y ) * 500.0f * fDT );

			//if(pCollider->GetPhysicsBox())
			//{

			//	vec3f fTowards = (TargetPos- m_pParent->GetWorldPos()).normalize();
			//	vec3f fVelocity = pCollider->GetPhysicsBox()->GetCentroid() - pCollider->GetPhysicsBox()->GetPrevCentroid();
			//	fVelocity.negate();

			//	pCollider->GetPhysicsBox()->ApplyUniformForce( fVelocity, 0.3f ); 
			//	pCollider->GetPhysicsBox()->ApplyUniformForce( fTowards.normalize(), 500.0f * fDT );
			//}


			if( m_pTarget->GetCollidableObject()->GetBodyType() == BT_DYNAMIC && m_pTarget->GetType() == OBJ_PLAYER )
			{
				vec2f Direction;
				vec2f Contact;

				Box* tempBox = (Box*)m_pParent->GetCollidableObject();

				if( tempBox )
				{
					if( tempBox->BoxToAABB( (*(AABB*)m_pTarget->GetCollidableObject()), Contact, Direction) )
					{
						if( m_pParent->GetType() == OBJ_CORK )
						{
							CComponent_CorkCollision* corkColl = (CComponent_CorkCollision*)m_pParent->GetComponent( ECOMP_COLLISION );
							if( corkColl && corkColl->GetCorked() )
							{
								return;
							}
						}

						m_bAttached = true;
						//pCollider->GetPhysicsBox()->NegateVelocity();
						pCollider->CleanRemoveVelocity();
						CComponent_Input* PlayersInput = (CComponent_Input*)m_pTarget->GetComponent(ECOMP_INPUT);
						PlayersInput->SetPulledObjectIsPulled(true);
						PlayersInput->SetPulledObjectType((eOBJECT_TYPE)m_pParent->GetType());
						PlayersInput->SetObjectPulled(NULL);
						CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
					}
				}
			}
		}
		else
		{
			//TODO: set velocity to zero
			if(pCollider->GetPhysicsBox())
			{
				vec3f TargetPos = m_pTarget->GetWorldPos();
				TargetPos.y += 50.0f;

				CComponent_Input* targetInput = (CComponent_Input*)m_pTarget->GetComponent (ECOMP_INPUT);

				if (targetInput)
				{
					if (targetInput->GetRotatedRight())
					{
						TargetPos.x += 80.0f;
					}
					else
					{
						TargetPos.x -= 80.0f;
					}

				}


				pCollider->GetPhysicsBox()->SetPosition(TargetPos);
				pCollider->GetPhysicsBox()->NegateVelocity();
			}
		}
	}
	else
	{
		m_bAttached = false;
	}
}