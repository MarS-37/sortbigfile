#pragma once
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>


// class is responsible for sorting blocks of 
// numbers numbers and writing them to files
class SortAlgorithm
{
public:
    static void SortBlock(const std::vector<int>& block, size_t block_index);
};