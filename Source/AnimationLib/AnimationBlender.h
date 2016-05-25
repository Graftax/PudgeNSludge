#ifndef ANIMATIONBLENDER_H_
#define ANIMATIONBLENDER_H_

#include "AnimationSystem.h"

__declspec(align(32))
class AnimationBlender
{
	float m_fTransitionTime;
	float m_fCurrentTime;

	Interpolator* m_pInterpolatorOne;
	Interpolator* m_pInterpolatorTwo;

	vector<D3DXMATRIX> m_vBlendedFrames;
	bool m_bIsDoneInterpolating;
public:
	AnimationBlender(void);
	~AnimationBlender(void);

	void SetTransitionTime(float fTime){ m_fTransitionTime = fTime; }
	void SetCurrentTime(float fTime) { m_fCurrentTime = fTime; }
	void AddCurrentTime(float fTime) { m_fCurrentTime += fTime;}
	void SetIsDoneInterp(bool value){ m_bIsDoneInterpolating = value; }
	const vector<D3DXMATRIX>& GetFrames() { return m_vBlendedFrames;}
	float GetDissolveFactor() { return (m_fCurrentTime/m_fTransitionTime);}
	bool isDoneInterp() { return m_bIsDoneInterpolating; }
	bool SetInterpolators( Interpolator* pInterp1, Interpolator* pInterp2);
	bool Dissolve();
};

#endif