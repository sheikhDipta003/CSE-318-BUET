#include "board.cpp"

// min-heap
// Function to swap position of two elements
void swap(Node *a, Node *b) {
  Node temp = *b;
  *b = *a;
  *a = temp;
}

// Function to heapify the tree
void heapify(vector<Node> &hT, int i) {
  int size = hT.size();
  
  // Find the largest among root, left child and right child
  int smallest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;
  if (l < size && hT[l].getKey() < hT[smallest].getKey())
    smallest = l;
  if (r < size && hT[r].getKey() < hT[smallest].getKey())
    smallest = r;

  // Swap and continue heapifying if root is not smallest
  if (smallest != i) {
    swap(&hT[i], &hT[smallest]);
    heapify(hT, smallest);
  }
}

// Function to build a Min-Heap from the given array
void buildHeap(vector<Node>& ht)
{
    // Index of last non-leaf node
    int N = ht.size();
  
    // Perform reverse level order traversal
    // from last non-leaf node and heapify
    // each node
    for (int i = N/2 - 1; i >= 0; i--) {
        heapify(ht, i);
    }
}

// Function to insert an element into the tree
void insert(vector<Node> &hT, Node newNode) {
  hT.push_back(newNode);
  buildHeap(hT);
}

// Function to delete an element from the tree
void remove(vector<Node> &hT, int key) {
  int N = hT.size();
  int i;
  for (i = 0; i < N; i++) {
    if (key == hT[i].getKey())
      break;
  }
  if(i >= N) return;
  swap(&hT[i], &hT[N - 1]);

  hT.pop_back();
  for (int i = N / 2 - 1; i >= 0; i--) {
    heapify(hT, i);
  }
}

int height(vector<Node> &ht) {  //returns height of the priority queue (in number of nodes)
    if(ht.size() <= 1) return ht.size();
    int k = 0;
    for(int i = 0; i < ht.size(); ){
        k++;
        i = 2 * i + 1;
    }
    return k;
}

//removes and returns the minimum element
Node getMin(vector<Node> &ht){
    Node result = ht[0];
    remove(ht, result.getKey());
    return result;
}

// Print the tree
void printTree(vector<Node> &hT) {
  for (int i = 0; i < hT.size(); ++i){
    hT[i].printBoard();
    cout << "---------------------\n";
  }
}

// Driver code
// int main() {
//     vector<Node> heapTree;
//     vector<vector<int>> v = {{7,2,4},{6,0,5},{8,3,1}};
//     vector<vector<int>> v1 = {{7,0,4},{6,2,5},{8,3,1}};
//     vector<vector<int>> v2 = {{7,2,4},{0,6,5},{8,3,1}};
//     vector<vector<int>> v3 = {{7,2,4},{6,5,0},{8,3,1}};
//     vector<vector<int>> v4 = {{7,2,4},{6,3,5},{8,0,1}};
//     heapTree.push_back(Node(v));
//     heapTree.push_back(Node(v1));
//     heapTree.push_back(Node(v2));
//     heapTree.push_back(Node(v3));
//     heapTree.push_back(Node(v4));
//     buildHeap(heapTree);
//     printTree(heapTree);

//     // vector<vector<int>> v = {{7,2,4},{6,0,5},{8,3,1}};
//     // Node bd(v);
//     // cout << bd.heuristic_manhattan();
// }
