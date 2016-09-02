#!/usr/bin/env bash
eval "timeout 15s valgrind --tool=callgrind ./solvesudoku < samples/combined_21886.txt"

