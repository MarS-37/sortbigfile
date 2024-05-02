#pragma once
#include "FileManager.h"

class SortManager
{
public:
    static void RunSort(const std::string& filename, std::string& resultfilename);

    template <typename T>
    static void MergeSort(T* arr, int low, int high);

private:
    template <typename T>
    static void Merge(T* arr, int low, int mid, int high);
};