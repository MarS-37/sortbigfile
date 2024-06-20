#ifndef SORTALGORITHM_H
#define SORTALGORITHM_H

// template class for MergeSort algorithm
template <typename T>
class SortAlgorithm
{
public:
    // recursive MergeSort function
    static void MergeSort(T* arr, int low, int high);

private:
    // merge function to combine two sorted halves
    static void Merge(T* arr, int low, int mid, int high);
};

#include "SortAlgorithm.cpp"

#endif // SORTALGORITHM_H