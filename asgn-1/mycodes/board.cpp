#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

//returns absolute value of an integer
int abs_(int x){return (x>0)?x:(-x);}

//swap two elements in the board
void swap(vector<vector<int>> &u, int i1, int j1, int i2, int j2){
    int t = u.at(i1).at(j1);
    u.at(i1).at(j1) = u.at(i2).at(j2);
    u.at(i2).at(j2) = t;
}

//represents a search node
class Node{
    vector<vector<int>> v;  //contains the elements of the current board in row-major order
    int dim;    //dimension of the board
    int moves;      //number of moves to reach in this state
    Node* prev;     //points to the previous search node
    int key;    //the key with respect to which the nodes will be inserted in the pq
public:
    Node(vector<vector<int>> v1, Node* p, int d=3, int m=0, char ch='m'){
        v = v1; dim = d; moves = m; prev = p; this->setKey(ch);
    }
    Node(){}
    int getDim(){return this->dim;}
    vector<vector<int>> getElem(){return this->v;}
    int getMoves(){return this->moves;}
    Node* getPrevNode(){return this->prev;}
    void setDim(int D){this->dim = D;}
    void setElem(vector<vector<int>> v1){v = v1;}
    void setMoves(int m){moves = m;}
    void setPrevNode(Node* p){this->prev = p;}

    bool operator == (const Node &nd){  //true if this vector equals 'nd'
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                if(this->v[i][j]!=nd.v[i][j]) return false;
            }
        }
        return true;
    }

    // Define the comparison operator for the class
    bool operator<(const Node& other) const {
        return key > other.key;
    }

    void operator = (const Node &nd ) { 
        this->v = nd.v;
        this->dim = nd.dim;
        this->moves = nd.moves;
        this->prev = nd.prev;
        this->key = nd.key;
    }

    void printBoard(){     //print elements of the board
        for(int i=0;i<v.size();i++){
            for(int j=0;j<v[i].size();j++)  cout<<v[i][j]<<" ";
            cout << endl;
	    }
        cout << endl;
    }
    void printVector(vector<int> v){     //print elements of a 1d vector
        cout << "[";
        for(int i = 0; i < v.size(); i++){
            cout << " " << v.at(i);
        }
        cout << " ]\n";
    }
    //convert 2d board into 1d vector
    void toList(vector<int> &t){
        for(int i = 0; i < v.size(); i++){
            for(int j = 0; j < v[0].size(); j++){
                t.push_back(v[i][j]);
            }
        }
        // printVector(t);   //cross-checking
    }
    int heuristic_hamming(){
        vector<int> temp;
        this->toList(temp);

        //find the hamming distance of the board
        int dist = 0;
        for(int i = 0; i < temp.size(); i++){
            if(temp[i]){
                if(i != temp[i]-1) dist++;
            }
        }
        return dist;
    }
    int heuristic_manhattan(){
        //find the manhattan distance of the board
        int dist = 0;
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                if(v[i][j]){
                    dist += abs_(i - (v[i][j]-1)/dim);   //horizontal distance
                    dist += abs_(j - (v[i][j]-1)%dim);   //vertical distance
                }
            }
        }
        return dist;
    }

    void setKey(char ch){
        // key, f(n) = g(n) [#moves to reach the board] + h(n) [estimated #moves to reach goal state]
        key = moves;
        if(ch == 'h')   key += heuristic_hamming();
        else if(ch == 'm')  key += heuristic_manhattan();
    }

    int getKey(){return this->key;}

    //returns all possible next states
    vector<Node> getAllNext(){
        vector<Node> neighbors;
        
        //find index of blank
        int br, bc;
        for(int i = 0; i < dim; i++)
            for(int j = 0; j < dim; j++)
                if(!v[i][j]){br = i; bc = j; break;}

        //determine next states and add them in 'neighbors' vector
        if(bc-1 >= 0){
            vector<vector<int>> u = v;
            swap(u,br,bc,br,bc-1);
            neighbors.push_back(Node(u, this, dim, moves + 1));
        }
        if(bc+1 <= dim-1){
            vector<vector<int>> u = v;
            swap(u,br,bc,br,bc+1);
            neighbors.push_back(Node(u, this, dim, moves + 1));
        }
        if(br-1 >= 0){
            vector<vector<int>> u = v;
            swap(u,br,bc,br-1,bc);
            neighbors.push_back(Node(u, this, dim, moves + 1));
        }
        if(br+1 <= dim-1){
            vector<vector<int>> u = v;
            swap(u,br,bc,br+1,bc);
            neighbors.push_back(Node(u, this, dim, moves + 1));
        }

        return neighbors;
    }

    //return number of inversions of this board
    int count_inv(){
        int count = 0; //to store inversion count
        vector<int> u;
        this->toList(u);

        for (int i1 = 0; i1 < u.size() - 1; i1++) //for each element
        for (int i2 = i1 + 1; i2 < u.size(); i2++){ // to compare element right to current item
            if (u[i1] && u[i2] && (u[i1] > u[i2])) count++;
        }
        
        return count;
    }

    //returns true if the board has reached the goal state
    bool isEqualToGoal(){
        vector<int> nodes;
        this->toList(nodes);
        
        int N = nodes.size();
        
        for(int i = 0; i < N; i++){
            if(nodes[i] > 0) {
                if(nodes[i]-1!=i) return false;
            }
            else{
                if(nodes[N-1]!=0) return false;
            }
        }
        return true;
    }
};

// int main(){
//     vector<vector<int>> v = {{0,1,3},{4,2,5},{7,8,6}};
//     Node bd(v);
//     cout << bd.heuristic_manhattan() << " " << bd.getKey() << endl;

    // vector<Node> N = bd.getAllNext();
    // for(int i = 0; i < N.size(); i++){
    //     N[i].printBoard();
    //     cout << "--------------------\n";
    // }

    // cout << bd.count_inv() << endl;
    // cout << bd.isEqualToGoal() << endl;

//     return 0;
// }



