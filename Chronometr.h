#pragma once
#include <chrono>


// class measures the execution time of operations
class Chronometr
{
public:
	void TimeStart();

	void TimeFinish();

	double TimeResult();

private:
	std::chrono::high_resolution_clock::time_point start_time;
	std::chrono::high_resolution_clock::time_point end_time;
};