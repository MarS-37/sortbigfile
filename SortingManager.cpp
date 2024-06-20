#include "SortingManager.h"
#include "SortAlgorithm.h"
#include "FileManager.h"


void SortManager::FileSorts(const std::string& filename, std::string& resultfilename)
{
	static int processed = 0;

	std::ifstream fs(filename);

	if (!fs.is_open()) {
		throw std::runtime_error("Failed to open file for reading.");
	}

	std::unique_ptr<int[]> part1;
	std::unique_ptr<int[]> part2;

	std::mutex mtx;
	std::vector<std::thread> threads;

	while (true) {

		int size1 = FileManager::ReadTempBlock(fs, part1);
		int size2 = FileManager::ReadTempBlock(fs, part2);

		if (size1 == 0 && size2 == 0) {
			break;
		}


		// launch threads for sorting
		if (size1 > 0) {
			SortAlgorithm<int>::MergeSort(part1.get(), 0, size1 - 1);
		}

		if (size2 > 0) {
			SortAlgorithm<int>::MergeSort(part2.get(), 0, size2 - 1);
		}

		// join threads
		for (auto& thread : threads) {
			if (thread.joinable()) {
				thread.join();
			}
		}

		threads.clear();


		if (size1 > 0 && size2 > 0) {
			processed += size1 + size2;

			std::cout << " string processing = " << processed << std::endl;

			FileManager::MergeToFile(part1.get(), part2.get(), size1, size2);
			FileManager::MergeFiles(resultfilename);
		}
		else if (size1 > 0) {
			processed += size1;

			std::ofstream temp("tmp1.txt", std::ofstream::trunc);

			for (int i = 0; i < size1; ++i) {
				temp << part1[i] << std::endl;
			}

			FileManager::MergeFiles(resultfilename);
		}
		else if (size2 > 0) {
			processed += size2;

			std::ofstream temp("tmp1.txt", std::ofstream::trunc);

			for (int i = 0; i < size2; ++i) {
				temp << part2[i] << std::endl;
			}

			FileManager::MergeFiles(resultfilename);
		}
	}

	fs.close();
}