#include <string>
#include <ctime>
#include <chrono>
#include "FileManager.h"
#include "SortingManager.h"


int main()
{
    std::string sourceFileName = "unsorted_file.txt";
    std::string resultFileName = "sorted_file.txt";

    auto start = std::chrono::high_resolution_clock::now();

    // creating a file
    FileManager::CreateRandFile(sourceFileName);

    // time tracking
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durations = finish - start;
    std::cout << "File creation time: " << (durations.count()) / 60 << " min" << std::endl;

    std::fstream res;
    res.open(resultFileName, std::fstream::out | std::ofstream::trunc);
    res.close();

    // file sorting
    // time tracking
    start = std::chrono::high_resolution_clock::now();
    SortManager::RunSort(sourceFileName, resultFileName);

    // time tracking
    finish = std::chrono::high_resolution_clock::now();
    durations = finish - start;
    std::cout << "Sorting time: " << (durations.count()) / 60 << " min" << std::endl;

    // deletion request
    char comm;
    std::cout << "Delete temporary files? (y/n): ";
    std::cin >> comm;
    if (comm == 'y' or comm == 'Y') {
        // Delete temporary files
        FileManager::DeletedFiles();
    }

    return 0;
}