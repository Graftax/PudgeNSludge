//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#define IF(x) if(x)(x)

#include "CComponent_PlayerArmCollision.h"
#include "../PS/CGame.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/GamePlayState.h"
#include "../Utilities/KdTree/KdTree.h"

CComponent_PlayerArmCollision::CComponent_PlayerArmCollision()
{
	pAnimComp = NULL;
	m_nFrameIndex = 0;
	m_pGame = nullptr;
	m_pCamera = nullptr;
	m_nFrameIndex = -1;
	m_ObjectsParent = nullptr;
	this->CleanRemoveVelocity();

}

CComponent_PlayerArmCollision::~CComponent_PlayerArmCollision(void)
{

}

bool CComponent_PlayerArmCollision::Initialize ()
{
	pAnimComp = NULL;
	m_nFrameIndex = 0;
	m_pGame = nullptr;
	m_pCamera = nullptr;
	m_nFrameIndex = -1;
	m_ObjectsParent = nullptr;
	this->CleanRemoveVelocity();

	m_pGame = CGame::GetInstance();
	m_pCamera = m_pGame->GetCamera();
	CComponent_Collision::Initialize();
	return true;
}

void CComponent_PlayerArmCollision::Update( float fDT )
{

#if _ANIMATIONS

	// Grab the Animation Frame from the Component.
	matrix4f animFrame = pAnimComp->GetAnimationFrames()[m_nFrameIndex].GetWorldMat();

	// Grab the joints matrix from the Bind Pose and Calculate the inverse.
	matrix4f BoneMatrix = *(matrix4f*)&pAnimComp->GetBindPose()->GetHierarchyList()[m_nFrameIndex]->m_InverseLocalMatrix;
	Inverse( &BoneMatrix, &BoneMatrix );

	// Multiply the Bind Pose by the Animation frame to move the Joint from its Bind Pose to its Animation Frames position.
	m_pParent->GetFrame().GetLocalMat() = BoneMatrix * animFrame;
	// Apply a 180 degree rotation because the arms facing behind the player without this.
	m_pParent->GetFrame().GetLocalMat().rotate_y_pre( D3DXToRadian( 180.0f) );
	// Dirty the frame.
	m_pParent->GetFrame().Update();
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//			This will Calculate the degree of rotation for the arm to face down the Cursor.							   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( !CObjectManager::GetInstance()->GetPlayer()->GetAlive() )
		return;

	// Grab the parents world and local matrix.
	matrix4f _worldMat = (*m_pParent->GetWorldMat());
	matrix4f _localMat = (*m_pParent->GetLocalMat());

	// Grab the World cursor Position.
	vec3f _CursorPos = m_pCamera->GetWorldCursor();
	// Calculate Vector from your Position to the Cursor Position.
	vec3f PosToCursor =  _CursorPos - _worldMat.axis_pos;
	PosToCursor.normalize();
	// Dot the Forward vector of the arm with the vector calculated above to determine
	// how much the arm needs to rotate by.
	vec3f PlayerArmsY = _worldMat.axis_y;
	float AngleBetween = dot_product( PlayerArmsY, PosToCursor);
	_localMat.rotate_x_pre( (-AngleBetween) );

	// Set Local Matrix.
	m_pParent->SetLocalMat( &_localMat );
}

void CComponent_PlayerArmCollision::Shutdown(void)
{
}

void CComponent_PlayerArmCollision::SetObjectsParent(IBaseObject* obj )
{
	// We want to attach this arm to the Player.
	// So make this objects frame a child of the objects frame we wish
	// to attach to.
	obj->GetWorldMat();
	m_ObjectsParent = obj;
	obj->GetFrame().AddChild( (Node*)&m_pParent->GetFrame() );
	// Update the matrix to set it as dirty, and call the world matrix to update
	// it/flatten the hierarchy.
	obj->GetFrame().Update();
	m_pParent->GetWorldMat();
}

void CComponent_PlayerArmCollision::TranslateObjectByJoint()
{
	// Grab the joints matrix and get the inverse.
	matrix4f BoneMatrix = *(matrix4f*)&pAnimComp->GetBindPose()->GetHierarchyList()[m_nFrameIndex]->m_InverseLocalMatrix;
	Inverse( &BoneMatrix, &BoneMatrix );
	//BoneMatrix.translate_post( -9.0f, 0.0f, 0.0f );

	// Set the local matrix to this new matrix and it will translate the object to the joint in local space.
	m_pParent->SetLocalMat( &BoneMatrix );
	m_pParent->GetFrame().Update();
	m_pParent->GetWorldMat();
}