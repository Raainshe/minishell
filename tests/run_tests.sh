#!/bin/bash
# Script to run all tokenizer and garbage collector tests

# Set colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
RESET='\033[0m'

# Check if valgrind is installed
if command -v valgrind &> /dev/null; then
    HAS_VALGRIND=true
    echo -e "${GREEN}Valgrind detected. Will perform memory leak checks.${RESET}"
else
    HAS_VALGRIND=false
    echo -e "${YELLOW}Valgrind not found. Will skip memory leak checks.${RESET}"
fi

# Compile all tests
echo -e "\n${YELLOW}Compiling tests...${RESET}"
make
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed!${RESET}"
    exit 1
fi
echo -e "${GREEN}Compilation successful!${RESET}"

# Run functionality tests
echo -e "\n${YELLOW}Running functionality tests...${RESET}"
./test_tokenizer
if [ $? -ne 0 ]; then
    echo -e "${RED}Functionality tests failed!${RESET}"
    exit 1
fi
echo -e "${GREEN}Functionality tests passed!${RESET}"

# Run memory leak tests
echo -e "\n${YELLOW}Running memory leak tests...${RESET}"
./test_memory_leaks
if [ $? -ne 0 ]; then
    echo -e "${RED}Memory leak tests failed!${RESET}"
    exit 1
fi
echo -e "${GREEN}Memory leak tests passed!${RESET}"

# Run valgrind tests if available
if [ "$HAS_VALGRIND" = true ]; then
    echo -e "\n${YELLOW}Running valgrind memory leak check on test_tokenizer...${RESET}"
    VALGRIND_OUTPUT=$(valgrind --leak-check=full --error-exitcode=1 ./test_tokenizer 2>&1)
    if [ $? -ne 0 ]; then
        echo -e "${RED}Memory leaks detected in test_tokenizer!${RESET}"
        echo "$VALGRIND_OUTPUT" | grep -A 5 "definitely lost" | head -n 20
        echo -e "${YELLOW}See full output for more details.${RESET}"
    else
        echo -e "${GREEN}No memory leaks detected in test_tokenizer!${RESET}"
    fi

    echo -e "\n${YELLOW}Running valgrind memory leak check on test_memory_leaks...${RESET}"
    VALGRIND_OUTPUT=$(valgrind --leak-check=full --error-exitcode=1 ./test_memory_leaks 2>&1)
    if [ $? -ne 0 ]; then
        echo -e "${RED}Memory leaks detected in test_memory_leaks!${RESET}"
        echo "$VALGRIND_OUTPUT" | grep -A 5 "definitely lost" | head -n 20
        echo -e "${YELLOW}See full output for more details.${RESET}"
    else
        echo -e "${GREEN}No memory leaks detected in test_memory_leaks!${RESET}"
    fi
fi

echo -e "\n${GREEN}All tests completed successfully!${RESET}"
