#include "FileManager.h"


// static method initialization
std::mutex FileManager::mtx;


// method creates a file with numbers
void FileManager::CreateRandFile(size_t num_lines, const std::string& filename)
{
    // calculate time
    Chronometr chronometr;
    chronometr.TimeStart();

    std::ofstream out_file(filename);
    if (!out_file)
        throw std::runtime_error("Failed to open a file to record random numbers!");

    // random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000, 4000);

    for (size_t i = 0; i < num_lines; ++i) {
        out_file << dis(gen) << "\n";

        // progress
        if (i % (num_lines / 100) == 0) {
            std::lock_guard<std::mutex> guard(mtx);
            std::cout << "\rFile \"" << filename << "\" created: " << (i * 100 / num_lines) + 1 << " % " << std::flush;
         }
    }

    out_file.close();
    std::cout << "\rFile \"" << filename << "\" created: 100%\n";

    // calculate time
    chronometr.TimeFinish();

    std::lock_guard<std::mutex> guard(mtx);
    std::cout << "File creation time: " << chronometr.TimeResult() << " min\n\n";
}


// method of reading blocks from a file
std::vector<int> FileManager::ReadBlock(std::ifstream& in_file, size_t block_size)
{
    std::vector<int> block;
    std::string line;

    for (size_t i = 0; i < block_size && std::getline(in_file, line); ++i) {
        try {
            block.push_back(std::stoi(line));
        }
        catch (const std::invalid_argument& e) {
            throw std::runtime_error("Error converting a string to number!");
        }
    }

    return block;
}


// method for deleting temporary block files
void FileManager::DeletedFiles(size_t num_blocks)
{
    char response;
    std::cout << "\nDelete temporary files& (y/n): ";
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
        for (size_t i = 0; i < num_blocks; ++i) {
            std::string filename = "tmp_" + std::to_string(i) + ".txt";
            if (std::remove(filename.c_str()) != 0) {
                std::cerr << "Error when deleting a file: " << filename << "\n";
            }
            else {
                std::cout << " File " << filename << " deleted.\n";
            }
        }
    }
}


// static method for merging sorted blocks
void FileManager::MergeSortedBlocks(size_t num_blocks, size_t num_lines)
{
    // calculate time
    Chronometr chronometr;
    chronometr.TimeStart();

    // structure for comparing tuples
    // in the priority queue
    struct Compare
    {
        bool operator()(const std::tuple<int, size_t, std::ifstream*>& a, const std::tuple<int, size_t, std::ifstream*>& b)
        {
            // compare the first elements of tuples
            return std::get<0>(a) > std::get<0>(b);
        }
    };

    // queue for block merge
    std::priority_queue<std::tuple<int, size_t, std::ifstream*>,
        std::vector<std::tuple<int, size_t, std::ifstream*>>,
        Compare> min_heap;

    // vector of pointer to block files
    std::vector<std::ifstream*> block_files;
    // open block files and add the first 
    // number from each block to the queue
    for (size_t i = 0; i < num_blocks; ++i) {
        std::ifstream* in_file = new std::ifstream("tmp_" + std::to_string(i) + ".txt");

        if (!in_file || !*in_file)
            throw std::runtime_error("Failed to open a file to record sorted numbers!");

        int num;

        if (*in_file >> num)
            min_heap.emplace(num, i, in_file);

        block_files.push_back(in_file);
    }

    // open a file to record sorted numbers
    std::ofstream out_file("sorted.txt");

    if (!out_file)
        throw std::runtime_error("Failed to open a file to record sorted numbers!");

    size_t count = 0;

    // merge sorted blocks
    while (!min_heap.empty()) {
        // extract the minimum number from the queue
        auto [num, block_index, in_file] = min_heap.top();
        min_heap.pop();

        // write number to file
        out_file << num << "\n";

        int next_num;
        if (*in_file >> next_num)
            min_heap.emplace(next_num, block_index, in_file);

        if (++count % (num_lines / 100) == 0) {
            std::lock_guard<std::mutex> guard(mtx);
            std::cout << "\rSorting \"sorted_file.txt\" created: " << (count * 100 / num_lines) + 1 << "%" << std::flush;
        }
    }

    std::cout << "\rSorting \"sorted_file.txt\" created: 100%\n";

    // calculate time
    chronometr.TimeFinish();
    
    std::lock_guard<std::mutex> guard(mtx);
    std::cout << "File sorting time: " << chronometr.TimeResult() << " min\n";

    // close block files and free memory
    for (auto infile : block_files) {
        infile->close();
        delete infile;
    }
}