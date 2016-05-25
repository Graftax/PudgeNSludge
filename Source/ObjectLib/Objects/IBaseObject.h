/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: IBaseObject
//
// Purpose: Contain position data
//
// Original Author: Rueben Massey
//
// Creation Date: 5/8/2012
//
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef IBASEOBJECT_H_
#define IBASEOBJECT_H_

//#include <list>
#include <unordered_map>

#include "..//..//PS/config.h"
#include "..//..//Utilities/MathLib/matrix4.h"
#include "../../Utilities/KdTree/KdTreeNodes.h"
#include "../../Utilities/Frame/Frame.h"

class IComponent;

enum eOBJECT_TYPE { 
	OBJ_NULL, 
	OBJ_PLAYER, 
	OBJ_PLAYER_ARM,
	OBJ_GOO, 
	OBJ_MONSTER, 
	OBJ_HULK, 
	OBJ_SLUDGE,
	OBJ_BOSS,
	OBJ_CRUSHER,
	OBJ_CRUSHERBASE,
	OBJ_BRIDGE, 
	OBJ_DOOR,
	OBJ_DOORBUTTON,
	OBJ_BOXBUTTON,
	OBJ_OUTLET, 
	OBJ_PLUG, 
	OBJ_PIT, 
	OBJ_WALL_BREAKABLE,
	OBJ_PIPE_STEAM,
	OBJ_PIPE_INTAKE,
	OBJ_PIPE_SPAWN,
	OBJ_PIPE_BOX,
	OBJ_CRATE, 
	OBJ_CORK, 
	OBJ_LEVEL, 
	OBJ_CHKPNT, 
	OBJ_POOL, 
	OBJ_WORLD_COLLISION,
	OBJ_WALLCHUNK,
	OBJ_ANIMATED_BG,
	OBJ_LAND_MESHEFFECT,
	OBJ_CHECKPOINT,
	OBJ_GOO_PARTICLE_EFFECT,
	OBJ_CRUSHER_PARTICLE_EFFECT,
	OBJ_PLAYER_DEATH_EFFECT,
	OBJ_OBJECT_SELECTION,
	OBJ_SPAWNPIPE_DRIP,
	OBJ_MAX };

__declspec(align(32))
class IBaseObject : public CSceneObject
{
private:
	
	bool m_bAlive;
	bool m_bRendered;
	char m_cLevelID; // The ID of the Level that this object belongs to.
	short m_sReferences;
	eOBJECT_TYPE m_eType;
	Frame m_ObjectFrame;
	std::unordered_map< unsigned int, IComponent* > m_mapComponentList;

public:
	/////////////////////////////
	//  Accessors
	/////////////////////////////
	eOBJECT_TYPE GetType( void ) { return m_eType; }
	short GetRefCount( void ) { return m_sReferences; }
	Frame& GetFrame(void){ return m_ObjectFrame; }
	matrix4f* GetWorldMat(void){ return &m_ObjectFrame.GetWorldMat(); }
	matrix4f* GetLocalMat(void){ return &m_ObjectFrame.GetLocalMat(); }
	matrix4f GetPositionMatrix(void);
	vec3f& GetWorldPos(void){ return m_ObjectFrame.GetWorldMat().axis_pos; }
	bool GetAlive () { return m_bAlive; }
	bool GetRendered() const { return m_bRendered; }
	char GetLevelID(void) { return m_cLevelID; }

	/////////////////////////////
	// Mutators
	/////////////////////////////
	void SetType( eOBJECT_TYPE eType ) { m_eType = eType; }
	void SetLocalMat( matrix4f* _localMat ) { m_ObjectFrame.GetLocalMat() = *_localMat; m_ObjectFrame.Update(); }
	void SetLocalPos( vec3f* pos ){ m_ObjectFrame.GetLocalMat().axis_pos = *pos; m_ObjectFrame.Update(); }
	void SetAlive ( bool _bAlive ) { m_bAlive = _bAlive; }
	void SetRendered(bool val) { m_bRendered = val; }
	void SetLevelID( char _Id ) { m_cLevelID = _Id; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: IBaseObject
	// Purpose: Default Constructor for all Objects
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	IBaseObject();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: ~IBaseObject
	// Purpose: Default Destructor, should be defined in all objects the allocate memory
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	~IBaseObject();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: AddComponent
	// Purpose: Add a component to the component list
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddComponent( IComponent* pComponent );

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: InitComponents
	// Purpose: Initialize all components
	// Original Author: Rueben Massey
	// Creation Date: 8/4/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitComponents( void );

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: GetComponent
	// Purpose: Get a component from the component list
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* GetComponent( const unsigned int eComponentType );

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: AddRef
	// Purpose: Add a reference. YOU MUST USE THIS WHEN YOU CREATE A POINTER TO AN OBJECT THAT HAS ALREADY
	//			BEEN CREATED AND MIGHT HAVE MORE THEN ONE PERSON POINTING AT IT.
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddRef() { ++m_sReferences; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Release
	// Purpose: Removes a reference.
	//			Deletes object if there are no more references.
	//			YOU MUST USE THIS WHEN YOU STOP USING THE OBJECT POINTER
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Release();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Render
	// Purpose: Causes the object to submit to the renderer, if it can.
	// Original Author: Ethan Pendergraft	
	// Creation Date: 5/11/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render();
};

#endif// IBASEOBJECT_H_