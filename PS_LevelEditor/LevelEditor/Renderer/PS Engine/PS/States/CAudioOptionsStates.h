/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CAudioOptionsState
// Purpose: To control audio levels for our game
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CAUDIOOPTIONSSTATE_H_
#define CAUDIOOPTIONSSTATE_H_

#include "IGameState.h"
#include "../../Utilities/MathLib/vec2.h"

enum eAUDIO_ITEMS{ AUDIO_SFX, AUDIO_MUSIC, AUDIO_DIALOGUE, AUDIO_EXIT };

#define AMENU_MIN_X		0.374065f
#define AMENU_MAX_X		0.855362f
#define ASFX_MIN_Y		0.710723f
#define ASFX_MAX_Y		0.940150f
#define AMUSIC_MIN_Y	0.431421f
#define AMUSIC_MAX_Y	0.638404f
#define ADIALOGUE_MIN_Y	0.162095f
#define ADIALOGUE_MAX_Y 0.384040f
#define AEXIT_MIN_Y		0.0f
#define AEXIT_MAX_Y		0.099751f

#define ASLIDER_WIDTH			0.030f
#define ASLIDER_HEIGHT			0.075f
#define ASLIDER_BAR_WIDTH		0.4255f
#define ASLIDER_MAX				0.8105f
#define ASLIDER_MIN				0.385f

#define ASLIDER_DOWN_WIDTH		0.025f
#define ASLIDER_DOWN_HEIGHT		0.070f
#define ASLIDER_DOWN_MAX		0.81f
#define ASLIDER_DOWN_MIN		0.3845f

#define ASLIDER_SFX_POSY		0.725f
#define ASLIDER_MUSIC_POSY		0.456f
#define ASLIDER_DIALOGUE_POSY	0.184f


class CGame;
class CInputManager;
class CAudioManager;

__declspec(align(32))
class CAudioOptionsState : public IGameState
{
private:
	static CAudioOptionsState* m_pInstance;
	CGame* m_pGame;
	CInputManager* m_pInput;
	CAudioManager* m_pAudio;

	float m_fPlayTimer;

	int m_nCurrentlySelected;
	LPDIRECT3DTEXTURE9 m_lpCurrentSelection;

	//TEXTURES TO RENDER
	// Backgrounds
	LPDIRECT3DTEXTURE9 m_lpOptionsAudioSound;
	LPDIRECT3DTEXTURE9 m_lpOptionsAudioMusic;
	LPDIRECT3DTEXTURE9 m_lpOptionsAudioDiag;
	LPDIRECT3DTEXTURE9 m_lpOptionsAudioExit;
	LPDIRECT3DTEXTURE9 m_lpOptionsSlider;

	LPDIRECT3DTEXTURE9 m_nReticle;

	// mouse vector
	D3DXVECTOR4 _mousePos;

	// SFX slider position
	vec2f m_fSFXSliderPosVec;
	// Music slider position
	vec2f m_fMusicSliderPosVec;
	// Dialogue slider position
	vec2f m_fDialogueSliderPosVec;

	bool m_bPrevDown;


private:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: COptionsState
	// Purpose: Default Constructor.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	CAudioOptionsState();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: ~COptionsState
	// Purpose: Default Destructor.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	~CAudioOptionsState();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: COptionsState Copy Constructor
	// Purpose: Default copy constructor.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	CAudioOptionsState( const CAudioOptionsState& obj );

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: operator =
	// Purpose: Assignment Operator.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:	
	// Last Modification Date:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	CAudioOptionsState& operator = ( const CAudioOptionsState& obj);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: GetInstance
	// Purpose: Returns the instance of the Main Menu.
	// Original Author: Rueben Massey
	// Creation Date: 5/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	static CAudioOptionsState* GetInstance ();

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

#endif // CAUDIOOPTIONSSTATE_H_