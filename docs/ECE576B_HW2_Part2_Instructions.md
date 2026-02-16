# ECE 576B – Homework 2  
## Part 2: Mixed Cycle-Accurate and Approximate-Timed SAD Model

## Overview

In Part 2, you will revise your Part 1 implementation to introduce timing behavior and a cycle-accurate RTL memory model.

You will:

- Add approximate timing to the SAD component
- Implement a cycle-accurate synchronous memory model (MEMORY_RTL)
- Integrate the RTL memory inside the existing memory wrapper
- Maintain compatibility with the `simple_mem_if` interface

This part moves the model from purely functional to timing-aware and clocked behavior.

---

# Part 2A – Approximate-Timed SAD Model

## Timing Assumptions

Assume:

- Every individual operation (addition, subtraction, comparison, etc.) takes 10 ns

---

## Required Changes to SAD Component

You must:

- Insert `wait()` statements to model operation delays
- Annotate timing assumptions using C++ comments
- Maintain functional correctness

Example concept:

    v = a - b;
    wait(10, SC_NS);  // subtraction delay

    if (v < 0)
    {
        wait(10, SC_NS);  // comparison delay
        v = -v;
        wait(10, SC_NS);  // negation delay
    }

    sad += v;
    wait(10, SC_NS);  // addition delay

You must determine where delays logically belong based on operations.

---

## Key Requirements

- Use `wait(time, SC_NS)` for delays
- Clearly comment each modeled delay
- Do not convert SAD to RTL
- SAD remains transaction-level but approximate-timed

---

# Part 2B – Cycle-Accurate RTL Memory Model

You must implement a synchronous RTL memory component named:

    MEMORY_RTL

---

## Required Module Template

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

        MEMORY_RTL(sc_module_name name, char* memInitFilename)
            : sc_module(name)
        {
            SC_METHOD(rtl);
            sensitive << Clk.pos();
        }

        void rtl()
        {
            // memory behavior implemented here
        }
    };

---

## Required Memory Behavior (Synchronous)

On rising edge of Clk:

- If Ren == 1
  - Output memData[Addr] to DataOut
  - Assert Ack if address valid

- If Wen == 1
  - Write DataIn to memData[Addr]
  - Assert Ack if address valid

- If address invalid
  - Ack must remain 0

This models synchronous memory behavior.

---

## Clock Requirements

Assume memory operates at 250 MHz.

You must implement a clock generator using an SC_THREAD:

    void oscillator()
    {
        while (true)
        {
            clk.write(SC_LOGIC_0);
            wait(2, SC_NS);
            clk.write(SC_LOGIC_1);
            wait(2, SC_NS);
        }
    }

This produces a 4 ns clock period.

---

# Integration Requirements

The MEMORY_RTL must be instantiated inside the original memory component.

The original memory component must:

- Continue implementing simple_mem_if
- Use sc_signal to connect to MEMORY_RTL inputs and outputs
- Drive Ren, Wen, Addr, DataIn signals
- Wait for Ack signal before returning
- Return data and success flag via the interface

---

## Signal Connections Example

Within memory wrapper:

    sc_signal<sc_logic> clk;
    sc_signal<sc_logic> ren, wen;
    sc_signal<int> addr;
    sc_signal<int> dataIn;
    sc_signal<int> dataOut;
    sc_signal<sc_logic> ack;

Connect these signals to the MEMORY_RTL instance.

---

## Interface Behavior in Part 2

In the Read and Write methods:

1. Drive address and control signals
2. Wait for positive clock edge(s)
3. Wait for Ack
4. Return appropriate data and status

Unlike Part 1, memory accesses now require synchronization with clock cycles.

---

# Abstraction Levels Comparison

| Part | Model Type | Characteristics |
|------|------------|----------------|
| Part 1 | Untimed | Functional only |
| Part 2A | Approximate-Timed | wait() delays added |
| Part 2B | Cycle-Accurate RTL | Synchronous memory, clocked |

---

# Final Requirements Checklist

You must submit:

- Updated SAD with approximate timing
- MEMORY_RTL implementation
- Memory wrapper integrating RTL
- Clock generator thread
- Makefile
- No executables or build artifacts

---

## Important Notes

- Keep simple_mem_if unchanged
- Maintain compatibility between SAD and memory
- Clearly comment timing assumptions
- Ensure correct synchronous behavior
