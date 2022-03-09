/**********************************************************************************
// Timer (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Use a high precision counter to measure time.
//
**********************************************************************************/

#ifndef LE_TIMER_H
#define LE_TIMER_H

// ---------------------------------------------------------------------------------
// Includes

#include <windows.h>											// Access to the Windows high precision counter

namespace LearningEngine
{
	class Timer
	{
	public:
		Timer();												// Constructor

		void Start();											// Starts time counter
		void Stop();											// Stops time counter
		double Reset();											// Resets time counter and returns elapsed time
		double GetElapsedTime();								// Returns elapsed time in seconds
		bool WasElapsed(double secs);							// Check if time (in seconds) was elapsed 

		long long GetStamp();									// Returns current counter value
		double GetTimeElapsedFromStart(long long stamp);		// Returns elapsed time since beginning
		bool WasElapsed(long long stamp, double secs);			// Check if time was elapsed since beginning

	private:
		static LARGE_INTEGER freq;								// Counter frequency
		LARGE_INTEGER start;									// Start value of counter
		LARGE_INTEGER end;										// End value of counter
		bool stopped;											// Counter state
	};

	// ---------------------------------------------------------------------------------
	// Inline Functions - Getters

	inline bool Timer::WasElapsed(double secs)
	{ return (GetElapsedTime() >= secs ? true : false); }

	inline bool Timer::WasElapsed(long long stamp, double secs)
	{ return (GetTimeElapsedFromStart(stamp) >= secs ? true : false); }
}

#endif // !LE_TIMER_H
