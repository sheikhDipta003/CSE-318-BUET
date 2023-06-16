#include "min_priority_queue.cpp"

//returns true if the vector contains the element
bool contains(vector<Node> nodes, Node nd){
    for(int i = 0; i < nodes.size(); i++){
        if(nodes[i]==nd) return true;
    }
    return false;
}

//print the complete path
void printSolve(vector<Node> nodes){
    for(int i = 0; i < nodes.size(); i++) nodes[i].printBoard();
}

//returns true if the puzzle is solvable
bool isSolvable(Node init, int dim){
    int inv = init.count_inv();

    //if odd, true for even inversion-count
    if(dim % 2){
        if(inv % 2) return false;
        else return true;
    }
    //if even, ic + row-distance(blank) must be even
    //find row-distance of blank
    vector<vector<int>> u = init.getElem();
    int br;
    for(int i = 0; i < dim; i++)
        for(int j = 0; j < dim; j++)
            if(!u[i][j]){br = i; break;}

    //if parity same, then sum is even, return true
    return (br % 2 ) == (inv % 2);
}

//returns a vector of nodes from initial state to goal state, inclusive
void solve(Node init){
    //create an instance of priority queue and insert the initial node
    // vector<Node> pq;
    // pq.push_back(init);
    // buildHeap(pq);
    priority_queue<Node> pq;
    pq.push(init);

    //create a closed-list
    vector<Node> closed;

    //for printing number of 'explored nodes' and 'expanded nodes'
    int explored=0, expanded=0;

    Node _min, _prev=Node();  //to store node with min key, that will eventually hold the goal state

    while(true){
        //delete from the priority queue the search node with the minimum priority
        _min = pq.top(); pq.pop();
        // _min.setPrevNode(&_prev);
        //repeat the following until the search node dequeued corresponds to the final goal board
        if(_min.isEqualToGoal()) {
            _min.printBoard();
            _min.getPrevNode()->printBoard();
            break;
        }
        //insert the node into the closed list
        closed.push_back(_min);
        //insert onto the priority queue all neighboring search nodes (that can be reached by one move) which are not in the closed list
        vector<Node> nb = _min.getAllNext();
        for(int i = 0; i < nb.size(); i++) {
            if (!contains(closed, nb[i])){
                // insert(pq, nb.at(i));
                pq.push(nb.at(i));
                explored++;
            }
        }
        expanded++;
        // _prev = _min;
    }

    //print the number of 'explored nodes' and 'expanded nodes' as well as total number of moves needed to reach the goal state
    cout << "explored = " << explored << ", expanded = " << expanded << endl;
    cout << "minimum number of moves = " << _min.getMoves() << endl;


}

int main(){
    //testing when unsolvable
    // vector<vector<int>> v = {{7,2,4},{6,0,5},{8,3,1}};
    // Node bd(v);
    // cout << isSolvable(bd, 3) << endl;

    //testing when solvable and showing the complete path
    // int dim;
    // cin >> dim;
    // vector<vector<int>> v(dim*dim);
    // for(int i = 0; i < dim; i++){
    //     for(int j = 0; j < dim; j++){
    //         cin >> v[i][j];
    //     }
    // }
    vector<vector<int>> v = {{4, 3, 1},{5, 6, 8},{7, 2, 0}};
    Node bd(v, nullptr, 3, 0, 'm');
    if(isSolvable(bd, 3)) solve(bd);
    else cout << "Unsolvable\n";
}
