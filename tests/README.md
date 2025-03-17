# Tokenizer and Garbage Collector Tests

This directory contains tests for the minishell tokenizer and garbage collector functionality.

## Test Programs

1. **test_tokenizer**: Tests the functionality of the tokenizer and garbage collector
2. **test_memory_leaks**: Specifically tests for memory leaks in the garbage collector and tokenizer

## Test Cases in test_tokenizer

The test_tokenizer program includes the following test cases:

1. **Simple Command Test**: Tests basic command tokenization (e.g., `ls -la`)
2. **Pipes Test**: Tests commands with pipes (e.g., `ls -la | grep a | wc -l`)
3. **Redirections Test**: Tests input/output redirections (e.g., `cat < input.txt > output.txt`)
4. **Here Doc and Append Test**: Tests here document and append redirections (e.g., `cat << EOF >> output.txt`)
5. **Quotes Test**: Tests handling of single and double quotes (e.g., `echo "Hello 'World'" 'Single "Quotes"'`)
6. **Complex Command Test**: Tests a complex command combining multiple features
7. **Edge Cases Test**: Tests various edge cases like empty input, no spaces around operators, etc.
8. **Garbage Collector Test**: Tests the garbage collector's ability to allocate, track, and free memory

## Test Cases in test_memory_leaks

The test_memory_leaks program includes the following tests:

1. **gc_malloc and free_gc Test**: Tests the basic allocation and freeing functionality
2. **Tokenizer Memory Management Test**: Tests memory management during tokenization of different inputs
3. **Nested Tokenization Test**: Tests memory accumulation with nested tokenization without intermediate freeing

## Running the Tests

To compile and run the tests:

1. Make sure you're in the `tests` directory:
   ```
   cd tests
   ```

2. Compile the test programs:
   ```
   make
   ```

3. Run the functionality tests:
   ```
   ./test_tokenizer
   ```

4. Run the memory leak tests:
   ```
   ./test_memory_leaks
   ```

5. For thorough memory leak checking, use Valgrind:
   ```
   valgrind --leak-check=full ./test_tokenizer
   valgrind --leak-check=full ./test_memory_leaks
   ```

## Expected Output

### test_tokenizer

The test program will output information about each test, including:
- The input string being tokenized
- The tokens identified, their content, and type
- Messages indicating when memory for a context is freed

### test_memory_leaks

This program will output information about:
- Memory allocations and their addresses
- When memory is added to contexts
- When contexts are freed
- When all memory is freed

All tests should run without crashes, and the garbage collector tests should properly free all allocated memory.

## Checking for Memory Leaks

When running with Valgrind, you should see "All heap blocks were freed -- no leaks are possible"
if the garbage collector is working correctly. If there are leaks, Valgrind will show details about
the leaked memory blocks and where they were allocated.
