# Simple Shell 1.0

A basic UNIX command-line interpreter implemented in C as part of the low-level programming curriculum at Holberton School. This shell mimics the fundamental behaviors of the standard `/bin/sh` shell.

## Description
This project is a simple command-line interface that reads commands from standard input, parses them into executable arguments, handles path resolutions, manages environment structures, and executes them within newly forked child processes.

## Features
- Displays a prompt (`$ `) and waits for user input in interactive mode.
- Functions seamlessly in non-interactive environments (e.g., pipe inputs like `echo "ls" | ./hsh`).
- Tracks and reflects accurate system status codes (e.g., returns `127` when a command is not found).
- Isolates directory lookups strictly conforming to POSIX constraints when the `PATH` variable is customized, blank, or unset.
- **Built-in Commands:**
  - `exit`: Smoothly terminates the shell execution environment.
  - `env`: Prints all active environment variables to standard output.

## Compilation & Installation
The code is written in compliance with the `gnu89` standard and can be compiled using:
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
