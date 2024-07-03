#include "SortManager.h"
#include "FileManager.h"


int main()
{
	size_t num_lines;
	size_t block_size;

	// user input for num_lines and 
	// block_size values with validation
	try {
		std::cout << "Enter number of rows (num_lines): ";
		std::cin >> num_lines;

		if (num_lines < 1000)
			throw std::invalid_argument("The number of rows must be a positive number!");

		std::cout << "Enter the block size (block_size): ";
		std::cin >> block_size;

		if (block_size < 100)
			throw std::invalid_argument("The block size must be a positive number!");

		std::cout << std::endl;
		SortManager sort_manager;
		sort_manager.SortMerge(num_lines, block_size);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";

		return 1;
	}


    return 0;
}