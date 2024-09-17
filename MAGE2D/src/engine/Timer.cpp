#include "Timer.h"
#include "Types.h"

Timer::Timer()
{
	// get high-resolution counter frequency
	QueryPerformanceFrequency(&m_frequency);

	// zero the start and end counter values
	ZeroMemory(&m_startTime, sizeof(m_startTime));
	ZeroMemory(&m_endTime, sizeof(m_endTime));

	// timer is running
	m_isStopped = false;
}

void Timer::Start()
{
	if (m_isStopped)
	{
		// save time before stopping
		//
		//      <--- elapsed ---> <--- stopped --->
		// ----|-----------------|----------------|----> time
		//    start              end              now 
		//

		// time elapsed before stopping
		llong elapsed = m_endTime.QuadPart - m_startTime.QuadPart;

		// resume time counting
		//
		//      <--- elapsed --->
		// ----|-----------------|-----------> time
		//    start              now
		//                         

		// account for time already elapsed before stopping
		QueryPerformanceCounter(&m_startTime);
		m_startTime.QuadPart -= elapsed;

		// resume normal counting
		m_isStopped = false;
	}
	else
	{
		// start time counting
		QueryPerformanceCounter(&m_startTime);
	}
}

void Timer::Stop()
{
	if (!m_isStopped)
	{
		// mark the stopping point
		QueryPerformanceCounter(&m_endTime);
		m_isStopped = true;
	}
}

float Timer::Reset()
{
	llong elapsed;

	if (m_isStopped)
	{
		// reset time counting
		//
		//      <--- elapsed ---> <--- stopped --->
		// ----|-----------------|----------------|------> time
		//    start              end              start
		//                       

		// get time elapsed before stopping
		elapsed = m_endTime.QuadPart - m_startTime.QuadPart;

		// restart time counting
		QueryPerformanceCounter(&m_startTime);

		// counting reactivated
		m_isStopped = false;
	}
	else
	{
		// reset time counting
		//
		//      <--- elapsed --->
		// ----|-----------------|------------> time
		//    start              end     
		//                       start

		// finalize time counting
		QueryPerformanceCounter(&m_endTime);

		// calculate elapsed time (in cycles)
		elapsed = m_endTime.QuadPart - m_startTime.QuadPart;

		// restart counter
		m_startTime = m_endTime;
	}

	// convert time to seconds
	return float(elapsed / double(m_frequency.QuadPart));
}

float Timer::GetElapsedTime()
{
	llong elapsed;

	if (m_isStopped)
	{
		// time elapsed until stopping
		//
		//      <--- elapsed ---> <--- stopped --->
		// ----|-----------------|----------------|----> time
		//    start              end              now
		//
		elapsed = m_endTime.QuadPart - m_startTime.QuadPart;
	}
	else
	{
		// time elapsed
		//
		//      <--- elapsed ---> 
		// ----|-----------------|-----------> time
		//    start              end               
		//

		// finalize time counting
		QueryPerformanceCounter(&m_endTime);

		// calculate elapsed time (in cycles)
		elapsed = m_endTime.QuadPart - m_startTime.QuadPart;
	}

	// convert time to seconds
	return float(elapsed / double(m_frequency.QuadPart));
}
