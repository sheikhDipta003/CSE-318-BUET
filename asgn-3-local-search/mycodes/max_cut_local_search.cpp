#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <algorithm>

using namespace std;

// Function to calculate the cut value of the given partition
int calculateCut(const vector<vector<int>>& graph, const vector<int>& partition) {
    int cutValue = 0;
    for (size_t i = 0; i < graph.size(); i++) {
        for (size_t j = i + 1; j < graph[i].size(); j++) {
            if (partition[i] != partition[j]) {
                cutValue += graph[i][j];
            }
        }
    }
    return cutValue;
}

// Function to generate a random initial partition
vector<int> generateRandomPartition(size_t N) {
    vector<int> partition(N, 0);
    for (size_t i = 0; i < N / 2; i++) {
        partition[i] = 1;
    }
    random_shuffle(partition.begin(), partition.end());
    return partition;
}

// Greedy randomized construction of initial solution
vector<int> greedyRandomizedConstruction(const vector<vector<int>>& graph, double alpha) {
    size_t N = graph.size();
    vector<int> currentPartition(N, 0);
    vector<int> rem_vertices(N);

    for (size_t i = 0; i < N; i++) {
        rem_vertices[i] = i;
    }

    int currentSet = 1; // Start with set 1
    int rem_count = N;

    while (rem_count > 0) {
        // Greedily select the next vertex to include in the current set
        int v = -1;             // the vertex with maximum degree
        int w = INT_MIN;        // value of the maximum degree

        for (size_t i = 0; i < rem_count; i++) {
            int u = rem_vertices[i];
            int weight = 0;

            for (size_t j = 0; j < N; j++) {
                if (currentPartition[j] == currentSet) {
                    weight += graph[u][j];
                }
            }

            if (weight > w) {
                w = weight;
                v = u;
            }
        }

        // Randomized greedy selection based on alpha
        if (rand() / static_cast<double>(RAND_MAX) < alpha) {
            currentPartition[v] = currentSet;
        }

        // Remove the selected vertex from the remaining vertices
        int k = -1;
        for (size_t i = 0; i < rem_count; i++) {
            if (rem_vertices[i] == v) {
                k = i;
                break;
            }
        }

        if (k != -1) {
            rem_vertices[k] = rem_vertices[rem_count - 1];
            rem_count--;
        }

        currentSet = 1 - currentSet; // Switch to the other set for the next iteration
    }

    return currentPartition;
}

// Local search algorithm to find a Max-Cut solution
vector<int> localSearchMaxCut(const vector<vector<int>>& graph, vector<int> initialPartition, int max_iter_count) {
    size_t N = graph.size();
    vector<int> currentPartition = initialPartition;
    int currentCut = calculateCut(graph, currentPartition);

    for (int i = 0; i < max_iter_count; i++) {
        int v = rand() % N;                            // select a vectex randomly
        currentPartition[v] = 1 - currentPartition[v]; // Move the vertex to the other set
        int newCut = calculateCut(graph, currentPartition);

        if (newCut > currentCut) {
            // Keep the move if it improves the cut
            currentCut = newCut;
        } else {
            // Revert the move if it doesn't improve the cut
            currentPartition[v] = 1 - currentPartition[v];
        }
    }

    return currentPartition;
}

// GRASP with local search algorithm to find a Max-Cut solution
vector<int> graspMaxCut(const vector<vector<int>>& graph, double alpha, int max_iter_grasp, int max_iter_local) {
    vector<int> bestSolution;
    int bestCutValue = INT_MIN;

    for (int i = 0; i < max_iter_grasp; i++) {
        vector<int> currentSolution = greedyRandomizedConstruction(graph, alpha);
        vector<int> improvedSolution = localSearchMaxCut(graph, currentSolution, max_iter_local); // Use local search for improvement
        int currentCutValue = calculateCut(graph, improvedSolution);

        if (currentCutValue > bestCutValue) {
            bestCutValue = currentCutValue;
            bestSolution = improvedSolution;
        }
    }

    return bestSolution;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    // Sample graph represented using an adjacency matrix
    vector<vector<int>> graph = {
        {0, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0, 0},
        {1, 1, 0, 0, 1, 1},
        {1, 1, 0, 0, 1, 1},
        {0, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 0, 0}
    };

    double alpha = 0.3; // GRASP parameter (0 <= alpha <= 1)
    int max_iter_grasp = 10; // Number of GRASP iterations
    int max_iter_local = 1000; // Number of iterations for local search
    vector<int> maxCutPartition = graspMaxCut(graph, alpha, max_iter_grasp, max_iter_local);

    // Output the results
    cout << "Max-Cut Partition: ";
    for (int partition : maxCutPartition) {
        cout << partition << " ";
    }
    cout << endl;

    int maxCutValue = calculateCut(graph, maxCutPartition);
    cout << "Max-Cut Value: " << maxCutValue << endl;

    return 0;
}
