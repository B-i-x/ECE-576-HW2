#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "../../part1/config.h"

int main()
{
    // Build filename: memfile_Default.txt, memfile_Small.txt, etc.
    std::string filename = "memfile_" + std::string(CONFIG_NAME) + ".txt";

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot create " << filename << std::endl;
        return 1;
    }

    int total_size = INPUT2_ADDR + (NUM_BLOCKS * BLOCK_SIZE);

    // Seed for deterministic pseudo-random values
    srand(42);

    for (int addr = 0; addr < total_size; addr++)
    {
        if (addr >= INPUT1_ADDR && addr < INPUT1_ADDR + (NUM_BLOCKS * BLOCK_SIZE))
        {
            // Input1 region: values 0-255 (pixel-like)
            file << (rand() % 256);
        }
        else if (addr >= INPUT2_ADDR && addr < INPUT2_ADDR + (NUM_BLOCKS * BLOCK_SIZE))
        {
            // Input2 region: values 0-255 (pixel-like)
            file << (rand() % 256);
        }
        else
        {
            // Gap between input regions
            file << 0;
        }

        if (addr < total_size - 1)
            file << " ";
    }

    file << std::endl;
    file.close();

    std::cout << "Generated " << filename << " (" << total_size << " values)"
              << " using config: " << CONFIG_NAME << std::endl;

    return 0;
}
