/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: IGameState
// Purpose: This is a pure virutal state used for the base of all states.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IGAMESTATE_H
#define _IGAMESTATE_H

#include "../config.h"

enum GAMESTATES {MAIN_MENU_STATE, GAMEPLAY_STATE, MAX_STATES};

__declspec(align(32))
class IGameState
{
public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Init
	// Purpose: Initializes the state.
	// Original Author: Josh Fields
	// Creation Date: 3/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Init(void) = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown
	// Purpose: Shuts down the state.
	// Original Author: Josh Fields
	// Creation Date: 3/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Shutdown() = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update
	// Purpose: Updates the state.
	// Original Author: Josh Fields
	// Creation Date: 3/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Update(float fTime) = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Input
	// Purpose: Gets input to update the state.
	// Original Author: Josh Fields
	// Creation Date: 3/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool Input() = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Render
	// Purpose: Render the state.
	// Original Author: Josh Fields
	// Creation Date: 3/30/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Render() = 0;
};
#endif