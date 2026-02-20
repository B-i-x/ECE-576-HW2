#ifndef MEMORY_RTL_H
#define MEMORY_RTL_H

#include <systemc.h>
#include <fstream>
#include "../config/config.h"

class MEMORY_RTL : public sc_module
{
public:
    sc_in<sc_logic> Clk;
    sc_in<sc_logic> Ren, Wen;
    sc_in<int> Addr;
    sc_in<int> DataIn;
    sc_out<int> DataOut;
    sc_out<sc_logic> Ack;

    unsigned int memData[MEM_SIZE];

    SC_HAS_PROCESS(MEMORY_RTL);

    MEMORY_RTL(sc_module_name name, const char* memInitFilename)
        : sc_module(name)
    {
        SC_METHOD(rtl);
        sensitive << Clk.pos();

        // Initialize all memory to 0
        for (int i = 0; i < MEM_SIZE; i++) {
            memData[i] = 0;
        }

        // Load memory contents from file
        std::ifstream file(memInitFilename);
        if (!file.is_open())
        {
            cerr << "Error: Cannot open memory file: " << memInitFilename << endl;
            return;
        }

        int val;
        int addr = 0;
        while (file >> val && addr < MEM_SIZE)
        {
            memData[addr++] = (unsigned int)val;
        }
        file.close();
    }

    void rtl()
    {
        // Default: deassert Ack
        Ack.write(SC_LOGIC_0);

        if (Ren.read() == SC_LOGIC_1)
        {
            int address = Addr.read();
            if (address >= 0 && address < MEM_SIZE)
            {
                DataOut.write((int)memData[address]);
                Ack.write(SC_LOGIC_1);
            }
            // If address invalid, Ack remains 0
        }
        else if (Wen.read() == SC_LOGIC_1)
        {
            int address = Addr.read();
            if (address >= 0 && address < MEM_SIZE)
            {
                memData[address] = (unsigned int)DataIn.read();
                Ack.write(SC_LOGIC_1);
            }
            // If address invalid, Ack remains 0
        }
    }
};

#endif
