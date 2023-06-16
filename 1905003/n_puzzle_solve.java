import java.util.ArrayList;
import java.util.Collections;
import java.util.PriorityQueue;
import java.util.Scanner;

class Solve {
    //returns true if the puzzle is solvable
    boolean isSolvable(Node init, int dim){
        int inv = init.count_inv();

        //if odd, true for even inversion-count
        if(dim % 2==1){
            if(inv % 2==1) return false;
            else return true;
        }
        //if even, ic + row-distance(blank) must be even
        //find row-distance of blank
        ArrayList<ArrayList<Integer>> u = init.board;
        int br=0;
        for(int i = 0; i < dim; i++)
            for(int j = 0; j < dim; j++)
                if(u.get(i).get(j)==0){br = i; break;}

        //if parity same, then sum is even, return true
        return (br % 2) == (inv % 2);
    }

    //returns a vector of nodes from initial state to goal state, inclusive
    void solve(Node init){
        //create an instance of priority queue and insert the initial node
        PriorityQueue<Node> pq = new PriorityQueue<>();
        pq.offer(init);

        //create a closed-list
        ArrayList<Node> closed = new ArrayList<>();

        //for printing number of 'explored nodes' and 'expanded nodes'
        int explored=0, expanded=0;

        Node _min;  //to store node with min key, that will eventually hold the goal state

        while(true) {
            //delete from the priority queue the search node with the minimum priority
            _min = pq.poll();
            //repeat the following until the search node dequeued corresponds to the final goal board
            assert _min != null;
            if (_min.isEqualToGoal()) break;
            //insert the node into the closed list
            closed.add(_min);
            //insert onto the priority queue all neighboring search nodes (that can be reached by one move) which are not in the closed list
            ArrayList<Node> nb = _min.getAllNext();
            for (Node node : nb) {
                if (!closed.contains(node)) {
                    pq.offer(node);
                    explored++;
                }
            }
            expanded++;
        }

        //print the complete path with #explored and #expanded nodes
        printSolve(_min, explored, expanded);
    }

    void printSolve(Node nd, int explored, int expanded){
        //print the number of 'explored nodes' and 'expanded nodes' as well as total number of moves needed to reach the goal state
        System.out.print("\nminimum number of moves = " + nd.moves + "\n");
        System.out.print("explored = " + explored + ", expanded = " + expanded + "\n");

        ArrayList<Node> nodes = new ArrayList<>();
        while(nd != null){
            nodes.add(nd);
            nd = nd.prev;
        }
        Collections.reverse(nodes);
        for(Node node: nodes){
            node.printBoard();
            System.out.println();
        }
    }
}

public class n_puzzle_solve {
    public static void main(String[] args) {
        int dim;
        ArrayList<ArrayList<Integer>> arr = new ArrayList<>();
        Scanner scanner = new Scanner(System.in);
        dim = Integer.parseInt(scanner.nextLine());
        for (int i = 0; i < dim; i++) {
            ArrayList<Integer> row = new ArrayList<>();
            for (int j = 0; j < dim; j++) {
                int element = scanner.nextInt();
                row.add(element);
            }
            arr.add(row);
        }
        Node nd = new Node(arr,null,dim,0,'m');
        Solve slv = new Solve();
        if(slv.isSolvable(nd,dim)) slv.solve(nd);
        else System.out.println("Unsolvable");
    }
}
