#include "SortingManager.h"
#include "FileManager.h"
#include "Sorting.h"

void SortingManager::RunSort(const std::string& filename, std::string& resultfilename)
{
    static int processed = 0;

    std::fstream fs;
    fs.open(filename, std::fstream::in);

    if (!fs.is_open()) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    while (!fs.eof()) {
        std::unique_ptr<int[]> part1;
        std::unique_ptr<int[]> part2;

        int size1 = FileManager::ReadTempBlock(fs, part1);
        int size2 = FileManager::ReadTempBlock(fs, part2);

        if (size1 == 0 || size2 == 0) {
            return;
        }

        processed += size1 + size2;
        std::cout << " string processing = " << processed << std::endl;

        Sorting::MergeSort(part1.get(), 0, size1 - 1);
        Sorting::MergeSort(part2.get(), 0, size2 - 1);

        FileManager::MergeToFile(part1.get(), part2.get(), size1, size2);
        FileManager::MergeFiles(resultfilename);
    }

    fs.close();
}