#ifndef MEMORY_H
#define MEMORY_H

#include <systemc.h>
#include <fstream>
#include "simple_mem_if.h"
#include "config.h"

class memory : public sc_module, public simple_mem_if
{
public:
    sc_export<simple_mem_if> target;

    unsigned int mem[MEM_SIZE];

    SC_HAS_PROCESS(memory);

    memory(sc_module_name name, const char* filename) : sc_module(name)
    {
        target.bind(*this);

        // Initialize all memory to 0
        for (int i = 0; i < MEM_SIZE; i++)
            mem[i] = 0;

        // Load memory contents from file
        std::ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error: Cannot open memory file: " << filename << endl;
            SC_REPORT_ERROR("MEMORY", "Cannot open memory file");
            return;
        }

        int val;
        int addr = 0;
        while (file >> val && addr < MEM_SIZE)
        {
            mem[addr++] = (unsigned int)val;
        }
        file.close();
    }

    bool Write(unsigned int addr, unsigned int data)
    {
        if (addr >= MEM_SIZE)
            return false;
        mem[addr] = data;
        return true;
    }

    bool Read(unsigned int addr, unsigned int& data)
    {
        if (addr >= MEM_SIZE)
            return false;
        data = mem[addr];
        return true;
    }
};

#endif
