#include "SortingManager.h"
#include "FileManager.h"


void SortManager::FileSorts(const std::string& filename, std::string& resultfilename)
{
	static int processed = 0;

	std::ifstream fs(filename);

	if (!fs.is_open()) {
		throw std::runtime_error("Failed to open file for reading.");
	}

	std::unique_ptr<int[]> part1;
	std::unique_ptr<int[]> part2;

	while (true) {

		int size1 = FileManager::ReadTempBlock(fs, part1);
		int size2 = FileManager::ReadTempBlock(fs, part2);

		if (size1 == 0 && size2 == 0) {
			break;
		}

		if (size1 > 0) {
			SortManager::MergeSort(part1.get(), 0, size1 - 1);
		}

		if (size2 > 0) {
			SortManager::MergeSort(part2.get(), 0, size2 - 1);
		}

		if (size1 > 0 && size2 > 0) {
			processed += size1 + size2;

			std::cout << " string processing = " << processed << std::endl;

			FileManager::MergeToFile(part1.get(), part2.get(), size1, size2);
			FileManager::MergeFiles(resultfilename);
		}
		else if (size1 > 0) {
			processed += size1;

			std::ofstream temp("tmp1.txt", std::ofstream::trunc);

			for (int i = 0; i < size1; ++i) {
				temp << part1[i] << std::endl;
			}

			FileManager::MergeFiles(resultfilename);
		}
		else if (size2 > 0) {
			processed += size2;

			std::ofstream temp("tmp1.txt", std::ofstream::trunc);

			for (int i = 0; i < size2; ++i) {
				temp << part2[i] << std::endl;
			}

			FileManager::MergeFiles(resultfilename);
		}
	}

	fs.close();
}


template <typename T>
void SortManager::MergeSort(T* arr, int low, int high)
{
    if (low < high) {
        int mid = low + (high - low) / 2;
        MergeSort(arr, low, mid);
        MergeSort(arr, mid + 1, high);
        Merge(arr, low, mid, high);
    }
}


template <typename T>
void SortManager::Merge(T* arr, int low, int mid, int high)
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