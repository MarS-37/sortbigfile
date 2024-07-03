#include "Chronometr.h"


// method fixes the start time
void Chronometr::TimeStart()
{
	start_time = std::chrono::high_resolution_clock::now();
}


// method fixes the end time
void Chronometr::TimeFinish()
{
	end_time = std::chrono::high_resolution_clock::now();
}


// method calculates the duration 
double Chronometr::TimeResult()
{
	std::chrono::duration<double> durations = end_time - start_time;

	return durations.count() / 60;
}