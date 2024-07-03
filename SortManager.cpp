#include "SortManager.h"
#include "SortAlgorithm.h"
#include "FileManager.h"


// method for performing block sorting and merging
void SortManager::SortMerge(size_t num_lines, size_t block_size)
{
	if (block_size >= num_lines)
		throw std::invalid_argument("The block size cannot be greater than or equal to the number of lines!");
	
	const std::string filename = "unsorting_file.txt";

	// rsndom number generation
	file_manager.CreateRandFile(num_lines, filename);

	// calculate the number of blocks
	size_t num_blocks = (num_lines + block_size - 1) / block_size;

	// open the file for reading
	std::ifstream in_file(filename);
	if (!in_file)
		throw std::runtime_error("Failed to open a file to read random numbers!");

	// maximum number of threads
	const size_t max_threads = std::thread::hardware_concurrency();
	std::vector<std::future<void>> futures;

	// reading and sorting blocks in separate threads
	for (size_t i = 0; i < num_blocks; ++i) {
		std::vector<int> block = file_manager.ReadBlock(in_file, block_size);

		if (futures.size() >= max_threads) {
			// wait for one of the futures to complete
			for (auto& fut : futures) {
				if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
					fut.get();
					futures.erase(std::remove_if(futures.begin(), futures.end(),
						[](std::future<void>& f) 
						{ return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }),
						futures.end());
					break;
				}
			}
		}

		futures.push_back(std::async(std::launch::async, SortAlgorithm::SortBlock, block, i));
	}

	// wait for all futures to complete
	for (auto& fut : futures)
		fut.wait();
	
	// merge sorted blocks and delete temporary files
	FileManager::MergeSortedBlocks(num_blocks, num_lines);
	file_manager.DeletedFiles(num_blocks);
}