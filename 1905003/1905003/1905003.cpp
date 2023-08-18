#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <random>

using namespace std;

void printResult(const vector<vector<int>>& graph, const vector<int>& finalPartition);

ofstream outFile("output.txt", std::ios::app);
ofstream statsFile("stats.txt", std::ios::app);
ifstream inFile("inputs/g43.rud");

int k = 0;      //total number of iteration of local_search_max_cut() for a given 'max_iter_grasp' in main()
int cutValue = 0;   //summation of the cut-values of each call to local_search_max_cut() for a given 'max_iter_grasp' in main()

// Function to calculate the cut value of the given partition
int calculateCut(const vector<vector<int>>& graph, const vector<int>& partition) {
    int cutValue = 0;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = i + 1; j < graph[i].size(); j++) {
            if (partition[i] != -1 && partition[i] != partition[j]) {
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

// prints all elements of any container in given outputstream
template <typename Container>
void showElements(const Container& container, std::ostream& os = std::cout) {
    os << "{";
    auto it = container.begin();
    if (it != container.end()) {
        os << *it;
        it++;
    }
    for (; it != container.end(); it++) {
        os << ", " << *it;
    }
    os << "}";
}

// Function to find the maximum weight edge in the graph
tuple<int,int,int> findMaxWeightEdge(vector<vector<int>> graph) {
    int u = -1, v = -1, wt = INT_MIN, n = graph.size();
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (graph[i][j] > wt) {
                wt = graph[i][j];
                u = i;
                v = j;
            }
        }
    }

    return make_tuple(u,v,wt);
}

// Greedy contstruction method for the initial solution in GRASP
vector<int> greedy_construction(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> currentPartition(n, -1);
    int currentCut = -1;

    int u, v, wt;
    tie(u, v, wt) = findMaxWeightEdge(graph);
    currentPartition[u] = 0;
    currentPartition[v] = 1;

    for(int i = 0; i < n; i++){
        if (currentPartition[i] == -1) {
            currentPartition[i] = 0;
            int newCut_0 = calculateCut(graph, currentPartition);
            currentPartition[i] = 1;
            int newCut_1 = calculateCut(graph, currentPartition);
            if(newCut_0 >= newCut_1){
                currentPartition[i] = 0;
            }
            else{
                currentPartition[i] = 1;
            }
        }
    }

    // statsFile << "greedy_construction: " << calculateCut(graph, currentPartition) << endl;
    // printResult(graph, currentPartition);

    return currentPartition;
}

// simple random heuristic for the initial solution in GRASP
vector<int> randomized_construction(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> vertices(n, 0);
    vector<int> currentPartition(n, -1);
    for(int i = 0; i < n; i++) vertices[i] = i;

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> uniform_dist(0, 1);

    for (int v : vertices) {
        // Randomly assign vertex to partition X (0) or partition Y (1)
        currentPartition[v] = uniform_dist(gen);
    }

    // statsFile << "randomized_construction: " << calculateCut(graph, currentPartition) << endl;
    // printResult(graph, currentPartition);

    return currentPartition;
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
        for(int j = i+1; j < n; j++){
            if(graph[i][j] < wmin){
                wmin = graph[i][j];
            }
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(graph[i][j] > wmax){
                wmax = graph[i][j];
            }
        }
    }

    thres = wmin + alpha * (wmax - wmin);
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(graph[i][j] >= thres){
                e_rcl.push_back({i,j});
            }
        }
    }

    pair<int,int> random_edge = e_rcl[rand() % e_rcl.size()];
    X.insert(random_edge.first);
    Y.insert(random_edge.second);
    
    // until all the vertices are partitioned, continue the loop
    while(!are_sets_identical(XY,U)){
        // subtract XY from U to get R
        set_difference(U.begin(), U.end(), XY.begin(), XY.end(), inserter(R, R.end()));

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

        // testing
        // showElements(X, outFile);
        // outFile << endl;
        // showElements(Y, outFile);
        // outFile << endl << endl;

        // store union of X and Y in XY
        XY = X;
        XY.insert(Y.begin(), Y.end());
    }

    for(int v : Y){
        resultPartition[v] = 1;
    }

    // statsFile << "semi_greedy_construction: " << calculateCut(graph, resultPartition) << endl;

    return resultPartition;
}

// Local search algorithm to find a Max-Cut solution
vector<int> local_search_max_cut(const vector<vector<int>>& graph, vector<int> initialPartition) {
    int n = graph.size();
    vector<int> currentPartition = initialPartition;
    bool change = true;
    int i = 0;

    while(change){
        change = false;
        i++;
        for(int v = 0; v < n; v++){
            int sigma_0 = 0, sigma_1 = 0;
            for(int u = 0; u < n; u++){
                if(graph[v][u]){
                    if(currentPartition[u] == 1)    sigma_0 += graph[v][u];
                    else if(currentPartition[u] == 0)   sigma_1 += graph[v][u];
                }
            }

            if(currentPartition[v] == 0 && sigma_1 > sigma_0){
                currentPartition[v] = 1;
                change = true;
            }
            else if(currentPartition[v] == 1 && sigma_0 > sigma_1){
                currentPartition[v] = 0;
                change = true;
            }
        }
    }

    k += i;
    cutValue += calculateCut(graph, currentPartition);

    // statsFile << "no. of iterations in local search : " << k << endl;
    // statsFile << "maxcut-value in this iteration : " << calculateCut(graph, currentPartition) << endl << endl;

    return currentPartition;
}

// GRASP with local search algorithm to find a Max-Cut solution
vector<int> grasp_max_cut(const vector<vector<int>>& graph, int max_iter_grasp) {
    vector<int> bestSolution;
    int bestCutValue = INT_MIN;

    for (int i = 0; i < max_iter_grasp; i++) {
        cout << "GRASP iteration no. : " << i << endl;

        // vector<int> currentSolution = randomized_construction(graph);
        // vector<int> currentSolution = greedy_construction(graph);
        vector<int> currentSolution = semi_greedy_construction(graph);

        vector<int> improvedSolution = local_search_max_cut(graph, currentSolution); // Use local search for improvement
        int currentCutValue = calculateCut(graph, improvedSolution);

        if (currentCutValue > bestCutValue) {
            bestCutValue = currentCutValue;
            bestSolution = improvedSolution;
        }
    }

    return bestSolution;
}

int main(int argc, char* argv[]) {    
    srand(static_cast<unsigned>(time(0)));  // Seed the random number generator

    int n, e;                               // number of vertices -> n, number of edges -> e
    inFile >> n >> e;

    int u, v, wt;                           // u -> starting vertex, v -> ending vertex, wt -> weight of the edge (u---v)
    vector<vector<int>> graph(n, vector<int>(n,0));
    for(int i = 0 ; i < e; i++){
        inFile >> u >> v >> wt;
        graph[u-1][v-1] = wt;
        graph[v-1][u-1] = wt;
    }
    
    int max_iter_grasp = 10;                // Number of GRASP iterations

    // Check if the argument are provided
    if (argc >= 2) {
        max_iter_grasp = std::atoi(argv[1]);    // overwrite default value
    }

    vector<int> maxCutPartition = grasp_max_cut(graph, max_iter_grasp);

    // Output the results
    statsFile << "semi_greedy_construction:\n";
    statsFile << "average no. of iterations of local search : " << (k * 1.0/max_iter_grasp) << ", average cut-value of max_cut_local_search() : " << (cutValue * 1.0/max_iter_grasp) << endl;
    statsFile << "no. of iterations of GRASP : " << max_iter_grasp << ", best cut-value of grasp_max_cut() : " << calculateCut(graph, maxCutPartition) << endl << endl;

    printResult(graph, maxCutPartition);

    return 0;
}

void printResult(const vector<vector<int>>& graph, const vector<int>& finalPartition){
    unordered_set<int> S0, S1;
    int n = graph.size();

    for(int i = 0; i < n; i++){
        if(finalPartition[i] == 0) S0.insert(i+1);
    }
    for(int i = 0; i < n; i++){
        if(finalPartition[i] == 1) S1.insert(i+1);
    }

    outFile << "semi_greedy_construction:\n";
    outFile << "Max Partition:\n";
    showElements(S0, outFile);
    outFile << endl;
    showElements(S1, outFile);
    outFile << endl;

    int maxCutValue = calculateCut(graph, finalPartition);
    outFile << "Max-Cut Value: " << maxCutValue << endl << endl;
}