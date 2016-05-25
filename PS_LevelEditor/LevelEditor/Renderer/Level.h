#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "Frame.h"
#include "IBaseObject.h"

class Level
{
private:
	std::string m_szName;
	Frame m_Frame;

	std::vector <IBaseObject*> m_vHazards;

public:
	Level ();
	~Level ();

	std::string GetLevelName () { return m_szName; }
	void SetLevelName ( std::string _szName ) { m_szName = _szName; }

	Frame& GetFrame () { return m_Frame; }
	void SetFrame ( Frame _Frame ) { m_Frame = _Frame; }

	std::vector<IBaseObject*>& GetHazards () { return m_vHazards; }
	void SetHazards ( std::vector<IBaseObject*> _vHazards ) { m_vHazards = _vHazards; }

};
#endif