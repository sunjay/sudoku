#!/usr/bin/env bash
eval "timeout 30s valgrind --tool=callgrind ./solvesudoku < samples/longest10.txt"

