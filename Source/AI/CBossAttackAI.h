//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CBossAttackAI.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This is a virtual class for all ai
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _BOSSATTACKAI_H
#define _BOSSATTACKAI_H

#include "IBaseAI.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../ComponentManager/CComponent_SteamPipeCollision.h"

class CComponent_Animation;

#define MAXSTAGES 3

class BossAttackAI : public IBaseAI
{
private:
	struct AltAttackData
	{
		// x = min time, y = max time
		vec2f Timing;
		std::vector<vec2f> GooDropVels;
	};

	struct AltAttackInfo
	{
		int NumAltData;
		int CurAltData;
		AltAttackData* AltData;
	};

CPM_ParEmitter* m_pCrusherSteamEffect;

	// This is the information for the 3 Alt Goo Drops, Each Alt has attack data which tells it when to drop the goos and how many to drop durning that time
	AltAttackInfo AltInfomation[MAXSTAGES];
	float SteamAttackDis[2];

	typedef void (BossAttackAI::*AttackStage)();
	typedef void (BossAttackAI::*AttackFunc)();

	Sphere m_FloatSphere;
	Line m_LineToCeling;
	Line m_LineToRight;
	Line m_LineToLeft;
	Line m_DropGooLine;
	Line m_AltLineToLeft;
	Line m_AltLineToRight;
	std::vector<IBaseObject*> m_vSpawnedGoos;
	CComponent_SteamPipeCollision m_SteamCol;

	//bool m_bPrepareGooDrop;
	//bool m_bPrepareAlt;
	//bool m_bPrepareSteam;
	//bool m_bPrepareCursherAttack;
	//
	//bool m_bDropGoo;
	//bool m_bAltAttack;
	//bool m_bSteamOn;
	//bool m_bCrusherAttack;

	float m_fDropGoosTime;
	float m_fCornerTimer;
	int m_nNumGoosDroped;
	float m_fAltTimer;
	int m_nCrusherDrops;

	vec3f m_v3MoveDir;
	float m_fPrevDistance;
	float m_fElapsedTime;

	bool m_bCollideWithWall;	
	bool m_bPlayed;				//This bool is to tell us that a sound has previously played

	// Pointer To Sludge and His Ship. NOTE: AI Updates his Ship not Pudge.
	IBaseObject* pSludge;
	IBaseObject* pSludgesShip;

	CComponent_Animation* pAnimComp;
	bool m_bMaxHeightReached;
	int m_nCurStage;
	AttackStage m_Stages[MAXSTAGES];
	AttackFunc m_CurrentAttack;

	void CheckCollisions ();
	void RemoveGoos ();
	void CleanSpawnedVector ();
	void CheckCeilingCollision( AABB _Volume );
	void CheckWallCollision( AABB _Volume );


	bool CheckDropGooLine ();
	void DropGooAttack( vec2f Vel );
	float FollowObject( vec3f _v3Pos );

	void StageOne();
	void StageTwo();
	void StageThree();

	void PrepareGooAttack();
	void PrepareGooAlt ();
	void PrepareSteamAttack ();
	void PrepareCrusherAttack ();

	void GooAttack ();
	void AltGooAttack ();
	void AltGooAttack2 ();
	void AltGooAttack3 ();
	void SteamAttack ();
	void SteamAttack2 ();
	void CrusherAttack ();

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BossAttackAI(IBaseObject* _pParent):	Default Constructor that takes in the parent object.
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BossAttackAI(IBaseObject* _pParent);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ~BossAttackAI():	default destructor
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~BossAttackAI();

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
	void Update(float _fElapsedTime);

	///////////////////////////////////////
	//Accessors
	///////////////////////////////////////
	vec3f GetMoveDir() { return  m_v3MoveDir; }

	///////////////////////////////////////
	//Mutators
	///////////////////////////////////////
	void SetMoveDir (vec3f _vDir) { m_v3MoveDir = _vDir; }

};
#endif