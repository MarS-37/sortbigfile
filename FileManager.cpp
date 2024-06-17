#include "FileManager.h"


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


void FileManager::MergeFiles(const std::string& resultfilename)
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


int FileManager::ReadTempBlock(std::fstream& fs, std::unique_ptr<int[]>& arr)
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

void FileManager::DeletedFiles()
{
    std::filesystem::remove("tmp1.txt");
    std::filesystem::remove("tmp2.txt");
}