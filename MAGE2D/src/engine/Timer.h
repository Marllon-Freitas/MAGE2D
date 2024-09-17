#ifndef _MAGE2D_TIMER_H_
#define _MAGE2D_TIMER_H_

#include <windows.h>                    // access to Windows high-precision counter

class Timer
{
private:
	LARGE_INTEGER m_startTime, m_endTime;   // start and end counter values
	LARGE_INTEGER m_frequency;              // counter frequency
	bool m_isStopped;                       // state of the timer

public:
	Timer();

	void  Start();                          // starts/resumes the timer
	void  Stop();                           // stops the timer
	float Reset();                          // resets the timer and returns elapsed time
	float GetElapsedTime();                 // returns elapsed time in seconds    
	bool  HasElapsed(float seconds);        // checks if "seconds" have elapsed
};

inline bool Timer::HasElapsed(float seconds)
{
	return (GetElapsedTime() >= seconds ? true : false);
}

#endif
