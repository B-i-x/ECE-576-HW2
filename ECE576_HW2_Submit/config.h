#ifndef CONFIG_H
#define CONFIG_H

// Select active configuration here:
#define CONFIG_DEFAULT
// #define CONFIG_CUSTOM

#ifdef CONFIG_DEFAULT

#define CONFIG_NAME "Default"
#define NUM_BLOCKS 64
#define BLOCK_SIZE 64
//there are TWO grids of 64x64 pixels
#define INPUT1_ADDR 0 //address of 1st grid, the end address of this grid is 4096
#define INPUT2_ADDR 16384 //address of 2nd grid, the end address of this grid is 20479
#define SAD_OUTPUT_ADDR 32768 //address of SAD output, the end address of this grid is 32768 + 64 = 32832
#define MEM_SIZE 65536

#elif defined(CONFIG_SMALL)

#define CONFIG_NAME "Small"
#define NUM_BLOCKS 32
#define BLOCK_SIZE 32
#define INPUT1_ADDR 0
#define INPUT2_ADDR 4096
#define SAD_OUTPUT_ADDR 8192
#define MEM_SIZE 16384

#else
#error "No configuration selected. Define CONFIG_DEFAULT or CONFIG_CUSTOM in config.h"
#endif

//safety checks
static_assert(INPUT2_ADDR > INPUT1_ADDR + NUM_BLOCKS * BLOCK_SIZE, "Addresses overlap, increase INPUT2_ADDR");
static_assert(MEM_SIZE > INPUT2_ADDR + NUM_BLOCKS * BLOCK_SIZE, "Address overlaps memory limit, increase MEM_SIZE");
static_assert(INPUT2_ADDR >= INPUT1_ADDR + NUM_BLOCKS * BLOCK_SIZE, "INPUT1/INPUT2 overlap");
static_assert(SAD_OUTPUT_ADDR >= INPUT2_ADDR + NUM_BLOCKS * BLOCK_SIZE, "INPUT2/OUTPUT overlap");
static_assert(MEM_SIZE >= SAD_OUTPUT_ADDR + NUM_BLOCKS, "SAD output exceeds memory");
#endif
