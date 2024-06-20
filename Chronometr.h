#pragma once
#include <iostream>
#include <chrono>

// class for measuring time intervals
class Chronometr
{
public:
	Chronometr() : started(false) {};

	// method locks the start
	void TimeStart(std::string message);

	// method to end the time countdown
	void TimeFinish(std::string message);

	// method for outputting the result of time countdown
	void TimeResult(std::string message);

private:
	bool started;
	std::chrono::time_point<std::chrono::steady_clock> start_time;
};