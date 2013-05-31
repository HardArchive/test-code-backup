#include "ProfileTime.h"

CProfileTime::CProfileTime(void)
{
//��windows�£���õδ��Ƶ��
#ifndef __LINUX__
	::QueryPerformanceFrequency(&m_liPerfFreq);	
#endif
}

CProfileTime::~CProfileTime(void)
{
}

bool CProfileTime::Start()
{
#ifndef __LINUX__
	QueryPerformanceCounter(&m_liPerfStart);
#else
	m_HighResTimer.start();
#endif

	return true;
}

uint64 CProfileTime::Stop()
{
#ifndef __LINUX__
	QueryPerformanceCounter(&m_liPerfStop);

	return (((m_liPerfStop.QuadPart - m_liPerfStart.QuadPart) * 1000 * 1000 * 1000)/m_liPerfFreq.QuadPart);
#else
	m_HighResTimer.stop();

	uint64 u8TimeCost = 0;
	m_HighResTimer.elapsed_time(u8TimeCost);
	return u8TimeCost;
#endif
}


