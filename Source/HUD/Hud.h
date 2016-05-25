/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CHud
//
// Purpose: To display the Sludge meter to the screen
//
// Original Author: Rueben Massey
//
// Creation Date: 5/14/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HUD_H_
#define HUD_H_

#include "..//PS/CGame.h"
#include "CBitmapFont.h"
#include"../EffectsLib/ParticleManager.h"

class IBaseObject;
class CComponent_Input;
class TextureManager;

#define MAX_SLUDGE 100
#define MAX_PULLED_OBJECTS 4
#define CYCLONE_IMAGES 4
#define NUM_HUD_TEXTURES 8
#define NUM_OBJECT_HUD_TEXTURES 8
#define NUM_PANEL_TEXTURES 5

enum OBJECT_IMAGES { HUD_CORK, HUD_CRATE, HUD_GOO, HUD_PLUG };

__declspec(align(32))
class CHud
{
private:
	float m_fSludgeLevel;
	float m_fBossLife;

	unsigned int m_currentPulledObject;
	unsigned int m_currentCycloneTexture;
	IBaseObject* m_pPlayer;
	CComponent_Input* m_pPlayerInput;
	TextureManager* m_pTextureMngr;

	// Variables for HUD animations
	// Pudge HUD
	float m_fRotationTimer;
	int m_nRotationIndex;
	// Pulled Object Panel
	float m_fPanelTimer;
	int m_nPanelIndex;
	// Pulled Object Item


	// Bitmap Font
	CBitmapFont m_cBitmapFont;
	
	// Texture Arrays
	int m_nPudgeHUD[NUM_HUD_TEXTURES];
	int m_nObjectHUD[MAX_PULLED_OBJECTS][NUM_OBJECT_HUD_TEXTURES];
	int m_nPanelHUD[NUM_PANEL_TEXTURES];	
	// Background
	int m_nBackgroundHUD;
	
	int m_HUDBossHealthTextureHandle;
	int m_HUDBossOutlineTextureHandle;

	// Particles
	CPM_ParEmitter* m_SludgeParticles;

public:

	CHud();

	~CHud() {}

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	float* GetSludgeLevel () { return &m_fSludgeLevel; }
	float GetBossHealth () { return m_fBossLife; }


	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetSludgeAmount( float fSludgeAmount ) { m_fSludgeLevel = 100.0f - fSludgeAmount; }
	void SetBossHealth ( float fBossHealth ) { m_fBossLife = fBossHealth;}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Init()
	// Purpose: Initializes data members
	// Original Author: Rueben Massey
	// Creation Date: 5/14/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown()
	// Purpose: Cleans up memory
	// Original Author: Rueben Massey
	// Creation Date: 8/2/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update()
	// Purpose: update the meter over time
	// Original Author: Rueben Massey
	// Creation Date: 5/14/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fElapsedTime );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Render()
	// Purpose: Render the sludge meter to the screen
	// Original Author: Rueben Massey
	// Creation Date: 5/14/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render();
};

#endif// HUD_H_