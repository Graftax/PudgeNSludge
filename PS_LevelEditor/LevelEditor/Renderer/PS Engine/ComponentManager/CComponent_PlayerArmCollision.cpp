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
	float AngleBetween = dot_product( _worldMat.axis_y.negate(), PosToCursor);
	_localMat.rotate_x_pre( (AngleBetween) );

	// Set Local Matrix.
	m_pParent->SetLocalMat( &_localMat );
	// Dirty the Frame which gets updated later.
	m_pParent->GetFrame().Update();
#if _ANIMATIONS
	matrix4f animFrame = pAnimComp->GetAnimationFrames()[m_nFrameIndex].GetWorldMat();

	// Without this the Arm goes in the opposite direction that Pudge Goes
	animFrame.xy *= -1;
	animFrame.yy *= -1;
	animFrame.zy *= -1;
	animFrame.wy *= -1;
	

	// Multiply the Joint Frame with the World Mat to have the arm move by the animations as well.
	m_pParent->GetFrame().GetWorldMat() = (animFrame) * m_pParent->GetFrame().GetWorldMat();
#endif
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
	// Offset the object to the bind pose location to "place" the arm.
	m_pParent->GetWorldMat();
	matrix4f _localMat = (*m_pParent->GetLocalMat());
	D3DXMATRIX translate = pAnimComp->GetBindPose()->GetHierarchyList()[m_nFrameIndex]->m_InverseLocalMatrix;
	D3DXMATRIX matrix = _localMat.ma;
	translate._41 *= -1;
	translate._42 *= -1;
	translate._43 *= -1;

	D3DXMatrixMultiply( &matrix, &matrix, &translate );
	
	_localMat = (*(matrix4f*)&matrix);
	m_pParent->SetLocalMat( &_localMat );
	m_pParent->GetFrame().Update();
	m_pParent->GetWorldMat();
}