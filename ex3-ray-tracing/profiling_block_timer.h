//
//  profiling_block_timer.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//

#ifndef cg_projects_profiling_block_timer_h
#define cg_projects_profiling_block_timer_h

#include <stdio.h>

#if __APPLE__
#include <mach/mach_time.h>
#endif

class profiling_block_timer
{
	
public:
	
	profiling_block_timer (const char *title)
	: _title(title), _startTime(profiling_block_timer::TickTimeInSeconds())
	{
		
	}
	
	~profiling_block_timer ()
	{
		double totalTime = profiling_block_timer::TickTimeInSeconds() - _startTime;
		
		printf("%s: %0.3f sec\n", _title, totalTime);
	}
	
private:
	
	profiling_block_timer (const profiling_block_timer &timer);
	
	profiling_block_timer & operator= (const profiling_block_timer &timer);
	
private:
	
	const char *_title;
	
	double _startTime;
	
	static double TickTimeInSeconds()
	{
		
#if __APPLE__
		static double freqMultiplier = 0.0;
		if (freqMultiplier == 0.0)
		{
			mach_timebase_info_data_t freq;
			mach_timebase_info(&freq);
			
			freqMultiplier = ((double)freq.numer / (double)freq.denom) * 1.0e-9;
		}
		return mach_absolute_time() * freqMultiplier;
#else
//		//this is a fast timer to nanos
//		struct timespec now;
//		clock_gettime(CLOCK_MONOTONIC, &now);
//		return now.tv_sec + (real64)now.tv_nsec * 1.0e-9;
//
//#else

	// Perhaps a better call exists. (e.g. avoid adjtime effects)

	struct timeval tv;

	gettimeofday (&tv, NULL);

	return tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
	
#endif
	}
};

#endif
