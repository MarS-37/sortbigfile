#pragma once
#include <mutex>
#include "FileManager.h"

// class to manage sorting operations
class SortManager
{
public:
    static void FileSorts(const std::string& filename, std::string& resultfilename);
};