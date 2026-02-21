#include <iostream>
#include <fstream>
#include <string>
#include "config.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: memfile_solution_generator <memfile>" << std::endl;
        return 1;
    }

    // Load memory from file
    unsigned int mem[MEM_SIZE];
    for (int i = 0; i < MEM_SIZE; i++)
        mem[i] = 0;

    std::ifstream infile(argv[1]);
    if (!infile.is_open())
    {
        std::cerr << "Error: Cannot open " << argv[1] << std::endl;
        return 1;
    }

    int val;
    int addr = 0;
    while (infile >> val && addr < MEM_SIZE)
    {
        mem[addr++] = (unsigned int)val;
    }
    infile.close();

    // Open solution output file
    std::string filename = "memfile_" + std::string(CONFIG_NAME) + "_solution.txt";
    std::ofstream outfile(filename);
    if (!outfile.is_open())
    {
        std::cerr << "Error: Cannot create " << filename << std::endl;
        return 1;
    }

    // Compute SAD for each block (same algorithm as part1/sad.h)
    for (int block = 0; block < NUM_BLOCKS; block++)
    {
        int sad_val = 0;

        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            int v = (int)mem[INPUT1_ADDR + (block * BLOCK_SIZE) + i]
                  - (int)mem[INPUT2_ADDR + (block * BLOCK_SIZE) + i];
            if (v < 0)
                v = -v;
            sad_val += v;
        }

        std::string line = "SAD[" + std::to_string(block) + "] = " + std::to_string(sad_val);
        std::cout << line << std::endl;
        outfile << line << std::endl;
    }

    outfile.close();
    std::cout << "Solution written to " << filename << std::endl;

    return 0;
}
