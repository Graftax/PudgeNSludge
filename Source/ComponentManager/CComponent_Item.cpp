#include "CComponent_Item.h"
#include "CComponent_Collision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"

bool CComponent_Item::Initialize(void)
{
	m_pTarget = NULL;
	m_bAttached = false;
	m_fSpawnIgnoreTimer = 0.0f;

	return true;
}

void CComponent_Item::Update( float fDT )
{
	if(!m_pParent || !m_pParent->GetAlive())
	{
		return;
	}

	if(m_fSpawnIgnoreTimer > 0.0f)
	{
		m_fSpawnIgnoreTimer -= fDT;
	}

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
			TargetPos.y += GAME_UNIT * 0.5f;

			// Calculate Distance from the Box Position to the Target Position.
			vec3f DistanceVector = m_pParent->GetWorldMat()->axis_pos - TargetPos;
			float SquaredDistance = dot_product( DistanceVector, DistanceVector );

			float MaxSquaredDistance = 60000.0f;
			// If the Squared Distance is greater than the preferred value, lets scale it down.
			if( SquaredDistance <= MaxSquaredDistance )
			{
				// Calculate Lamda based on the distance from the Target and the Max Distance from the target, that way the closer the object the smaller it is.
				float Lamda = (SquaredDistance / MaxSquaredDistance);
				// Simple Float Lerp.
				// Start + (End - Start) * Lambda;
				float BoxScaleLerp = 1.0f + ( 0.0f - 1.0f) * Lamda;

				// Store the World Matrix to Grab its rotation data.
				D3DXMATRIX ObjectsWorldMatrix = m_pParent->GetWorldMat()->ma;
				D3DXQUATERNION RotationQuaternion;
				// Dont need these but will use anyways to pass into the function.
				D3DXVECTOR3 Scale, Translation;
				D3DXMatrixDecompose(&Scale, &RotationQuaternion, &Translation, &ObjectsWorldMatrix );

				// Create a Matrix, Create a Scale based off values calculated, set the position and Set the Parents Matrix to this new matrix.
				matrix4f ManipulatedMatrix;
				D3DXMATRIX RotationMatrix;
				D3DXMATRIX ResultMatrix;

				// Create a Scale Matrix.
				ManipulatedMatrix.make_scale( 1.0f - BoxScaleLerp, 1.0f - BoxScaleLerp, 1.0f - BoxScaleLerp);
				// Create a Rotation Matrix.
				D3DXMatrixRotationQuaternion( &RotationMatrix, &RotationQuaternion );

				// Multiply the two.
				D3DXMatrixMultiply( &ResultMatrix, (D3DXMATRIX*)ManipulatedMatrix.ma, &RotationMatrix );
				// Store the Matrix back in the matrix4f.
				ManipulatedMatrix = (*(matrix4f*)&ResultMatrix);
				// Set the Position and place it back in the Parents Frame.
				ManipulatedMatrix.axis_pos = m_pParent->GetFrame().GetLocalMat().axis_pos;
				m_pParent->SetLocalMat( &ManipulatedMatrix );
			}

			vec3f fTempVel = pCollider->GetVel();
			fTempVel.negate();
			fTempVel *= 0.2f;
			pCollider->CleanForce( vec2f( fTempVel.x, fTempVel.y ) );

			vec3f fTowards = (TargetPos - m_pParent->GetWorldPos()).normalize();
			pCollider->CleanForce( vec2f( fTowards.x, fTowards.y ) * 500.0f * fDT );			

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
						pCollider->CleanRemoveVelocity();
						CComponent_Input* PlayersInput = (CComponent_Input*)m_pTarget->GetComponent(ECOMP_INPUT);
						PlayersInput->SetPulledObjectIsPulled(true);
						PlayersInput->SetPulledObjectType((eOBJECT_TYPE)m_pParent->GetType());
						if(pCollider->GetPhysicsBox())
						{
							pCollider->GetPhysicsBox()->SetPosition(vec3f(0.0f, -10000.0f, 0.0f));
						}
					}
				}
			}
		}
	}
	else
	{
		// Store the World Matrix to Grab its rotation data.
		D3DXMATRIX ObjectsWorldMatrix = m_pParent->GetWorldMat()->ma;
		D3DXQUATERNION RotationQuaternion;
		// Dont need these but will use anyways to pass into the function.
		D3DXVECTOR3 Scale, Translation;
		D3DXMatrixDecompose(&Scale, &RotationQuaternion, &Translation, &ObjectsWorldMatrix );

		if( Scale.x < 1.0f || Scale.y < 1.0f || Scale.z < 1.0f )
		{
			// Create a Matrix, Create a Scale based off values calculated, set the position and Set the Parents Matrix to this new matrix.
			matrix4f ManipulatedMatrix;
			D3DXMATRIX RotationMatrix;
			D3DXMATRIX ResultMatrix;

			// Create a Scale Matrix.
			ManipulatedMatrix.make_scale( 1.0f, 1.0f, 1.0f);
			// Create a Rotation Matrix.
			D3DXMatrixRotationQuaternion( &RotationMatrix, &RotationQuaternion );

			// Multiply the two.
			D3DXMatrixMultiply( &ResultMatrix, (D3DXMATRIX*)ManipulatedMatrix.ma, &RotationMatrix );
			// Store the Matrix back in the matrix4f.
			ManipulatedMatrix = (*(matrix4f*)&ResultMatrix);
			// Set the Position and place it back in the Parents Frame.
			ManipulatedMatrix.axis_pos = m_pParent->GetFrame().GetLocalMat().axis_pos;
			m_pParent->SetLocalMat( &ManipulatedMatrix );
		}
	}
}