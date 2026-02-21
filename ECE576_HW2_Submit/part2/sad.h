#ifndef SAD_H
#define SAD_H

#include <systemc.h>
#include "simple_mem_if.h"
#include "config.h"

/*
For approximate timed model adding 10ns delay to all operations
We will be using the following list as a single operation:
Assignment
Compariron
Multiplication/Division
Addition Subtraction (by extension Negation but only if by itself)
Function Call Overhead

IGNORING ALL DEBUG/PRINT CODE AS NOT PART OF CORE FUNCTIONALITY
(Also assuming casting types and varibale declaration is a no op and optimized by compiler)

e.g.
i++     ->   two ops: increment/addition and assignment
f(a+b)  ->   two ops: addition and function overhead

Delays added after line that causes them unless not possible in which case there source is called out
*/

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

        wait(2*10, SC_NS); //2 op : assignment of 'block', comparison of 'block' (initial)
        for (int block = 0; block < NUM_BLOCKS; block++)
        {
            int sad_val = 0;
            wait(10, SC_NS); //1 op : assignment of 'sad_val'

            wait(2*10, SC_NS); //2 op : assignment of 'i', comparison of 'i' (initial)
            for (int i = 0; i < BLOCK_SIZE; i++)
            {
                mem_port->Read(INPUT1_ADDR + (block * BLOCK_SIZE) + i, data1);
                wait(4*10, SC_NS); //4 op : function call/assignment, addition, multiplication, addition

                mem_port->Read(INPUT2_ADDR + (block * BLOCK_SIZE) + i, data2);
                wait(4*10, SC_NS); //4 op : function call/assignment, addition, multiplication, addition

                int v = (int)data1 - (int)data2;
                wait(2*10, SC_NS); //2 op : assignment of 'v', subtraction

                wait(10, SC_NS); //1 op : comparison of 'v'
                if (v < 0) {
                    v = -v;
                    wait(2*10, SC_NS); //2 op : assignment of 'v', negation of 'v'
                }

                sad_val += v;
                wait(2*10, SC_NS); //2 op : assignment AND increment of 'sad_val'

                //if statement used just for delay logic, this is delay of for loop
                if(i<BLOCK_SIZE-1) wait(3*10, SC_NS); //3 op : comparison of 'i', assignment AND increment of 'i' (loop)
            }
            wait(10, SC_NS); //1 op : comparison of 'i'(final)

            mem_port->Write(SAD_OUTPUT_ADDR + block, (unsigned int)sad_val);
            wait(2*10, SC_NS); //2 op : function call/assignment, addition

            //ignored for timing of approximate timed model
            cout << "SAD[" << block << "] = " << sad_val << endl;

            //if statement used just for delay logic, this is delay of for loop
            if(block<NUM_BLOCKS -1) wait(3*10, SC_NS); //3 op : comparison of 'block', assignment AND increment of 'block' (loop)
        }
        wait(10, SC_NS); //1 op : comparison of 'block'(final)

        //ignored for timing of approximate timed model
        cout << "Simulation time: " << sc_time_stamp() << endl;
        cout << "Total clock cycles: " << sc_time_stamp() / sc_time(4, SC_NS) << endl;

        sc_stop();
    }
};

#endif
