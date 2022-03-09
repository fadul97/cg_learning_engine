/**********************************************************************************
// Timer (Source File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Use a high precision counter to measure time.
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "Timer.h"

// ---------------------------------------------------------------------------------
// Initialization of static class members
LARGE_INTEGER LearningEngine::Timer::freq = { };

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Timer::Timer()
{
	// Gets frequency from high precision counter
	if (!freq.QuadPart)
		QueryPerformanceFrequency(&freq);

	// Zero for start and end counter values
	ZeroMemory(&start, sizeof(start));
	ZeroMemory(&end, sizeof(end));

	// Counter is running
	stopped = false;
}

// -------------------------------------------------------------------------------
// Starts time counter
void LearningEngine::Timer::Start()
{
	if (stopped)
	{
		// Resume time count
		//
		//      <--- elapsed ---->
		// ----|------------------|------------> time
		//    start               end     

		// Stores elapsed time before pausing
		long long elapsed = end.QuadPart - start.QuadPart;

		// Take into account time already elapsed before pausing
		QueryPerformanceCounter(&start);
		start.QuadPart -= elapsed;

		// Resumes counter
		stopped = false;
	}
	else
	{
		// Starts time counter
		QueryPerformanceCounter(&start);
	}
}

// -------------------------------------------------------------------------------
// Stops time counter
void LearningEngine::Timer::Stop()
{
	if (!stopped)
	{
		// Marks the stopping point in time
		QueryPerformanceCounter(&end);
		stopped = true;
	}
}

// -------------------------------------------------------------------------------
// Resets time counter and returns elapsed time
double LearningEngine::Timer::Reset()
{
	long long elapsed;

	if (stopped)
	{
		// Get elapsed time before pausing
		elapsed = end.QuadPart - start.QuadPart;

		// Resets time counter
		QueryPerformanceCounter(&start);

		// Resumes counter
		stopped = false;
	}
	else
	{
		// End time counter
		QueryPerformanceCounter(&end);

		// Calculate elapsed time (in cicles)
		elapsed = end.QuadPart - start.QuadPart;

		// Reset counter
		start = end;
	}

	// Transform time into seconds
	return (elapsed / double(freq.QuadPart));
}

// -------------------------------------------------------------------------------
// Returns current counter value
long long LearningEngine::Timer::GetStamp()
{
	QueryPerformanceCounter(&end);
	return end.QuadPart;
}

// -------------------------------------------------------------------------------
// Returns elapsed time in seconds
double LearningEngine::Timer::GetElapsedTime()
{
	long long elapsed;

	if (stopped)
	{
		// Get elapsed time until pause
		elapsed = end.QuadPart - start.QuadPart;
	}
	else
	{
		// End time counter
		QueryPerformanceCounter(&end);

		// Calculate elapsed time (in cicles)
		elapsed = end.QuadPart - start.QuadPart;
	}

	// Transform time into seconds
	return (elapsed / double(freq.QuadPart));
}

// -------------------------------------------------------------------------------
// Returns elapsed time since beginning
double LearningEngine::Timer::GetTimeElapsedFromStart(long long stamp)
{
	long long elapsed;

	if (stopped)
	{
		// Get elapsed time until pause
		elapsed = end.QuadPart - stamp;
	}
	else
	{
		// End time counter
		QueryPerformanceCounter(&end);

		// Calculate elapsed time (in cicles)
		elapsed = end.QuadPart - stamp;
	}

	// Transform time into seconds
	return (elapsed / double(freq.QuadPart));
}