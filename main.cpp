#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
#include <filesystem>


// size constants
#define LINE_IN_FILE 400'000'000
#define TMP_BLOCK    500'000


class FileManager
{
public:
    static void CreateRandFile(const std::string& filename)
    {
        std::fstream fs;

        srand(static_cast<unsigned int>(time(nullptr)));

        const int low = -1'000;
        const int high = 4'000;

        fs.open(filename, std::fstream::out | std::ofstream::trunc);

        if (fs.is_open()) {
            std::cout << "File " << filename << " created\n";

            for (int i = 0; i < LINE_IN_FILE; i++) {
                fs << (low + rand() % high) << std::endl;
            }

            fs.close();

            std::cout << "File " << filename << " is full\n";
        }
    }

    static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2)
    {
        std::fstream temp;

        const int* first;
        const int* second;

        temp.open("tmp1.txt", std::fstream::out | std::ofstream::trunc);


        if (arr1[0] < arr2[0]) {
            first = arr1;
            second = arr2;
        }
        else {
            first = arr2;
            second = arr1;
            std::swap(elements1, elements2);
        }

        if (temp.is_open())
        {
            int i = 0;
            int j = 0;

            while (i < elements1 && j < elements2) {
                if (first[i] < second[j]) {
                    temp << first[i++] << std::endl;
                }
                else if (first[i] == second[j]) {
                    temp << first[i++] << std::endl;
                    temp << second[j++] << std::endl;
                }
                else {
                    temp << second[j++] << std::endl;
                }
            }

            while (i < elements1) {
                temp << first[i++] << std::endl;
            }

            while (j < elements2) {
                temp << second[j++] << std::endl;
            }

            temp.close();
        }
    }

    static void MergeFiles(const std::string& resultfilename)
    {
        std::fstream res;
        std::fstream temp1;
        std::fstream temp2;

        const std::string& tmp1 = "tmp1.txt";
        const std::string& tmp2 = "tmp2.txt";

        temp1.open(tmp1, std::fstream::in);
        res.open(resultfilename, std::fstream::in);
        temp2.open(tmp2, std::fstream::out | std::ofstream::trunc);

        if (!temp1.is_open() || !temp2.is_open() || !res.is_open()) {
            return;
        }

        int temp1_value;
        int res_value;

        temp1 >> temp1_value;
        res >> res_value;
        while (!temp1.eof() && !res.eof()) {
            if (temp1_value <= res_value) {
                temp2 << temp1_value << std::endl;
                temp1 >> temp1_value;
            }
            else {
                temp2 << res_value << std::endl;
                res >> res_value;
            }
        }

        while (!res.eof()) {
            temp2 << res_value << std::endl;
            res >> res_value;
        }

        while (!temp1.eof()) {
            temp2 << temp1_value << std::endl;
            temp1 >> temp1_value;
        }

        temp1.close();
        temp2.close();
        res.close();

        if (!std::filesystem::copy_file("tmp2.txt", resultfilename,
            std::filesystem::copy_options::overwrite_existing)) {
            return;
        }
    }

    static int ReadTempBlock(std::fstream& fs, std::unique_ptr<int[]>& arr)
    {
        arr.reset(new int[TMP_BLOCK]);
        int* tmp_arr;
        int i;
        for (i = 0; i < TMP_BLOCK && !fs.eof(); i++)
            fs >> arr[i];

        if (i == 1) {
            arr.reset();
            return 0;
        }

        if (i != TMP_BLOCK) {
            tmp_arr = new int[i];
            for (size_t j = 0; j < i; j++)
                tmp_arr[j] = arr[j];

            arr.reset(tmp_arr);
            return i - 1;
        }

        return TMP_BLOCK;
    }

    static void DeletedFiles()
    {
        std::filesystem::remove("tmp1.txt");
        std::filesystem::remove("tmp2.txt");
    }
};


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


class SortingManager
{
public:
    static void RunSort(const std::string& filename, std::string& resultfilename)
    {
        static int processed = 0;

        std::fstream fs;
        fs.open(filename, std::fstream::in);

        if (!fs.is_open()) {
            throw std::runtime_error("Failed to open file for reading.");
        }

        while (!fs.eof()) {
            std::unique_ptr<int[]> part1;
            std::unique_ptr<int[]> part2;

            int size1 = FileManager::ReadTempBlock(fs, part1);
            int size2 = FileManager::ReadTempBlock(fs, part2);

            if (size1 == 0 || size2 == 0) {
                return;
            }

            processed += size1 + size2;
            std::cout << " string processing = " << processed << std::endl;

            Sorting::MergeSort(part1.get(), 0, size1 - 1);
            Sorting::MergeSort(part2.get(), 0, size2 - 1);

            FileManager::MergeToFile(part1.get(), part2.get(), size1, size2);
            FileManager::MergeFiles(resultfilename);
        }

        fs.close();
    }
};


int main()
{
    std::string sourceFileName = "unsorted_file.txt";
    std::string resultFileName = "sorted_file.txt";

    auto start = std::chrono::high_resolution_clock::now();

    // creating a file
    FileManager::CreateRandFile(sourceFileName);

    // time tracking
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durations = finish - start;
    std::cout << "File creation time: " << (durations.count()) / 60 << " min" << std::endl;

    std::fstream res;
    res.open(resultFileName, std::fstream::out | std::ofstream::trunc);
    res.close();

    // file sorting
    // time tracking
    start = std::chrono::high_resolution_clock::now();
    SortingManager::RunSort(sourceFileName, resultFileName);

    // time tracking
    finish = std::chrono::high_resolution_clock::now();
    durations = finish - start;
    std::cout << "Sorting time: " << (durations.count()) / 60 << " min" << std::endl;

    // deletion request
    char comm;
    std::cout << "Delete temporary files? (y/n): ";
    std::cin >> comm;
    if (comm == 'y' or comm == 'Y') {
        // Delete temporary files
        FileManager::DeletedFiles();
    }

    return 0;
}