#ifndef DIRECTIONALLIGHT_H_
#define DIRECTIONALLIGHT_H_



#include "Light.h"

class DirectionalLight : public Light
{
public:
	D3DXVECTOR3 m_v3fDirection;
public:
	DirectionalLight(void){}
	~DirectionalLight(void){}
	virtual void ApplyLight( ID3DXEffect* _effect );
	inline void SetDirection( D3DXVECTOR3 _direction ) { m_v3fDirection = _direction; }
};

















#endif //DIRECTIONALLIGHT_H_