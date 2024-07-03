#pragma once
#include "FileManager.h"
#include <future>


// class to manage sorting operation
class SortManager
{
public:
    void SortMerge(size_t num_lines, size_t block_size);

private:
    // for working with files
    FileManager file_manager;
};