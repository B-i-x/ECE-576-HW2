SYSTEMC_HOME ?= $(HOME)/systemc-2.3.3-install
BUILD_DIR := build

all:
	cmake -S . -B $(BUILD_DIR) -DSYSTEMC_HOME=$(SYSTEMC_HOME)
	cmake --build $(BUILD_DIR)

gen_memfile: all
	cmake --build $(BUILD_DIR) --target gen_memfile

gen_solution: all
	cmake --build $(BUILD_DIR) --target gen_solution

run_part1: all
	cmake --build $(BUILD_DIR) --target run_part1

run_part2: all
	cmake --build $(BUILD_DIR) --target run_part2

clean:
	rm -rf $(BUILD_DIR)
