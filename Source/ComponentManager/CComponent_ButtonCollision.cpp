#include "CComponent_ButtonCollision.h"
#include "../Utilities/KdTree/KdTree.h"
#include "CComponent_DoorCollision.h"
#include "CComponent_BoxPipeCollision.h"
#include "../RenderEngine/DebugShapes.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "CComponent_LittleGooCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "CComponent_Sound.h"
#include "../RenderEngine/TextureManager.h"

static short m_sIconCount = 0;
CComponent_ButtonCollision::CComponent_ButtonCollision(void)
{
	m_bPressed = m_bPrevPressed = false;
	m_pPartner = nullptr;
	m_nChannel = -1;
	m_nDirection = -1;
	m_pCollisionObj = nullptr;

	m_pAnimComponent = nullptr;
	m_pSoundComponent = nullptr;
	ButtonUp = true;
	m_bPrevDone = false;

	m_szWireMeshName = "";
	m_pWireMesh = nullptr;

	m_nWireTexture = 0;
	m_nScrollingWireTexture = 0;

	// Icon
	m_nIconTexture1 = 0;
	m_nIconTexture2 = 0;
	m_pIcon = nullptr;
	m_sCount = 0;
}

CComponent_ButtonCollision::~CComponent_ButtonCollision(void)
{

}

/////////////////////////////////////////////////////////////////////////
//	Initialize():	Used to initialize the values of member variables.
//	Returns:		bool - returns true if everything succeeded in
//					initializing.
//	Mod Date:		7/8/12
//	Mod Name:		Josh Morgan
/////////////////////////////////////////////////////////////////////////
bool CComponent_ButtonCollision::Initialize(void)
{
	CComponent_Collision::Initialize();
	m_bPressed = m_bPrevPressed = false;
	m_pPartner = nullptr;
	m_nChannel = -1;
	m_nDirection = -1;

	ButtonUp = true;
	m_bPrevDone = false;

	m_szWireMeshName = "";
	m_pWireMesh = nullptr;
	m_nScrollingWireTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/T_WireswithPower_D.png" );


	m_pCollisionObj = (AABB*)m_pParent->GetCollidableObject();
	m_pOriginalCollision = (*m_pCollisionObj);

	m_pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );

	m_pSoundComponent = (CComponent_Sound*)m_pParent->GetComponent( ECOMP_SOUND );

	// Icon
	m_nIconTexture1 = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/arrow1.png" );
	m_nIconTexture2 = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/arrow2.png" );
	m_pIcon = CIcon::GetInstance();
	m_sIconCount += 1;
	m_sCount = m_sIconCount;

	return true;
}

void CComponent_ButtonCollision::SetWireMeshName( std::string _szWireName )
{
	m_pWireMesh = nullptr;
	m_szWireMeshName = _szWireName;
	m_pWireMesh = MeshManager::GetInstance()->GetRenderNode( m_szWireMeshName.c_str() );
	if( m_pWireMesh )
	{
		m_nWireTexture = m_pWireMesh->GetRenderData().m_uiTextureHandles[0];
		m_pWireMesh->GetRenderData().m_uiTextureHandles[BURNDOWN_TEX] = m_nScrollingWireTexture;
		m_pWireMesh->ClearAllFlags();
		m_pWireMesh->AddFlags( RE_BURNOWNER | RE_BURNTEX | RE_UVS_PY );
	}
}

/////////////////////////////////////////////////////////////////////////
//	Update():		Updates and checks the current collisions of the 
//					button.
//	Returns:		void
//	Mod Date:		7/8/12
//	Mod Name:		Josh Morgan
/////////////////////////////////////////////////////////////////////////
void CComponent_ButtonCollision::Update(float _fDT)
{
	if(!m_pParent)
	{
		return;
	}

	//Icon
	if( m_sCount < 12 )
	{
		D3DXMATRIX scalemat;
		D3DXMatrixIdentity( &scalemat );
		D3DXMatrixScaling( &scalemat, 100.0f, 100.0f, 0.0f );
		m_pIcon->Render( m_nIconTexture1, m_nIconTexture2, m_pParent->GetFrame().GetWorldMat().axis_pos + vec3f( -50.0f, 100.0f, 0.0f ), scalemat );
	}

	if(m_bPressed)
	{
		m_bPrevPressed = true;		// I was JUST down
		m_bPressed = false;			// but for collision sake, I'm up again
		if( m_pWireMesh )
		{
			m_pWireMesh->SetExtraData( m_pWireMesh->GetExtraData() - _fDT );
			if( m_pWireMesh->GetExtraData() < 0.0f )
				m_pWireMesh->SetExtraData( 0.0f );
		}
	}
	else
	{
		if( m_pWireMesh )
		{
			m_pWireMesh->SetExtraData( m_pWireMesh->GetExtraData() + _fDT );
			if( m_pWireMesh->GetExtraData() > 1.0f )
				m_pWireMesh->SetExtraData( 1.0f );
		}
		m_bPrevPressed = false;
		ButtonUp = true;
	}

	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint( NewCenter );
	CheckCollisions(_fDT);

	if(m_bPressed && !m_bPrevPressed)
	{
#if _ANIMATIONS
		if( m_pAnimComponent )
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState( ButtonClosingState::GetState() );
			m_pAnimComponent->SetAnimationBackwards( false );
		}
#endif
	}
	else if(!m_bPressed && m_bPrevPressed)
	{
		//play sound and animation here for the button to go up, and if my buddy is a door, he should close
		if(m_pPartner && m_pPartner->GetType() == OBJ_DOOR)
		{
			((CComponent_DoorCollision*)m_pPartner->GetComponent(ECOMP_COLLISION))->SetPressed(false);
		}

		if(m_pSoundComponent && m_bPrevDone)
		{
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_BUTTON_CLICK);
		}

#if _ANIMATIONS
		if( m_pAnimComponent )
		{
			m_pAnimComponent->SetAnimationBackwards( true );
		}
#endif
	}

#if _ANIMATIONS
	if( m_pAnimComponent )
	{
		if(m_pSoundComponent && m_pAnimComponent->GetInterpolator().IsAnimationDone() && m_bPressed && !m_bPrevDone)
		{
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_BUTTON_CLICK);
		}

		//play sound and animation here for the button to go down, and trigger the partner activity
		if(m_pPartner && m_pAnimComponent->GetInterpolator().IsAnimationDone() && m_bPressed && !m_bPrevDone)
		{
			CParticleManager::GetInstance()->CreateEffectOneShot( "Sparks", GetParent()->GetWorldPos() + vec3f( 30.0f, 0.0f, 0.0f ) , vec3f( 0.5f, 0.5f, 0.0f ) );
			CParticleManager::GetInstance()->CreateEffectOneShot( "Sparks", GetParent()->GetWorldPos() + vec3f( -30.0f, 0.0f, 0.0f ) , vec3f( -0.5f, 0.5f, 0.0f ) );

			if(m_pPartner->GetType() == OBJ_DOOR)
			{
				((CComponent_DoorCollision*)m_pPartner->GetComponent(ECOMP_COLLISION))->SetPressed(true);
			}

			if(m_pPartner->GetType() == OBJ_PIPE_BOX)
			{
				((CComponent_BoxPipeCollision*)m_pPartner->GetComponent(ECOMP_COLLISION))->SetPressed(true);
			}
		}
	}
#endif

	DropButtonAABB( _fDT );

	m_bPrevDone = m_pAnimComponent->GetInterpolator().IsAnimationDone();
}

/////////////////////////////////////////////////////////////////////////
//	CheckCollisions():	Called by update to check the collisions.
//	Returns:			void
//	Mod Date:			7/8/12
//	Mod Name:			Josh Morgan
/////////////////////////////////////////////////////////////////////////
void CComponent_ButtonCollision::CheckCollisions( float _fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_PLUG) | (1<<OBJ_CORK) | (1<<OBJ_GOO);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects(m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);

	for(unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex)
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent(ECOMP_COLLISION));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();

		// Safety Check
		if(curVolume && curCollision && curCollision != this)
		{
			// Check if the body type is dynamic
			// meaning it's a moving object in the game
			if(BT_DYNAMIC == curVolume->GetBodyType())
			{
				CheckDynamicCollisions(curCollision, curVolume, _fDT);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//	CheckDynamicCollisions():	Checks for collisions with any debris
//								so it can be pushed out accordingly.
//	Returns:					void
//	Mod Date:					7/8/12
//	Mod Name:					Josh Morgan
/////////////////////////////////////////////////////////////////////////
void CComponent_ButtonCollision::CheckDynamicCollisions(	CComponent_Collision* _pCurCollision, 
	CCollisionVolume* _pCurVolume, 
	float _fDT )
{
	// Variables to catch returns
	vec2f Direction;
	vec2f CP;

	// Check collisions
	// based on the current objects collision volume
	switch( _pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((AABB*)_pCurVolume)->AABBToAABB( *((AABB*)(m_pParent->GetCollidableObject())), Direction))
			{
				vec3f NormalizedDirection = vec3f (Direction.x, Direction.y, 0.0f).normalize();
				float fDotProductUp = dot_product( NormalizedDirection, vec3f( 0.0f, 1.0f, 0.0f ) );
				if( fDotProductUp > 0.5f )
				{
					if(_pCurCollision->GetParent()->GetType() == OBJ_PLAYER)
					{
						m_bPressed = true;
						ButtonUp = false;
						_pCurCollision->SetCollidingGroundAlone(true);
					}
				}

				// Apply push-out
				_pCurCollision->CleanOffset( Direction );
				_pCurCollision->CleanForce( Direction * 0.5f );
			}
			break;
		}
	case VMT_BOX:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((Box*)_pCurVolume)->BoxToAABB(*((AABB*)(m_pParent->GetCollidableObject())), CP, Direction))
			{
				IBaseObject* colParent = _pCurCollision->GetParent();
				// get the item component of this object
				CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);

				// special case for Lil Goos
				if(_pCurCollision->GetParent()->GetType() == OBJ_GOO)
				{
					CComponent_LittleGooCollision* pLilGoo = (CComponent_LittleGooCollision*)_pCurCollision;
					
					if(pLilGoo->GetVolitile() && pLilGoo->GetParent()->GetAlive())
					{
						pLilGoo->GetParent()->SetAlive(false);
						pLilGoo->CleanRemoveVelocity();
					}

					if(pLilGoo->GetParent()->GetAlive() && !pLilGoo->GetCollidingGroundLast())
					{
						// Apply push-out
						//pLilGoo->CleanRemoveVelocity();
					}
				}	

				//// if its being pulled
				//if( itemcomp && itemcomp->GetTarget() )
				//{
				//	// stop the player from pulling
				//	itemcomp->SetTarget( NULL );
				//	//pCurCollision->CleanRemoveVelocity();
				//}
				tPhyObject* ObjectsPhysicsBox = _pCurCollision->GetPhysicsBox();

				// Create an offset vector.
				vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

				////////// FRICTION
				vec3f fCross;
				cross_product( fCross, MoveDirection, vec3f( 0.0f, 0.0f, 1.0f ) );
				float3 fProj = prjPoint2Line( ObjectsPhysicsBox->m_pPoints[0]->m_CurrPos, ObjectsPhysicsBox->m_pPoints[0]->m_PrevPos, ObjectsPhysicsBox->m_pPoints[0]->m_PrevPos + fCross );
				float3 fPrevPush = fProj - ObjectsPhysicsBox->m_pPoints[0]->m_PrevPos;
				fPrevPush *= -0.3f;
				//////////

				// Apply push-out
				_pCurCollision->CleanOffset(Direction);
				_pCurCollision->CleanForce( vec2f( fPrevPush.x, fPrevPush.y ) + Direction * 0.05f);

				vec3f NormalizedDirection = vec3f (Direction.x, Direction.y, 0.0f).normalize();
				float fDotProductUp = dot_product( NormalizedDirection, vec3f( 0.0f, 1.0f, 0.0f ) );
				if( fDotProductUp > 0.5f )
				{
					if(_pCurCollision->GetParent()->GetType() == OBJ_CRATE)// || _pCurCollision->GetParent()->GetType() == OBJ_GOO)
					{
						m_bPressed = true;
						ButtonUp = false;
					}
				}

				_pCurCollision->SetCollidingGroundAlone(true);
			}
			break;
		}
	default:
		break;
	};
}

/////////////////////////////////////////////////////////////////////////
//	Shutdown():		Cleans up any memory allocated, and resets variables.
//	Returns:		void
//	Mod Date:		7/8/12
//	Mod Name:		Josh Morgan
/////////////////////////////////////////////////////////////////////////
void CComponent_ButtonCollision::Shutdown(void)
{	
	m_bPressed = m_bPrevPressed = false;
	m_pPartner = nullptr;
	m_nChannel = -1;
	m_nDirection = -1;
	m_pCollisionObj = nullptr;

	m_pAnimComponent = nullptr;
	m_pSoundComponent = nullptr;
	ButtonUp = true;
	m_bPrevDone = false;

	m_szWireMeshName = "";
	m_pWireMesh = nullptr;

	m_nIconTexture1 = 0;
	m_nIconTexture2 = 0;

	m_pIcon->DeleteInstance();
	m_pIcon = nullptr;

	CComponent_Collision::Shutdown();
}

/////////////////////////////////////////////////////////////////////////
//	Update():		Updates the Collision when the Button drops.
//	Returns:		void
//	Mod Date:		7/12/12
//	Mod Name:		Andy Madruga
/////////////////////////////////////////////////////////////////////////
void CComponent_ButtonCollision::DropButtonAABB( float fDT )
{
	float DistanceToDrop = 13.0f;
	AABB ButtonDownCollision = m_pOriginalCollision;
	float ElapsedTime = fDT * 3.0f;

	ButtonDownCollision.SetMaxPoint( vec2f( m_pCollisionObj->GetMaxPoint2D().x, m_pCollisionObj->GetMaxPoint2D().y - DistanceToDrop * ElapsedTime) );
	ButtonDownCollision.SetMinPoint( vec2f( m_pCollisionObj->GetMinPoint2D().x - 25.0f, m_pCollisionObj->GetMinPoint2D().y - 25.0f) );
	// Button is up so raise the collision
	if( ButtonUp )
	{
		if( m_pCollisionObj->GetMaxPoint2D().y < m_pOriginalCollision.GetMaxPoint3D().y )
		{
			// Raise The Button Collision
			m_pCollisionObj->SetMaxPoint( vec2f( m_pCollisionObj->GetMaxPoint2D().x, m_pCollisionObj->GetMaxPoint2D().y + DistanceToDrop * ElapsedTime) );
		}
		else
			m_pCollisionObj->SetMaxPoint( m_pOriginalCollision.GetMaxPoint() );
	}
	// Button is not up so drop collision
	else
	{
		if( m_pCollisionObj->GetMaxPoint2D().y > (m_pOriginalCollision.GetMaxPoint3D().y - DistanceToDrop) )
		{
			// Lower The Button Collision
			m_pCollisionObj->SetMaxPoint( vec2f( m_pCollisionObj->GetMaxPoint2D().x, m_pCollisionObj->GetMaxPoint2D().y - DistanceToDrop * ElapsedTime) );
		}
		else
			m_pCollisionObj->SetMaxPoint( vec2f(m_pOriginalCollision.GetMaxPoint3D().x, m_pOriginalCollision.GetMaxPoint3D().y - DistanceToDrop) );
	}
}