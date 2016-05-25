#ifndef _LEVELMANAGER_H
#define _LEVELMANAGER_H


#include "CLevel.h"

#define MAX_ACTIVE_LEVELS 3

class LevelManager
{
private:
	static LevelManager* m_pInstance;
	
	LevelManager ();
	~LevelManager ();

	LevelManager(LevelManager const&) {}			
	LevelManager& operator=(LevelManager const&) {}
	
	std::vector <CLevel> m_vLevels;						// All the loaded levels.
	CLevel*	m_pActiveLevels[MAX_ACTIVE_LEVELS];			// An array of pointers to the active levels.

	int m_nCurLevelIndex;
	int m_nCurLevelToLoadIndex;							// The current level index to load.
	
	bool m_bLoadNext;
	bool m_bUnloadFirstLevel;

public:

	static LevelManager* GetInstance ();

	static void DeleteInstance ();

	void Init (char* _szLevelDataFileName);
	void Shutdown ();
	
	void AddLevel (CLevel _lvl) { m_vLevels.push_back (_lvl); }
	void LoadLevel ();
	void LoadNextLevel ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: UnloadFirstActiveLevel
	// Purpose: Clears the objects belonging to the first active level, and removes it from the active level array.
	// Original Author: Andy Madruga
	// Creation Date: 8/2/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void UnloadFirstActiveLevel(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: LoadAllLevelsToRender
	// Purpose: This will Load up all the render nodes for all 5 levels so everything is rendered at once.
	// Original Author: Andy Madruga
	// Creation Date: 8/2/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadAllLevelsToRender(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update
	// Purpose: This will update the level manager which will load the next level and unload levels. As
	//			well as updating the light managers in each level.
	// Original Author: Andy Madruga
	// Creation Date: 8/7/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float _fDT );
	
	void SetLoadNextLevel (bool _bLoad) { m_bLoadNext = _bLoad; }
	void SetCurrentLevel (int _nIndex) { m_nCurLevelIndex = _nIndex; }

	CLevel* GetCurrentLevel(void) { return &m_vLevels[m_nCurLevelIndex]; }
	
	void ResetCurrentLevel(void);

	int GetCurrentLevelIndex () { return m_nCurLevelIndex; }
	int GetNumLevels () { return m_vLevels.size(); }

	CLevel** GetActiveLevels(void) { return &m_pActiveLevels[0]; }
	CLevel* GetLevelByIndex( int _index ) { return &m_vLevels[_index]; }

	void SetUnloadFirstLevel( bool _val ) { m_bUnloadFirstLevel = _val; }
	bool GetUnloadFirstLevel( void ) { return m_bUnloadFirstLevel; }
};

#endif