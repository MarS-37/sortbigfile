#pragma once
#include "Chronometr.h"
#include <iostream>
#include <fstream>
#include <random>
#include <mutex>
#include <queue>


// class contains methods for working with 
// files. create, read, merge and delete
class FileManager
{
public:
    static void CreateRandFile(size_t num_lines, const std::string& filename);

    std::vector<int> ReadBlock(std::ifstream& in_file, size_t block_size);

    void DeletedFiles(size_t num_block);

    static void MergeSortedBlocks(size_t num_blocks, size_t num_lines);


private:
    static std::mutex mtx;
};