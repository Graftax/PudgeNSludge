#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include "Light.h"


class SpotLight : public Light
{
private:
	float m_fCutOff;
	float m_fExponent;
	float m_fRadius;
	vec3f m_v3fAttenuation;
	vec3f m_v3fDirection;
	vec3f m_v3fPosition;
	CCollisionSphere	m_CollisionSphere;

public:
	virtual void ApplyLight( ID3DXEffect* _effect );
	vec3f GetPosition(void) { return m_v3fPosition; }
	void SetCutOff(float _cutOff );
	void SetExponent( float _exponent ) { m_fExponent = _exponent; }
	void SetAttenuation( const vec3f* _attenuation, float _radius );
	void SetRadius( float _radius ) { m_fRadius = _radius; }
	void SetDirection( const vec3f _direction ) { m_v3fDirection = _direction; m_v3fDirection.normalize(); }
	void SetPosition( const vec3f _position ) { m_v3fPosition = _position; } 
	void MoveLeft(void) { m_v3fPosition.x += -0.5f; GetFrame().GetLocalMat().axis_pos.x += -0.5f; }

	inline CCollisionSphere& GetCollisionSphere(void) { return m_CollisionSphere; }

};
















#endif //SPOTLIGHT_H_