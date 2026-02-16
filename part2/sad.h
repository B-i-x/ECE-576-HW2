#ifndef SAD_H
#define SAD_H

#include <systemc.h>
#include "simple_mem_if.h"
#include "config.h"

class sad : public sc_module
{
public:
    sc_port<simple_mem_if> mem_port;

    SC_HAS_PROCESS(sad);

    sad(sc_module_name name) : sc_module(name)
    {
        SC_THREAD(compute_sad);
    }

    void compute_sad()
    {
        unsigned int data1, data2;

        for (int block = 0; block < NUM_BLOCKS; block++)
        {
            int sad_val = 0;

            for (int i = 0; i < BLOCK_SIZE; i++)
            {
                mem_port->Read(INPUT1_ADDR + (block * BLOCK_SIZE) + i, data1);
                mem_port->Read(INPUT2_ADDR + (block * BLOCK_SIZE) + i, data2);

                int v = (int)data1 - (int)data2;
                if (v < 0)
                    v = -v;

                sad_val += v;
            }

            mem_port->Write(SAD_OUTPUT_ADDR + block, (unsigned int)sad_val);
            cout << "SAD[" << block << "] = " << sad_val << endl;
        }

        sc_stop();
    }
};

#endif
