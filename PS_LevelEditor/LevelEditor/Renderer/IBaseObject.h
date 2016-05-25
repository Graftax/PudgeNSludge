#ifndef IBaseObject_H
#define IBaseObject_H

#include <d3d9.h>
#include <d3dx9.h>
#include "PS RenderEngine\RenderEngine.h"

// enum HAZARDTYPE { HZ_STEAM_PIPE, HZ_SPAWN_PIPE, HZ_BRIDGE, HZ_BREAKABLE_WALL, HZ_CRUSHER, HZ_GOO_PIT, HZ_STEAM_INTAKE, HZ_OUTLETS, HZ_MAX };

enum HazardType {HZ_STEAM_PIPE, HZ_SPAWN_PIPE, HZ_BRIDGE, HZ_BREAKABLE_WALL, HZ_OUTLET, HZ_CRUSHER, HZ_GOO_PIT, HZ_STEAM_INTAKE, HZ_WATER_POOL, HZ_CRUSHER_BASE, HZ_DOOR_BUTTON, HZ_BOX_BUTTON, HZ_BOX_SPAWN, HZ_DOOR, HZ_CHECK_POINT, HZ_MAX};
enum ObjectType {OT_CRATE, OT_CORK, OT_PLUG, OT_PUDGE, OT_SLIMEMONSTER, OT_MAX};

class IBaseObject
{
private:
	D3DXMATRIX m_WorldMatrix;
	RenderNode* m_pRenderNode;

	int m_HazardType;
	int m_ObjectType;

public:
	IBaseObject ();
	~IBaseObject ();

	void Render ();

	void SetWorldMatrix ( D3DXMATRIX matrix ) { m_WorldMatrix = matrix; }
	D3DXMATRIX& GetWorldMatrix () { return m_WorldMatrix; }

	void SetRenderNode ( RenderNode* pNode ) { m_pRenderNode = pNode; }
	RenderNode* GetRenderNode () { return m_pRenderNode; }

	void SetObjectType ( int type ) { m_ObjectType = type; }
	int GetObjectType () { return m_ObjectType; }

	void SetHazardType ( int type ) { m_HazardType = type; }
	int GetHazardType () { return m_HazardType; }
};
#endif