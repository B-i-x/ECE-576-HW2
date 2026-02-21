#include <systemc.h>
#include "memory.h"
#include "sad.h"

int sc_main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: sad memfile" << endl;
        return 1;
    }

    // Instantiate modules
    memory mem("memory", argv[1]);
    sad sad_module("sad");

    // Connect SAD port to memory export
    sad_module.mem_port(mem.target);

    // Run simulation
    sc_start();

    return 0;
}
