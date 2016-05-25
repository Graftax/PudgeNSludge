#ifndef ATH_SPECIALNEW
#define ATH_SPECIALNEW

#include <map>

class ATH_NewCounter
{

private:

	static std::map< unsigned int, unsigned int > s_Memcount;

public:

	static void AddMemory( unsigned int _type, unsigned int _amount )
	{
		if( s_Memcount.count( _type ) > 0 )
		{
			s_Memcount[ _type ] += _amount;
		}
		else
		{
			s_Memcount[ _type ] = _amount;
		}
	}
};

#endif