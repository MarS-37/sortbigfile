#ifndef SORTALGORITHM_TPP
#define SORTALGORITHM_TPP

#include "SortAlgorithm.h"
#include <algorithm>
#include <memory>


// recursive MergeSort function
template <typename T>
void SortAlgorithm<T>::MergeSort(T* arr, int low, int high)
{
    if (low < high) {
        int mid = low + (high - low) / 2;
        MergeSort(arr, low, mid);
        MergeSort(arr, mid + 1, high);
        Merge(arr, low, mid, high);
    }
}


// merge function to combine two sorted halves
template <typename T>
void SortAlgorithm<T>::Merge(T* arr, int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;

    auto left = std::make_unique<T[]>(n1);
    auto right = std::make_unique<T[]>(n2);

    std::copy(arr + low, arr + low + n1, left.get());
    std::copy(arr + mid + 1, arr + mid + 1 + n2, right.get());

    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        }
        else {
            arr[k++] = right[j++];
        }
    }

    while (i < n1) {
        arr[k++] = left[i++];
    }

    while (j < n2) {
        arr[k++] = right[j++];
    }
}

#endif // SORTALGORITHM_TPP