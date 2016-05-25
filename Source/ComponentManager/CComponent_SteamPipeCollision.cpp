//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_SteamPipeCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/CWinGameState.h"
#include "../PS/CGame.h"
#include "../AudioLib/AudioManager.h"
#include "CComponent_CorkCollision.h"
#include "Component_Emitter.h"
#include "../EffectsLib/ParticleManager.h"
#include "../Utilities/KdTree/KdTree.h"

CComponent_SteamPipeCollision::CComponent_SteamPipeCollision()
{
	m_cvSteam.SetVolumeType(VMT_AABB);

	m_bSteamOn = true;
	m_bCurrentlyLooping = false;
	m_bCorked = false;
	m_bConstant = true;
	m_fFrequencyTimer = 0.0f;
	m_fDurationTimer = 0.0f;

	m_pSoundComponent = nullptr;
	m_pPlayer = NULL;
	m_pObjectMngr = NULL;
	indexForActivePart = 0;
	m_pCollisionObject = nullptr;
	m_bPrevOff = false;


	m_SteamDir = vec3f(0.0f, 0.0f, 0.0f);
	index = -1;
	m_bConstant = false;
	m_dDuration = 0.0;
	m_dFrequency = 0.0;
	m_nChannel = -1;
	m_nLength = -1;
	m_pSteamEffect = nullptr;

	this->CleanRemoveVelocity();
}

CComponent_SteamPipeCollision::~CComponent_SteamPipeCollision(void)
{
}

bool CComponent_SteamPipeCollision::Initialize ()
{
	m_cvSteam.SetVolumeType(VMT_AABB);

	m_bSteamOn = true;
	m_bCurrentlyLooping = false;
	m_bCorked = false;
	m_bConstant = true;
	m_fFrequencyTimer = 0.0f;
	m_fDurationTimer = 0.0f;

	m_pSoundComponent = nullptr;
	m_pPlayer = NULL;
	m_pObjectMngr = NULL;
	m_pCollisionObject = nullptr;

	m_SteamDir = vec3f(0.0f, 0.0f, 0.0f);
	index = -1;
	m_bConstant = false;
	m_dDuration = 0.0;
	m_dFrequency = 0.0;
	m_nChannel = -1;
	m_nLength = -1;

	this->CleanRemoveVelocity();

	CComponent_Collision::Initialize();

	m_pObjectMngr = CObjectManager::GetInstance();

	m_pSteamEffect = CParticleManager::GetInstance()->CreateEffect( "SteamPipe", vec3f( 0.0f, 0.0f, 0.0f ) );

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));		

	// Set the collision object 
	// to the parent's collision object 
	m_pCollisionObject = (AABB*)m_pParent->GetCollidableObject();		

	return true;
}

void CComponent_SteamPipeCollision::Render()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: Update data for this component
// Original Author: Rueben Massey
// Creation Date: 6/6/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SteamPipeCollision::Update( float fDT )
{
	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	m_cvSteam.SetWorldPoint( NewCenter );
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint( NewCenter );

	// handle collision with:
	// Player
	// Throwables
	// Entities
	CheckCollisions( fDT );

	// Only check for steam
	// if the intake is not corked
	if( !m_bCorked )
	{
		// Set steam on/off if 
		// the steam isn't constant
		if( !m_bConstant )
		{
			// Run interval steam collisions
			if (m_bSteamOn)
			{
				m_fDurationTimer += fDT;

				if (m_fDurationTimer >= m_dDuration)
				{
					m_fDurationTimer = 0.0f;
					m_bSteamOn = !m_bSteamOn;
				}
			}
			else
			{
				m_fFrequencyTimer += fDT;
				if( m_fFrequencyTimer >= m_dFrequency )
				{
					m_fFrequencyTimer = 0.0f;
					m_bSteamOn = !m_bSteamOn;
				}
			}
		}
		// check collisions
		// if the steam is on
		if( m_bSteamOn )
		{
			// play steam sound
			if( !m_bCurrentlyLooping && m_pSoundComponent)
			{
				if(m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP))
				{
					m_bCurrentlyLooping = true;
					m_bPrevOff = false;
				}
			}
			CheckSteamCollisions( fDT );
		}
		else
		{
			//Stop sound
			if( m_pSoundComponent && !m_bPrevOff )
			{
				m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP);
				m_bCurrentlyLooping = false;
				m_bPrevOff = true;
			}
		}
	}
	else
	{
		//Stop sound
		if( m_pSoundComponent )
		{
			m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP);
			m_bCurrentlyLooping = false;
		}
		m_bSteamOn = false;
	}

	// Emitting steam.
	if( m_bSteamOn )
	{
		m_pSteamEffect->m_fPosition = GetParent()->GetWorldPos();
		m_pSteamEffect->m_fDirection = m_SteamDir;
		m_pSteamEffect->m_fSpeed = (float)(m_nLength * GAME_UNIT) * 2.0f;
		m_pSteamEffect->Emit( fDT );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: BuildSteamAABB()
// Purpose: Create an AABB for the steam
// Original Author: Rueben Massey
// Creation Date: 6/5/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SteamPipeCollision::BuildSteamAABB( int nLength, int nDirection )
{
	m_nLength = nLength;
	switch( nDirection)
	{
	case 0:
		{
			m_cvSteam.SetMaxPoint (vec2f ( GAME_UNIT * 0.5f, STEAM_AABB_OFFSET + (GAME_UNIT * (float)nLength)) );
			m_cvSteam.SetMinPoint (vec2f ( -GAME_UNIT * 0.5f, STEAM_AABB_OFFSET ));
			m_SteamDir = vec3f( 0.0f, 1.0f, 0.0f );			
			break;
		}
	case 1:
		{
			m_cvSteam.SetMaxPoint (vec2f ( GAME_UNIT * 0.5f, -STEAM_AABB_OFFSET ));
			m_cvSteam.SetMinPoint (vec2f ( -GAME_UNIT * 0.5f, -STEAM_AABB_OFFSET -  (GAME_UNIT * (float)nLength)) );
			m_SteamDir = vec3f( 0.0f, -1.0f, 0.0f );
			break;
		}
	case 2:
		{
			m_cvSteam.SetMaxPoint (vec2f ( -STEAM_AABB_OFFSET, GAME_UNIT * 0.5f ));
			m_cvSteam.SetMinPoint (vec2f ( -STEAM_AABB_OFFSET - (GAME_UNIT * (float)nLength), -GAME_UNIT * 0.5f ) );
			m_SteamDir = vec3f( -1.0f, 0.0f, 0.0f );
			break;
		}
	case 3:
		{
			m_cvSteam.SetMaxPoint (vec2f ( STEAM_AABB_OFFSET + (GAME_UNIT * (float)nLength), GAME_UNIT * 0.5f ));
			m_cvSteam.SetMinPoint (vec2f ( STEAM_AABB_OFFSET, -GAME_UNIT * 0.5f ));
			m_SteamDir = vec3f( 1.0f, 0.0f, 0.0f );
			break;
		}
	};
	m_SteamLine.SetStartPoint( vec2f( GetPos().x, GetPos().y ) );
	m_SteamLine.SetEndPoint( vec2f( GetPos().x + m_SteamDir.x * (nLength * GAME_UNIT)  , GetPos().y + m_SteamDir.y * (nLength * GAME_UNIT) ) );
}

void CComponent_SteamPipeCollision::UpdateSteamLine (vec3f _v3Pos)
{
	m_SteamLine.SetStartPoint( vec2f( _v3Pos.x, _v3Pos.y ) );
	m_SteamLine.SetEndPoint( vec2f( _v3Pos.x + m_SteamDir.x * (m_nLength * GAME_UNIT)  , _v3Pos.y + m_SteamDir.y * (m_nLength * GAME_UNIT) ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckSteamCollisions()
// Purpose: Check for collision with the steam
//			and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/6/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SteamPipeCollision::CheckSteamCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER)|(1<<OBJ_PLUG)|(1<<OBJ_CRATE)|(1<<OBJ_CORK)|(1<<OBJ_GOO);
	

	// call the kd tree and get the near objects
	CSceneObject cSteam;
	cSteam.SetCollidableObject(&m_cvSteam);
	CKdTree::GetNearObjects ( &cSteam, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);

	// Item Collisions
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{
			// Check if the body type is dynamic
			// meaning it's a moving object in the game
			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				// collision checks
				// based on volume type
				switch( curVolume->GetVolumeType() )
				{
				case VMT_AABB:
					{
						vec2f Direction;
						vec3f CP;
						if( ((AABB*)curVolume)->AABBToAABB( m_cvSteam, CP, Direction ) )
						{
							// We have collided
							// Do stuff to the object

							// if we have a physics box
							if( curCollision->GetPhysicsBox() )
							{
								// Apply force to the object
								curCollision->GetPhysicsBox()->ApplyUniformForce( m_SteamDir, STEAM_FORCE * fDT );
								// push the object out from the middle of the steam
								// based on how close it is to the middle

								// Get the closest point
								// to a line that represents the
								// center of the steam
								vec2f closestpoint;
								m_SteamLine.ClosestPointOnA2DLine( vec2f( curCollision->GetPhysicsBox()->GetCentroid().x, curCollision->GetPhysicsBox()->GetCentroid().y ), closestpoint );

								// Get a vector
								// from the closest point
								// to the object
								vec2f vecToObj = vec2f( curCollision->GetPhysicsBox()->GetCentroid().x, curCollision->GetPhysicsBox()->GetCentroid().y ) - closestpoint;
								// get the magnitude
								// used for applying force
								float dist = vecToObj.magnitude();
								// normalize the vector
								// so it can be used to apply 
								// force in the correct direction
								vecToObj.normalize();								

								// apply force
								// away from the center line
								// based on how close the object is
								// to the center line
								// the closer the greater the force
								curCollision->GetPhysicsBox()->ApplyUniformForce( vec3f(vecToObj.x, vecToObj.y, 0.0f), (STEAM_PUSHOUT / dist) * fDT );
							}
							else
							{
								if( -1.0f != m_SteamDir.y )
									curCollision->GetVel() += m_SteamDir * (PLAYER_STEAM_FORCE * fDT);
								else
								{
									// We have collided
									// Do stuff to the object
									// if we have a physics box
									// Apply force to the object
									//curCollision->GetPhysicsBox()->ApplyUniformForce( m_SteamDir, STEAM_FORCE * fDT );
									vec2f steamDirWrapper = vec2f( m_SteamDir.x, m_SteamDir.y );
									curCollision->CleanForce( steamDirWrapper * STEAM_FORCE * fDT );
									// push the object out from the middle of the steam
									// based on how close it is to the middle

									// Get the closest point
									// to a line that represents the
									// center of the steam
									vec2f closestpoint;
									m_SteamLine.ClosestPointOnA2DLine( curCollision->GetPos2DClean(), closestpoint );

									// Get a vector
									// from the closest point
									// to the object
									vec2f vecToObj = vec2f( curCollision->GetPos2DClean() ) - closestpoint;
									// get the magnitude
									// used for applying force
									float dist = vecToObj.magnitude();
									// normalize the vector
									// so it can be used to apply 
									// force in the correct direction
									vecToObj.normalize();								

									// apply force
									// away from the center line
									// based on how close the object is
									// to the center line
									// the closer the greater the force
									//curCollision->GetPhysicsBox()->ApplyUniformForce( vec3f(vecToObj.x, vecToObj.y, 0.0f), (STEAM_PUSHOUT / dist) * fDT );
									curCollision->CleanForce( vec2f(vecToObj.x, vecToObj.y) * (STEAM_PUSHOUT / dist) * fDT );
								}
							}
						}
						break;
					}
				case VMT_BOX:
					{
						vec2f Direction;
						vec2f CP;
						if( ((Box*)curVolume)->BoxToAABB( m_cvSteam, CP, Direction ) )
						{
							// We have collided
							// Do stuff to the object
							// if we have a physics box
							// Apply force to the object
							//curCollision->GetPhysicsBox()->ApplyUniformForce( m_SteamDir, STEAM_FORCE * fDT );
							vec2f steamDirWrapper = vec2f( m_SteamDir.x, m_SteamDir.y );
							curCollision->CleanForce( steamDirWrapper * STEAM_FORCE * fDT );
							// push the object out from the middle of the steam
							// based on how close it is to the middle

							// Get the closest point
							// to a line that represents the
							// center of the steam
							vec2f closestpoint;
							m_SteamLine.ClosestPointOnA2DLine( curCollision->GetPos2DClean(), closestpoint );

							// Get a vector
							// from the closest point
							// to the object
							vec2f vecToObj = vec2f( curCollision->GetPos2DClean() ) - closestpoint;
							// get the magnitude
							// used for applying force
							float dist = vecToObj.magnitude();
							// normalize the vector
							// so it can be used to apply 
							// force in the correct direction
							vecToObj.normalize();								

							// apply force
							// away from the center line
							// based on how close the object is
							// to the center line
							// the closer the greater the force
							//curCollision->GetPhysicsBox()->ApplyUniformForce( vec3f(vecToObj.x, vecToObj.y, 0.0f), (STEAM_PUSHOUT / dist) * fDT );
							curCollision->CleanForce( vec2f(vecToObj.x, vecToObj.y) * (STEAM_PUSHOUT / dist) * fDT );
						}
						break;
					}				
				default:
					break;
				};
			}
		}
	}
}
void CComponent_SteamPipeCollision::Shutdown(void)
{
	if(m_pSoundComponent)
	{
		m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP);
	}

	CParticleManager::GetInstance()->DestroyEffect( m_pSteamEffect );

	m_bSteamOn = true;
	m_bCurrentlyLooping = false;
	m_bCorked = false;
	m_bConstant = true;
	m_fFrequencyTimer = 0.0f;
	m_fDurationTimer = 0.0f;

	m_pSoundComponent = nullptr;
	m_pPlayer = NULL;
	m_pObjectMngr = NULL;
	indexForActivePart = 0;
	m_pCollisionObject = nullptr;
	m_bPrevOff = false;


	m_SteamDir = vec3f(0.0f, 0.0f, 0.0f);
	index = -1;
	m_bConstant = false;
	m_dDuration = 0.0;
	m_dFrequency = 0.0;
	m_nChannel = -1;
	m_nLength = -1;
	m_pSteamEffect = nullptr;

	this->CleanRemoveVelocity();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision with the steampipe
//			and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SteamPipeCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER)|(1<<OBJ_PLUG)|(1<<OBJ_CRATE)|(1<<OBJ_CORK)|(1<<OBJ_GOO);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{
			// Check if the body type is dynamic
			// meaning it's a moving object in the game
			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				CheckDynamicCollisions( curCollision, curVolume, fDT );
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SteamPipeCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	// Variables to catch returns
	vec2f Direction;
	vec2f CP;

	// Check collisions
	// based on the current objects collision volume
	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((AABB*)pCurVolume)->AABBToAABB( *m_pCollisionObject, Direction ) )
			{
				// Apply push-out
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );
				// update the AABB
				((AABB*)pCurCollision->GetParent()->GetCollidableObject())->SetWorldPoint( pCurCollision->GetPos2DClean() );
				// set the player to grounded
				// if he is on top of us
				if( OBJ_PLAYER == pCurCollision->GetParent()->GetType() )
				{
					if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
					{
						((CComponent_PlayerCollision*)pCurCollision)->SetCollidingGroundAlone( true );
					}
				}
			}
			break;
		}
	case VMT_BOX:
		{

			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((Box*)pCurVolume)->BoxToAABB( *m_pCollisionObject, CP,Direction ) )
			{
				IBaseObject* colParent = pCurCollision->GetParent();
				// get the item component of this object
				CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);

				// if its being pulled
				if( itemcomp && itemcomp->GetTarget() )
				{
					// stop the player from pulling
					itemcomp->SetTarget( NULL );
					//pCurCollision->CleanRemoveVelocity();
				}
				
				// special case for Lil Goos
				if(pCurCollision->GetParent()->GetType() == OBJ_GOO)
				{
					CComponent_LittleGooCollision* pLilGoo = (CComponent_LittleGooCollision*)pCurCollision;
					
					if(pLilGoo->GetVolitile() && pLilGoo->GetParent()->GetAlive())
					{
						pLilGoo->GetParent()->SetAlive(false);
						pCurCollision->CleanRemoveVelocity();
					}
				}	

				// set the Box to grounded
				// if he is colliding with us
				if( OBJ_CRATE == pCurCollision->GetParent()->GetType() )
				{
					((CComponent_CrateCollision*)pCurCollision)->SetCollidingGroundAlone( true );
				}

				// Apply push-out
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );

			}
			break;
		}
	default:
		break;
	};
}
