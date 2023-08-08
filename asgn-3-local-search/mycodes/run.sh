#!/bin/bash
echo "Generating object files"
g++ max_cut_local_search.cpp -o maxcut.out
echo "All ready, running..."
./maxcut.out