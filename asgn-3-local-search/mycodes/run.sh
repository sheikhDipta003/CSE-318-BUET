#!/bin/bash
# version-1
# rm -f output.txt
echo "Generating object files"
g++ max_cut_local_search.cpp -o maxcut.out
echo "All ready, running"
echo "Using greedy_construction"
# args : <max_iter_grasp>
./maxcut.out 20

