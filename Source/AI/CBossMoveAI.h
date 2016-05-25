//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CBossAttackAI.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This is a virtual class for all ai
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _BOSSMOVEAI_H
#define _BOSSMOVEAI_H

#include "IBaseAI.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../ObjectLib/Objects/IBaseObject.h"

class CComponent_Animation;

class BossMoveAI : public IBaseAI
{
private:
	Line m_LineToCeling;
	Line m_LineToRight;
	Line m_LineToLeft;

	std::vector<vec3f> m_vPlayerPos;
	std::vector<vec3f> m_vSludgePos;
	std::vector<vec3f> m_vSludgeTpPos;
	
	int m_nCurArea;

	bool m_bPlayed;				//this bool tells us if a certain sound played

	Sphere m_FloatSphere;
	vec3f m_v3MoveDir;

	float m_fElapsedTime;
	bool m_bFadeScreen;

	// Pointer To Sludge and His Ship. NOTE: AI Updates his Ship not Pudge.
	IBaseObject* pSludge;
	IBaseObject* pSludgesShip;

	CComponent_Animation* pAnimComp;

	float m_fFadeTime;

	bool m_bFadeOut;
	bool m_bFadeIn;
	bool m_bDead;

	bool m_bMaxHeightReached;
	bool m_bInNextArea;

	void Death();
	void MoveToNextArea();

	void ChecKCelingCollision( AABB _Volume );
	void CheckWallCollision( AABB _Volume );
	void FadeScreen();

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BossAttackAI(IBaseObject* _pParent):	Default Constructor that takes in the parent object.
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BossMoveAI( IBaseObject* _pParent );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ~BossAttackAI():	default destructor
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~BossMoveAI();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Init():	init all member variables
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shutdown():	delete allocated memory and uninit member variables
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update(float _fElapsedTime):	Update the Boss Attack ai to preform 1 or 3 attacks
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float _fElapsedTime );

	///////////////////////////////////////
	//Accessors
	///////////////////////////////////////
	vec3f GetMoveDir() { return  m_v3MoveDir; }
	bool GetFadeScreen() { return m_bFadeScreen; }


	///////////////////////////////////////
	//Mutators
	///////////////////////////////////////
	void SetMoveDir( vec3f _vDir ) { m_v3MoveDir = _vDir; }
	void SetFadeScreen( bool _bFadeScreen ) { m_bFadeScreen = _bFadeScreen; }
	void SetDeath( bool _bDead ) { m_bDead = _bDead; }
	void SetPlayed(bool _played) { m_bPlayed = _played; }

};
#endif