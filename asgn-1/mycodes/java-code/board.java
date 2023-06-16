import java.util.ArrayList;
import java.lang.Math;

class Node implements Comparable<Node>{
    ArrayList<ArrayList<Integer>> board;
    int dim;
    int moves;
    Node prev;
    int key;
    char h_type;
    Node(ArrayList<ArrayList<Integer>> list, Node p, int d, int m, char ch){
        this.board = list;
        prev = p;
        dim = d;
        moves = m;
        h_type = ch;
        setKey(h_type);
    }

    void printBoard(){
        for(int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board.size(); j++) {
                System.out.print(board.get(i).get(j) + " ");
            }
            System.out.println();
        }
    }

    void printList(ArrayList<Integer> list){
        System.out.print("[");
        for (Integer integer : list) {
            System.out.print(" " + integer);
        }
        System.out.print("]\n");
    }

    int heuristic_hamming(){
        ArrayList<Integer> temp = new ArrayList<>();
        for (ArrayList<Integer> row : board) {
            temp.addAll(row);
        }
        int dist = 0;
        for(int i = 0; i < temp.size(); i++){
            if(temp.get(i) > 0){
                if(i != temp.get(i)-1) dist++;
            }
        }
        return dist;
    }

    int heuristic_manhattan(){
        int dist = 0;
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                if(board.get(i).get(j) > 0){
                    dist += Math.abs(i - (board.get(i).get(j)-1)/dim);
                    dist += Math.abs(j - (board.get(i).get(j)-1)%dim);
                }
            }
        }
        return dist;
    }

    void setKey(char ch){
        key = moves;
        if(ch=='m') key += heuristic_manhattan();
        else if(ch=='h') key += heuristic_hamming();
    }

    void swap(ArrayList<ArrayList<Integer>> u, int row1, int col1, int row2, int col2) {
        Integer temp = u.get(row1).get(col1);
        u.get(row1).set(col1, u.get(row2).get(col2));
        u.get(row2).set(col2, temp);
    }

    ArrayList<Node> getAllNext(){
        ArrayList<Node> neighbors = new ArrayList<>();

        int br=0, bc=0;
        for(int i = 0; i < dim; i++)
            for(int j = 0; j < dim; j++)
                if(board.get(i).get(j)==0){br = i; bc = j; break;}

        //determine next states and add them in 'neighbors' vector
        if(bc-1 >= 0){
            ArrayList<ArrayList<Integer>> u = new ArrayList<>();
            for (ArrayList<Integer> row : board) {
                ArrayList<Integer> newRow = new ArrayList<>(row);
                u.add(newRow);
            }
            swap(u, br,bc,br,bc-1);
            neighbors.add(new Node(u, this, dim, moves + 1, h_type));
        }
        if(bc+1 <= dim-1){
            ArrayList<ArrayList<Integer>> u = new ArrayList<>();
            for (ArrayList<Integer> row : board) {
                ArrayList<Integer> newRow = new ArrayList<>(row);
                u.add(newRow);
            }
            swap(u, br,bc,br,bc+1);
            neighbors.add(new Node(u, this, dim, moves + 1, h_type));
        }
        if(br-1 >= 0){
            ArrayList<ArrayList<Integer>> u = new ArrayList<>();
            for (ArrayList<Integer> row : board) {
                ArrayList<Integer> newRow = new ArrayList<>(row);
                u.add(newRow);
            }
            swap(u, br,bc,br-1,bc);
            neighbors.add(new Node(u, this, dim, moves + 1, h_type));
        }
        if(br+1 <= dim-1){
            ArrayList<ArrayList<Integer>> u = new ArrayList<>();
            for (ArrayList<Integer> row : board) {
                ArrayList<Integer> newRow = new ArrayList<>(row);
                u.add(newRow);
            }
            swap(u, br,bc,br+1,bc);
            neighbors.add(new Node(u, this, dim, moves + 1, h_type));
        }

        return neighbors;
    }

    int count_inv(){
        int count = 0; //to store inversion count
        ArrayList<Integer> u = new ArrayList<>();
        for (ArrayList<Integer> row : board) {
            u.addAll(row);
        }
//        printList(u);

        for (int i1 = 0; i1 < u.size() - 1; i1++) //for each element
            for (int i2 = i1 + 1; i2 < u.size(); i2++){ // to compare element right to current item
                if ((u.get(i1)>0) && (u.get(i2)>0) && (u.get(i1) > u.get(i2))) count++;
            }

        return count;
    }

    //returns true if the board has reached the goal state
    boolean isEqualToGoal(){
        ArrayList<Integer> nodes = new ArrayList<>();
        for (ArrayList<Integer> row : board) {
            nodes.addAll(row);
        }

        int N = nodes.size();

        for(int i = 0; i < N; i++){
            if(nodes.get(i) > 0) {
                if(nodes.get(i)-1!=i) return false;
            }
            else{
                if(nodes.get(N-1)!=0) return false;
            }
        }
        return true;
    }

    @Override
    public int compareTo(Node other) {
        // Compare based on key (lower key has higher priority)
        return Integer.compare(this.key, other.key);
    }
}

public class board {
    public static void main(String[] args) {
        //testing
//        int dim;
//        ArrayList<ArrayList<Integer>> arr = new ArrayList<>();
//        Scanner scanner = new Scanner(System.in);
//        dim = scanner.nextInt();
//        for (int i = 0; i < dim; i++) {
//            ArrayList<Integer> row = new ArrayList<>();
//            for (int j = 0; j < dim; j++) {
//                int element = scanner.nextInt();
//                row.add(element);
//            }
//            arr.add(row);
//        }
//        Node nd = new Node(arr,null,dim,0,'m');
//        System.out.println(nd.heuristic_manhattan());
//        System.out.println(nd.key);
//
//        ArrayList<Node> N = nd.getAllNext();
//        for (Node node : N) {
//            node.printBoard();
//            System.out.println("--------------------\n");
//        }
//
//        System.out.println(nd.count_inv());
//        System.out.println(nd.isEqualToGoal());
    }
}
