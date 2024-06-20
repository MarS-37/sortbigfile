#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <random>


// size constants
#define LINE_IN_FILE 200'000'000
#define TMP_BLOCK    5'000'000

// class for file management
class FileManager
{
public:
    // function to create a file with random numbers
    static void CreateRandFile(const std::string& filename);

    // function to merge two arrays into a temporary file
    static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2);

    // function to merge sorted data from temporary files into the result file
    static void MergeFiles(const std::string& resultfilename);

    // function to read a block of data from a file into an array
    static int ReadTempBlock(std::ifstream& fs, std::unique_ptr<int[]>& arr);

    // function to delete temporary files
    static void DeletedFiles();
};