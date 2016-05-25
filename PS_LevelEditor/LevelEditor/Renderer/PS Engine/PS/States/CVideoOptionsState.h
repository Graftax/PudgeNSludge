/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: MainMenuState
// Purpose: This is the Main Menu State
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CVIDEOOPTIONSSTATE_H_
#define CVIDEOOPTIONSSTATE_H_

#include "IGameState.h"
#include "../../Utilities/MathLib/vec2.h"

enum eVIDEO_ITEMS{ VIDEO_BRIGHTNESS, VIDEO_GAMMA, VIDEO_FS, VIDEO_EXIT, /*VIDEO_FS_ON, VIDEO_FS_OFF*/ };

#define VMENU_MIN_X					0.349127f
#define VMENU_MAX_X					0.905237f
#define VBRIGHT_MIN_Y				0.678304f
#define VBRIGHT_MAX_Y				0.912718f
#define VGAMMA_MIN_Y				0.394015f
#define VGAMMA_MAX_Y				0.581047f
#define VFULLSCREEN_MIN_Y			0.099751f
#define VFULLSCREEN_MAX_Y			0.216958f
#define VFULLSCREEN_ON_MIN_X		0.473815f
#define VFULLSCREEN_ON_MAX_X		0.633415f
#define VFULLSCREEN_OFF_MIN_X		0.633416f
#define VFULLSCREEN_OFF_MAX_X		0.820449f
#define VEXIT_MIN_Y					0.0f
#define VEXIT_MAX_Y					0.084788f

#define VSLIDER_WIDTH				0.030f
#define VSLIDER_HEIGHT				0.075f
#define VSLIDER_BAR_WIDTH			0.5185f
#define VSLIDER_MAX					0.8775f
#define VSLIDER_MIN					0.359f

#define VSLIDER_DOWN_WIDTH			0.025f
#define VSLIDER_DOWN_HEIGHT			0.070f
#define VSLIDER_DOWN_MAX			0.8770f
#define VSLIDER_DOWN_MIN			0.3585f

#define VCHECK_ON_MIN_X				0.546135f
#define VCHECK_ON_MIN_Y				0.094763f
#define VCHECK_OFF_MIN_X			0.720698f
#define VCHECK_OFF_MIN_Y			0.109726f
#define VCHECKMARK_WIDTH			0.137157f
#define VCHECKMARK_HEIGHT			0.159601f

//#define VSLIDER_BRIGHTNESS_POSY 0.683292f
#define VSLIDER_GAMMA_POSY		0.418593f

class CGame;
class CInputManager;
class CAudioManager;

__declspec(align(32))
class CVideoOptionsState : public IGameState
{
private:

	static CVideoOptionsState* m_pInstance;
	CGame* m_pGame;
	CInputManager* m_pInput;
	CAudioManager* m_pAudio;
	bool m_bSound;

	D3DXVECTOR4 _mousePos;
	int m_nCurrentlySelected;
	LPDIRECT3DTEXTURE9 m_lpCurrentSelection;

	bool m_bFullScreen;
	bool m_bPrevDown;

	//TEXTURES TO RENDER
	// Backgrounds
	//LPDIRECT3DTEXTURE9 m_lpOptionsVideoBright;
	LPDIRECT3DTEXTURE9 m_lpOptionsVideoGamma;
	LPDIRECT3DTEXTURE9 m_lpOptionsVideoExit;
	LPDIRECT3DTEXTURE9 m_lpOptionsVideoFS_On;
	LPDIRECT3DTEXTURE9 m_lpOptionsVideoFS_Off;
	LPDIRECT3DTEXTURE9 m_lpOptionsSlider;
	LPDIRECT3DTEXTURE9 m_lpOptionsCheck;
	
	// Reticle
	LPDIRECT3DTEXTURE9 m_nReticle;

	// Brightness slider position
	//vec2f m_fBrightnessSliderPosVec;
	// Gamma slider position
	vec2f m_fGammaSliderPosVec;
	// Check Mark
	vec2f m_fCheckMarkPos;

private:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: COptionsState
	// Purpose: Default Constructor.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	CVideoOptionsState();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: ~COptionsState
	// Purpose: Default Destructor.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	~CVideoOptionsState();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: COptionsState Copy Constructor
	// Purpose: Default copy constructor.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	CVideoOptionsState( const CVideoOptionsState& obj );

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: operator =
	// Purpose: Assignment Operator.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	CVideoOptionsState& operator = ( const CVideoOptionsState& obj);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: GetInstance
	// Purpose: Returns the instance of the Main Menu.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	static CVideoOptionsState* GetInstance ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: DeleteInstance
	// Purpose: Deletes the instance of the Main Menu.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DeleteInstance ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Init
	// Purpose: Initializes the state.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown
	// Purpose: Shuts down the state.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update
	// Purpose: Updates the state.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float fTime);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Input
	// Purpose: Gets input to update the state.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Input();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Render
	// Purpose: Render the state.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render();
};

#endif // CVIDEOOPTIONSSTATE_H_