#include "SortAlgorithm.h"


void SortAlgorithm::SortBlock(const std::vector<int>& block, size_t block_index)
{
	std::vector<int> sorted_block = block;

	std::sort(sorted_block.begin(), sorted_block.end());

	std::ofstream out_file("tmp_" + std::to_string(block_index) + ".txt");

	if (!out_file)
		throw std::runtime_error("Failed to open a file to write a sorted block!");

	for (const int& num : sorted_block)
		out_file << num << "\n";

	out_file.close();
}