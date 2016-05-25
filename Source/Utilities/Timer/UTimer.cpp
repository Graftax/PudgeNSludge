#include "UTimer.h"

CTimer::CTimer(void)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_llFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);

	m_bRunning = false;
}

void CTimer::Start( void )
{
	if( !m_bRunning )
	{
		m_bRunning = true;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);
	}
}

void CTimer::Reset(void)
{
	m_dElapsedTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);
}

double CTimer::GetElapsedTime(void)
{
	double dElapsed = 0;

	if (m_bRunning)
	{
		LONGLONG llCurTick;
		QueryPerformanceCounter((LARGE_INTEGER*)&llCurTick);

		dElapsed = (double)((llCurTick - m_llStartTick)) / (double)m_llFrequency;
	}

	return dElapsed;
}