#!/bin/bash
echo "Generating object files"
g++ 1905003.cpp -o 1905003.out
echo "All ready, running"
echo "Using semi_greedy_construction"
# args : <max_iter_grasp>
./1905003.out 25
