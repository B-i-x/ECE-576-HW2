#ifndef MEMORY_H
#define MEMORY_H

#include <systemc.h>
#include "simple_mem_if.h"
#include "../config/config.h"
#include "memory_rtl.h"

class memory : public sc_module, public simple_mem_if
{
public:
    sc_export<simple_mem_if> target;

    // Internal signals connecting to MEMORY_RTL
    sc_signal<sc_logic> clk_sig;
    sc_signal<sc_logic> ren_sig, wen_sig;
    sc_signal<int> addr_sig;
    sc_signal<int> dataIn_sig;
    sc_signal<int> dataOut_sig;
    sc_signal<sc_logic> ack_sig;

    // RTL memory instance
    MEMORY_RTL* mem_rtl;

    SC_HAS_PROCESS(memory);

    memory(sc_module_name name, const char* filename) : sc_module(name)
    {
        target.bind(*this);

        // Instantiate MEMORY_RTL and connect signals
        mem_rtl = new MEMORY_RTL("mem_rtl", filename);
        mem_rtl->Clk(clk_sig);
        mem_rtl->Ren(ren_sig);
        mem_rtl->Wen(wen_sig);
        mem_rtl->Addr(addr_sig);
        mem_rtl->DataIn(dataIn_sig);
        mem_rtl->DataOut(dataOut_sig);
        mem_rtl->Ack(ack_sig);

        // Start clock generator thread
        SC_THREAD(oscillator);
    }

    ~memory()
    {
        delete mem_rtl;
    }

    // 250 MHz clock generator (4 ns period: 2 ns low, 2 ns high)
    void oscillator()
    {
        while (true)
        {
            clk_sig.write(SC_LOGIC_0);
            wait(2, SC_NS);
            clk_sig.write(SC_LOGIC_1);
            wait(2, SC_NS);
        }
    }

    bool Write(unsigned int addr, unsigned int data)
    {
        // Drive address, data, and write enable signals
        addr_sig.write((int)addr);
        dataIn_sig.write((int)data);
        wen_sig.write(SC_LOGIC_1);
        ren_sig.write(SC_LOGIC_0);

        // Wait for rising clock edge so MEMORY_RTL processes the request
        wait(clk_sig.posedge_event());
        // Wait one delta cycle for Ack to propagate from MEMORY_RTL
        wait(SC_ZERO_TIME);

        // Check acknowledgment
        bool success = (ack_sig.read() == SC_LOGIC_1);

        // Deassert write enable
        wen_sig.write(SC_LOGIC_0);

        return success;
    }

    bool Read(unsigned int addr, unsigned int& data)
    {
        // Drive address and read enable signals
        addr_sig.write((int)addr);
        ren_sig.write(SC_LOGIC_1);
        wen_sig.write(SC_LOGIC_0);

        // Wait for rising clock edge so MEMORY_RTL processes the request
        wait(clk_sig.posedge_event());
    
        wait(clk_sig.posedge_event()); //i have to wait for 2 clock cycles to get the data out of the memory, otherwise I get a delta cycle and the data is not updated yet

        // Check acknowledgment and read data
        bool success = (ack_sig.read() == SC_LOGIC_1);
        if (success)
            data = (unsigned int)dataOut_sig.read();

        // Deassert read enable
        ren_sig.write(SC_LOGIC_0);

        return success;
    }
};

#endif
