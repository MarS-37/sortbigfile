#include "FileManager.h"


// function to create a file with random numbers
void FileManager::CreateRandFile(const std::string& filename)
{
    std::ofstream ofs(filename, std::ofstream::trunc);

    if (!ofs) {
        throw std::runtime_error("Failed to create file.");
    }

    std::cout << "File " << filename << " created\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000, 4000);

    for (int i = 0; i < LINE_IN_FILE; i++) {
        ofs << dis(gen) << std::endl;
    }

    std::cout << "File " << filename << " is full\n";
}


// function to merge two arrays into a temporary file
void FileManager::MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2)
{
    std::ofstream temp("tmp1.txt", std::ofstream::trunc);

    if (!temp) {
        throw std::runtime_error("Failed to create temporary file.");
    }

    const int* first = arr1;
    const int* second = arr2;

    if (arr1[0] > arr2[0]) {
        std::swap(first, second);
        std::swap(elements1, elements2);
    }

    int i = 0, j = 0;

    while (i < elements1 && j < elements2) {
        if (first[i] < second[j]) {
            temp << first[i++] << std::endl;
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
}


// function to merge sorted data from temporary files into the result file
void FileManager::MergeFiles(const std::string& resultfilename)
{
    std::ifstream res(resultfilename);
    std::ifstream temp1("tmp1.txt");
    std::ofstream temp2("tmp2.txt", std::ofstream::trunc);

    if (!res.is_open() || !temp1.is_open() || !temp2.is_open()) {
        throw std::runtime_error("Failed to open files for merging.");
    }

    int temp1_value, res_value;

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

    if (!std::filesystem::copy_file("tmp2.txt", resultfilename, std::filesystem::copy_options::overwrite_existing)) {
        throw std::runtime_error("Failed to copy temporary file to result file.");
    }
}


// function to read a block of data from a file into an array
int FileManager::ReadTempBlock(std::ifstream& fs, std::unique_ptr<int[]>& arr)
{
    arr = std::make_unique<int[]>(TMP_BLOCK);

    int i = 0;

    while (i < TMP_BLOCK && fs >> arr[i]) {
        ++i;
    }

    if (i == 0) {
        arr.reset();
        return 0;
    }

    if (i != TMP_BLOCK) {
        auto tmp_arr = std::make_unique<int[]>(i);
        std::copy(arr.get(), arr.get() + i, tmp_arr.get());
        arr.swap(tmp_arr);
    }

    return i;
}

// function to delete temporary files
void FileManager::DeletedFiles()
{
    std::filesystem::remove("tmp1.txt");
    std::filesystem::remove("tmp2.txt");
}