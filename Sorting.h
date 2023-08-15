#pragma once

class Sorting
{
public:
    template <typename T>
    static void MergeSort(T* arr, int low, int high)
    {
        if (low < high) {
            int mid = low + (high - low) / 2;
            MergeSort(arr, low, mid);
            MergeSort(arr, mid + 1, high);
            Merge(arr, low, mid, high);
        }
    }

private:
    template <typename T>
    static void Merge(T* arr, int low, int mid, int high)
    {
        int n1 = mid - low + 1;
        int n2 = high - mid;

        T* left = new T[n1];
        T* right = new T[n2];

        for (int i = 0; i < n1; i++)
            left[i] = arr[low + i];
        for (int j = 0; j < n2; j++)
            right[j] = arr[mid + 1 + j];

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

        delete[] left;
        delete[] right;
    }
};

