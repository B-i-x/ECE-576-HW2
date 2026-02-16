# ECE 576 Homework 2 - SystemC Setup

This project uses Docker to run SystemC examples.

## Prerequisites

- Docker installed on your system
- Docker Compose installed

## Setup

The Docker image is based on `learnwithexamples/systemc` with SystemC 2.3.3 pre-installed.

### Build the Docker image

```bash
docker compose build
```

## Configuration

All configurable defines live in `part1/config.h`. This includes memory size, block dimensions, and address mappings:

```cpp
#define NUM_BLOCKS 64
#define BLOCK_SIZE 64
#define INPUT1_ADDR 0
#define INPUT2_ADDR 16384
#define SAD_OUTPUT_ADDR 32768
#define MEM_SIZE 65536
```

To switch configurations, comment/uncomment the selector at the top of `config.h`:

```cpp
#define CONFIG_DEFAULT      // active
// #define CONFIG_SMALL     // inactive
```

## Memory Files

Memory initialization files are stored in the `memory_files/` directory. These are whitespace-separated integer files that get loaded into the memory module at startup.

### Generate a memory file

Generates `memory_files/memfile_Default.txt` (name based on `CONFIG_NAME` in `config.h`):

```bash
docker compose run --rm generate-memfile
```

### Generate expected SAD solution

Computes the expected SAD output for a given memory file and writes it to `memory_files/memfile_Default_solution.txt`:

```bash
docker compose run --rm generate-solution
```

## Running

### Part 1 - Untimed SAD

Compiles and runs the untimed SAD model against `memfile_Default.txt`:

```bash
docker compose run --rm part1
```

### Hello World example

Compiles and runs `example/hello.cpp`:

```bash
docker compose run --rm example
```

### Interactive shell

To compile and run programs manually:

```bash
docker compose run --rm systemc
```

Inside the container, compile SystemC programs with:

```bash
g++ -I. -I$SYSTEMC/include -L. -L$SYSTEMC/lib-linux64 -lsystemc <your_file.cpp>
```

## Environment Variables

The following environment variables are pre-configured in the Docker container:

- `SYSTEMC=/usr/local/systemc-2.3.3`
- `LD_LIBRARY_PATH=/usr/local/systemc-2.3.3/lib-linux64`

## Project Structure

```
.
├── Dockerfile
├── docker-compose.yml
├── readme.md
├── example/
│   └── hello.cpp               # Hello World SystemC example
├── memory_files/
│   ├── src/
│   │   ├── generate_memfile.cpp          # Generates memory init files
│   │   └── memfile_solution_generator.cpp # Generates expected SAD output
│   ├── memfile_Default.txt               # Generated memory file
│   └── memfile_Default_solution.txt      # Generated expected output
├── output/                      # Compiled binaries (gitignored)
└── part1/
    ├── config.h                 # All configurable defines
    ├── simple_mem_if.h          # Memory interface
    ├── memory.h                 # Memory module
    ├── sad.h                    # SAD computation module
    └── main.cpp                 # Top-level simulation entry
```
