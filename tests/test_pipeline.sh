#!/bin/bash
set -e

SHELL_EXEC=../myshell

echo "Running pipeline tests..."

# ---------- Test 1 : echo + cat ----------
output=$($SHELL_EXEC <<< "echo hello | cat")
output=$(echo -n "$output")
if [[ "$output" != "hello" ]]; then
    echo "Test 1 failed: expected 'hello', got '$output'"
    exit 1
fi
echo "Test 1 passed!"

# ---------- Test 2 : ls | wc -l ----------
output=$($SHELL_EXEC <<< "ls | wc -l")
output=$(echo -n "$output")
if ! [[ "$output" =~ ^[0-9]+$ ]]; then
    echo "Test 2 failed: expected a number, got '$output'"
    exit 1
fi
echo "Test 2 passed!"

# ---------- Test 3 : cd et pwd ----------
output=$($SHELL_EXEC <<< "pwd")
output=$(echo -n "$output")
CURRENT_DIR=$(pwd)

if [[ "$output" != "$CURRENT_DIR" ]]; then
    echo "Test 3 failed: expected '$CURRENT_DIR', got '$output'"
    exit 1
fi
echo "Test 3 passed!"

