#!/bin/bash

EXEC="./$1"

echo ""
LD_LIBRARY_PATH=. valgrind --leak-check=full "$EXEC"
# LD_LIBRARY_PATH=. valgrind --tool=massif "$EXEC"
