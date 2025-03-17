# Minishell

A simple shell implementation for 42 school.

## Features

- Command execution
- Pipes (`|`)
- Redirections (`<`, `>`, `<<`, `>>`)
- Support for quotes and environments
- Built-in commands

## Building

```
make
```

## Usage

```
./minishell
```

## Testing

The project includes comprehensive tests for the tokenizer and garbage collector components.

### Running Tests

To run all tests:

```
cd tests
./run_tests.sh
```

This script:
1. Compiles all test programs
2. Runs functionality tests
3. Runs memory leak tests
4. Performs Valgrind checks if available

### Available Test Programs

- `test_tokenizer`: Tests tokenizer functionality with various inputs
- `test_memory_leaks`: Tests garbage collector with focus on memory management

For more details about the tests, see the [tests README](tests/README.md).

## Components

### Tokenizer

Parses input strings into tokens with appropriate types (words, pipes, redirections, etc.).

### Garbage Collector

Manages memory allocations with context-based tracking, allowing for controlled memory freeing.

## Authors

- ksinn - [GitHub](https://github.com/Fearcon14)
- rmakoni - [GitHub](https://github.com/raainshe)
