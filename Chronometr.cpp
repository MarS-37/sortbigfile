#include "Chronometr.h"

// method locks the start
void Chronometr::TimeStart(std::string message = "")
{
	started = true;

	// get the current time
	start_time = std::chrono::steady_clock::now();

	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// structure for storing locale time
	std::tm start_tm;

	// get the locale time
	localtime_s(&start_tm, &now);

	// get milliseconds
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		start_time.time_since_epoch()
	);

	// output the start time in the format HH:MM:SS.mmm
	if (message != "") {
		std::cout << message << " ";

		// format and display hours, minutes and seconds
		std::cout << std::put_time(&start_tm, "%H:%M:%S");

		// output milliseconds
		std::cout << "." << std::setw(3) << std::setfill('0') << ms.count() % 1000 << " hour\n\n";
	}
}


// method to end the time countdown
void Chronometr::TimeFinish(std::string message = "")
{
	// if timing is started
	if (started) {
		// get the current time
		auto finish_time = std::chrono::steady_clock::now();
		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		// structure for storing locale time
		std::tm finish_tm;

		// get the locale time
		localtime_s(&finish_tm, &now);

		// get milliseconds
		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			finish_time.time_since_epoch()
		);

		// output the finish time in the format HH:MM:SS.mmm
		if (message != "") {
			std::cout << message << " ";

			// format and display hours, minutesand seconds
			std::cout << std::put_time(&finish_tm, "%H:%M:%S");
			std::cout << "." << std::setw(3) << std::setfill('0') << ms.count() % 1000 << " hour\n";
		}
	}
	else {
		// if the time countdown has not started, 
		// display an error message
		std::cout << "Timer has not started.\n";
	}
}


// method for outputting the result of time countdown
void Chronometr::TimeResult(std::string message = "")
{
	// if timing is started
	if (started) {
		// get current time
		auto finish_time = std::chrono::steady_clock::now();

		// calculate the time duration
		auto duration = finish_time - start_time;

		// convert duration to milliseconds
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		// hours
		auto hours = std::chrono::duration_cast<std::chrono::hours>(ms);
		ms -= hours;

		// minutes
		auto minutes = std::chrono::duration_cast<std::chrono::minutes>(ms);
		ms -= minutes;

		// seconds
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ms);
		ms -= seconds;

		// output the result time in the format HH:MM:SS.mmm
		if (message != "")
			std::cout << message << " ";

		// output of numbers with leading zeros
		std::cout << std::setfill('0');
		std::cout << std::setw(2) << hours.count() << ":";
		std::cout << std::setw(2) << minutes.count() << ":";
		std::cout << std::setw(2) << seconds.count() << ".";
		std::cout << std::setw(3) << ms.count() << " hour\n\n";
	}
	else {
		std::cout << "Timer has not started.\n";
	}
}