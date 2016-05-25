#ifndef ObjectManager_H
#define ObjectManager_H

#include <vector>
#include "IBaseObject.h"

class CObjectManager
{
private:
	std::vector<IBaseObject*> m_vObjects;
	static CObjectManager* m_pInstance;

	CObjectManager() {};
	~CObjectManager() {};

public:
	static CObjectManager* GetInstance ();
	static void DeleteInstance ();

	void Render ();
	void ShutDown ();

	void AddObject (IBaseObject* _iboObject);
	void RemoveObject (IBaseObject* _iboObject);

	void ClearAll ();

	std::vector<IBaseObject*>& GetAllObjects () { return m_vObjects; }
};
#endif