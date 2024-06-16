#include "SortingManager.h"
#include "FileManager.h"
#include "Chronometr.h"
#include <string>
#include <chrono>
#include <ctime>


int main()
{
    std::string sourceFileName = "unsorted_file.txt";
    std::string resultFileName = "sorted_file.txt";


    // time tracking
    Chronometr timer;
    timer.TimeStart("Start program:");


    // creating a file
    FileManager::CreateRandFile(sourceFileName);


    // time tracking
    timer.TimeResult("File creation time:");


    std::fstream res;
    res.open(resultFileName, std::fstream::out | std::ofstream::trunc);
    res.close();

 
    // file sorting
    SortManager::RunSort(sourceFileName, resultFileName);


    // time tracking
    timer.TimeResult("Sorting file time:");
    timer.TimeFinish("Finish program:");


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