#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Function to calculate the cut value of the given partition
int calculateCut(const vector<vector<int>>& graph, const vector<int>& partition) {
    int cutValue = 0;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = i + 1; j < graph[i].size(); j++) {
            if (partition[i] != partition[j]) {
                cutValue += graph[i][j];
            }
        }
    }
    return cutValue;
}

// returns true if set1 and set2 are identical
bool are_sets_identical(const unordered_set<int>& set1, const unordered_set<int>& set2) {
    if (set1.size() != set2.size()) {
        return false;
    }

    for (int element : set1) {
        if (set2.find(element) == set2.end()) {
            return false;
        }
    }

    return true;
}

// prints all elements of any container
template <typename Container>
void showElements(const Container& container) {
    for (const auto& element : container) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

// Semi-greedy randomized construction of initial solution with RCL (Restricted Candidate List)
vector<int> semi_greedy_construction(const vector<vector<int>>& graph) {
    int n = graph.size(), wmin = INT_MAX, wmax = INT_MIN;
    double alpha = rand() / RAND_MAX;
    double thres;
    vector<pair<int, int>> e_rcl;
    vector<int> v_rcl;
    unordered_set<int> X, Y, XY, R, U;
    vector<int> resultPartition(n, 0);      // X ::= 0, Y ::= 1

    for(int i = 0; i < n; i++)  { R.insert(i); U.insert(i); }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(graph[i][j] < wmin){
                wmin = graph[i][j];
            }
            if(graph[i][j] > wmax){
                wmax = graph[i][j];
            }
        }
    }

    thres = wmin + alpha * (wmax - wmin);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(graph[i][j] >= thres){
                e_rcl.push_back({i,j});
            }
        }
    }

    pair<int,int> random_edge = e_rcl[rand() % e_rcl.size()];
    X.insert(random_edge.first);
    Y.insert(random_edge.second);
    
    while(true){
        // store union of X and Y in XY and subtract XY from U to get R
        XY = X;
        XY.insert(Y.begin(), Y.end());
        set_difference(U.begin(), U.end(), XY.begin(), XY.end(), inserter(R, R.end()));
        if(are_sets_identical(XY,U)) break;     // if all the vertices are partitioned, break out of the loop

        unordered_map<int,int> sigmaX, sigmaY;  // vertex, cut-weight-contribution
        for(int v : R){
            sigmaX[v] = 0;
            for(int u = 0; u < n; u++){
                if(graph[v][u] && Y.count(u) > 0){
                    sigmaX[v] += graph[v][u];
                }
            }

            sigmaY[v] = 0;
            for(int u = 0; u < n; u++){
                if(graph[v][u] && X.count(u) > 0){
                    sigmaY[v] += graph[v][u];
                }
            }
        }

        int sigmaX_min = INT_MAX, sigmaY_min = INT_MAX, sigmaX_max = INT_MIN, sigmaY_max = INT_MIN;
        
        for(int v : R){
            if(sigmaX[v] < sigmaX_min){
                sigmaX_min = sigmaX[v];
            }
        }
        for(int v : R){
            if(sigmaY[v] < sigmaY_min){
                sigmaY_min = sigmaY[v];
            }
        }
        wmin = min(sigmaX_min, sigmaY_min);

        for(int v : R){
            if(sigmaX[v] > sigmaX_max){
                sigmaX_max = sigmaX[v];
            }
        }
        for(int v : R){
            if(sigmaY[v] > sigmaY_max){
                sigmaY_max = sigmaY[v];
            }
        }
        wmax = max(sigmaX_max, sigmaY_max);

        thres = wmin + alpha * (wmax - wmin);

        for(int v : R){
            if(max(sigmaX[v], sigmaY[v]) >= thres){
                v_rcl.push_back(v);
            }
        }

        int random_vertex = v_rcl[rand() % v_rcl.size()];

        if(sigmaX[random_vertex] > sigmaY[random_vertex]){
            X.insert(random_vertex);
        }
        else{
            Y.insert(random_vertex);
        }
    }

    for(int v : Y){
        resultPartition[v] = 1;
    }

    return resultPartition;
}

// Local search algorithm to find a Max-Cut solution
vector<int> local_search_max_cut(const vector<vector<int>>& graph, vector<int> initialPartition, int max_iter_count) {
    int n = graph.size();
    vector<int> currentPartition = initialPartition;
    int currentCut = calculateCut(graph, currentPartition);

    for (int i = 0; i < max_iter_count; i++) {
        int v = rand() % n;                                     // select a vectex randomly
        currentPartition[v] = 1 - currentPartition[v];          // Move the vertex to the other set
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
vector<int> grasp_max_cut(const vector<vector<int>>& graph, int max_iter_grasp, int max_iter_local) {
    vector<int> bestSolution;
    int bestCutValue = INT_MIN;

    for (int i = 0; i < max_iter_grasp; i++) {
        vector<int> currentSolution = semi_greedy_construction(graph);
        vector<int> improvedSolution = local_search_max_cut(graph, currentSolution, max_iter_local); // Use local search for improvement
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
        {0, 5, 0, 0, 6},
        {5, 0, 9, 0, 10},
        {0, 9, 0, 15, 2},
        {0, 0, 15, 0, 10},
        {6, 10, 2, 10, 0}
    };

    int max_iter_grasp = 10;        // Number of GRASP iterations
    int max_iter_local = 1000;      // Number of iterations for local search
    vector<int> maxCutPartition = grasp_max_cut(graph, max_iter_grasp, max_iter_local);

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
