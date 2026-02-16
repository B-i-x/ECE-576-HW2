# ECE 576B -- Homework 2

## Part 1: Untimed SAD Component

## Overview

In Part 1, you will implement an untimed SystemC model of a **Sum of
Absolute Differences (SAD)** system.\
The system consists of:

-   A SAD component
-   A memory component
-   A simple interface connecting them (`simple_mem_if`)

The goal is to model the functional behavior of the SAD computation
without timing.

------------------------------------------------------------------------

## System Description

The SAD component computes the **sum of absolute differences** between
two blocks of data stored in memory.

The system structure:

-   SAD reads two input blocks from memory
-   Computes absolute difference per element
-   Accumulates the sum for each block
-   Writes result back to memory
-   Prints result to console during simulation

------------------------------------------------------------------------

## Required Configuration Defines

Your SAD implementation must use the following configurable macros:

``` cpp
#define NUM_BLOCKS 64
#define BLOCK_SIZE 64
#define INPUT1_ADDR 0
#define INPUT2_ADDR 16384
#define SAD_OUTPUT_ADDR 32768
```

These values must be customizable.

------------------------------------------------------------------------

## SAD Algorithm (Functional Behavior)

For each block:

``` cpp
for (block = 0; block < NUM_BLOCKS; block++)
{
    sad = 0;

    for (i = 0; i < BLOCK_SIZE; i++)
    {
        v = MEM[INPUT1_ADDR + (block * BLOCK_SIZE) + i]
          - MEM[INPUT2_ADDR + (block * BLOCK_SIZE) + i];

        if (v < 0)
            v = -v;

        sad += v;
    }

    MEM[SAD_OUTPUT_ADDR + block] = sad;
}
```

### Requirements:

-   Perform memory reads using the interface
-   Compute absolute value correctly
-   Store SAD result in memory
-   Print each block's SAD result to console
-   All behavior occurs at current simulation time (no timing in Part 1)

------------------------------------------------------------------------

## simple_mem_if Interface

The SAD must communicate with memory through this interface:

``` cpp
class simple_mem_if : virtual public sc_interface
{
public:
    virtual bool Write(unsigned int addr, unsigned int data) = 0;
    virtual bool Read(unsigned int addr, unsigned int& data) = 0;
};
```

### Interface Rules:

-   `Read(addr, data)`
    -   Returns true if address valid\
    -   Returns false if invalid
-   `Write(addr, data)`
    -   Returns true if address valid\
    -   Returns false if invalid

The SAD must not access memory directly.

------------------------------------------------------------------------

## Memory Component Requirements

### 1. Memory Characteristics

-   32-bit wide memory
-   Size defined with:

``` cpp
#define MEM_SIZE
```

------------------------------------------------------------------------

### 2. Memory Initialization

-   Memory must be initialized from a file
-   File provided as a command-line argument:

```{=html}
<!-- -->
```
    sad memfile

-   File contains whitespace-separated integer values
-   Initialization must occur in the memory constructor
-   If file does not contain enough values:
    -   Remaining memory locations must be initialized to 0

------------------------------------------------------------------------

## Command Line Requirements

Your program must accept:

    sad memfile

Where:

-   `memfile` is the path to the memory initialization file

------------------------------------------------------------------------

## Functional Requirements Checklist

You must implement:

-   SAD module
-   Memory module
-   simple_mem_if interface
-   Command-line file loading
-   Memory initialization logic
-   Proper error handling for invalid memory addresses
-   Console printing of SAD results

------------------------------------------------------------------------

## Important Notes

-   This is an untimed model
-   No `wait()` statements should be used in Part 1
-   Focus strictly on functional correctness
-   All communication must go through `simple_mem_if`
