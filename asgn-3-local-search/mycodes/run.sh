#!/bin/bash
# version-1
rm -f output.txt
echo "Generating object files"
g++ max_cut_local_search.cpp -o maxcut.out
echo "All ready, running"
# args : <max_iter_local> <max_iter_grasp>
./maxcut.out 1000 10

# version-2
# echo "Generating object files"
# g++ max_cut_local_search2.cpp -o maxcut.out
# gnu_file=plot.gnuplot
# for alpha in $(seq 0.0 0.1 0.9); do
#     file="stats-$alpha.txt"
#     echo "Running with alpha=$alpha"
#     # args : <max_iter_local> <alpha> <statsFilePath>
#     ./maxcut.out 1000 "$alpha" "$file"

#     # # args [starting from 1, AFTER $gnu_file] :
#     # 1 -> output-file-name, 2 -> plot-title, 3 -> xlabel, 4 -> ylabel, 5 -> path-to-the-data-file, 6 -> x-data-col, 7 -> y-data-col
#     gnuplot -c $gnu_file "GRASP-stats-$alpha.png" "maxcut vs GRASP-iterations" "iterationCount" "maxcut" "$file" 1 2
# done

