#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "Light.h"


class PointLight : public Light
{
private:
	D3DXMATRIX tran;
	D3DXVECTOR3 m_v3fAttenuation;
	float m_fRadius;
	CCollisionSphere	m_CollisionSphere;

public:
	PointLight(void);

	virtual void ApplyLight( ID3DXEffect* _effect );

	void SetAttenuation( const D3DXVECTOR3* _attenuation );
	void SetRadius( float _radius ) { m_fRadius = _radius; }
	inline float GetRadius(void) { return m_fRadius; }
	D3DXMATRIX GetTran(void) { return tran; }

	inline CCollisionSphere& GetCollisionSphere(void) { return m_CollisionSphere; }

};



















#endif //POINTLIGHT_H_