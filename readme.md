# ECE 576 Homework 2 - SystemC Setup

This project uses Docker to run SystemC examples.

## Prerequisites

- Docker installed on your system
- Docker Compose installed

## Setup

The Docker image is based on `learnwithexamples/systemc` with SystemC 2.3.3 pre-installed.

## Commands

### Build the Docker image

```bash
docker compose build
```

### Run the Hello World example

This command compiles and runs `example/hello.cpp`:

```bash
docker compose run --rm example
```

### Open an interactive shell

To compile and run custom programs manually:

```bash
docker compose run --rm systemc
```

Inside the container, compile your SystemC programs with:

```bash
g++ -I. -I$SYSTEMC/include -L. -L$SYSTEMC/lib-linux64 -lsystemc <your_file.cpp>
./a.out
```

## Environment Variables

The following environment variables are pre-configured in the Docker container:

- `SYSTEMC=/usr/local/systemc-2.3.3`
- `LD_LIBRARY_PATH=/usr/local/systemc-2.3.3/lib-linux64`

## Project Structure

```
.
├── Dockerfile              # Docker image configuration
├── docker-compose.yml      # Docker Compose services
├── example/
│   └── hello.cpp          # Hello World SystemC example
└── README.md              # This file
```
