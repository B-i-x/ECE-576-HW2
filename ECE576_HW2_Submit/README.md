# ECE 576 HW2 — SystemC SAD (Part 1 + Part 2)

This folder contains my **ECE 576 HW2 submission** for the SystemC SAD assignment.

## Contents

- `part1/`  
  SystemC SAD implementation using the provided memory interface.

- `part2/`  
  RTL-style memory + SAD implementation (clocked), prints SAD outputs and simulation timing.

- `memory_files/`  
  - `memfile_Default.txt` (input memory file)
  - `memfile_Default_solution.txt` (expected output)
  - `src/` generators:
    - `generate_memfile.cpp` — generates `memfile_<CONFIG_NAME>.txt`
    - `memfile_solution_generator.cpp` — generates `memfile_<CONFIG_NAME>_solution.txt`

- `config.h`  
  Local copy of configuration used by the submission (so config is at the top level).

- `CMakeLists.txt`  
  Builds everything using CMake.

- `Makefile`  
  Convenience wrapper around CMake targets.

---

## Requirements

- CMake + GNU Make
- A working SystemC installation (tested with SystemC 2.3.3)

---

## Setup: point to your SystemC install

Set `SYSTEMC_HOME` to your SystemC installation directory (must contain `include/systemc.h`).

Example:
```bash
export SYSTEMC_HOME=$HOME/systemc-2.3.3-install
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib:$SYSTEMC_HOME/lib-linux64:$SYSTEMC_HOME/lib64:$LD_LIBRARY_PATH
```

---

## Build (recommended)

From this submission directory:

```bash
make SYSTEMC_HOME=$SYSTEMC_HOME
```

This builds:
- `part1_sad`
- `part2_sad`
- `generate_memfile`
- `generate_solution`

---

## Generate the memory file + expected solution

```bash
make gen_memfile SYSTEMC_HOME=$SYSTEMC_HOME
make gen_solution SYSTEMC_HOME=$SYSTEMC_HOME
```

Outputs (created in `memory_files/`):
- `memory_files/memfile_<CONFIG_NAME>.txt`
- `memory_files/memfile_<CONFIG_NAME>_solution.txt`

For the default config:
- `memory_files/memfile_Default.txt`
- `memory_files/memfile_Default_solution.txt`

---

## Run Part 1 and Part 2

```bash
make run_part1 SYSTEMC_HOME=$SYSTEMC_HOME
make run_part2 SYSTEMC_HOME=$SYSTEMC_HOME
```

- Both print `SAD[0] ... SAD[63]`
- Part 2 additionally prints simulation time and total clock cycles.

---

## Quick correctness check (diff)

Compare runtime output to expected solution:

```bash
make run_part1 SYSTEMC_HOME=$SYSTEMC_HOME | grep '^SAD' > /tmp/part1_out.txt
diff -u memory_files/memfile_Default_solution.txt /tmp/part1_out.txt

make run_part2 SYSTEMC_HOME=$SYSTEMC_HOME | grep '^SAD' > /tmp/part2_out.txt
diff -u memory_files/memfile_Default_solution.txt /tmp/part2_out.txt
```

If `diff` prints nothing, the output matches.

---

## Notes

- The generators in `memory_files/src/` use `CONFIG_NAME` from the config to name output files.
- This submission keeps `config.h` at the top level for a clean “config-by-itself” layout.
