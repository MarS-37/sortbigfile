#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

// size constants
#define LINE_IN_FILE 2'000'000
#define TMP_BLOCK    500'000

class FileManager
{
public:
    static void CreateRandFile(const std::string& filename);

    static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2);

    static void MergeFiles(const std::string& resultfilename);

    static int ReadTempBlock(std::fstream& fs, std::unique_ptr<int[]>& arr);

    static void DeletedFiles();
};

