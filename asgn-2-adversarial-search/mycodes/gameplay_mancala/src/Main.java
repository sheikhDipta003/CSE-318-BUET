import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
//        Board bd = new Board();
//        bd.play();

        //Min-max algorithm without alpha-beta pruning
        Board current= new Board();
        int playerType=0;

        while(true){
            current.showBoard();
            System.out.println();
            current.printSpace(9);
            System.out.print("player - " + playerType + "\n");

            if(current.isGameOver()) break;

            //generate all nodes derivable from 'current' board
            Board[] arr = new Board[6];     //at most 6 children are possible from this board
            boolean[] b = new boolean[6];              //is it the same player's turn?
            for (int i = 0; i < arr.length; i++) {
                arr[i] = new Board();
                //copy the state of this board to all the children
                System.arraycopy(current.bins, 0, arr[i].bins, 0, 14);
                //move the stones in i-th bin
                if(playerType==0) b[i] = arr[i].moveStones(playerType, i);
                else if(playerType==1) b[i] = arr[i].moveStones(playerType, i+7);
            }

            //select the node with minimum/maximum heuristic-value
            int k = 0;

            if (playerType == 0) {
                for (int i = 1; i < arr.length ; i++) {
                    if (arr[i].heuristic() < arr[k].heuristic()) {
                        k = i;
                    }
                }
            } else if (playerType == 1) {
                for (int i = 0; i < arr.length ; i++) {
                    if (arr[i].heuristic() > arr[k].heuristic()) {
                        k = i;
                    }
                }
            }

            //update 'current' and swap 'playerType' if necessary
            current = arr[k];
            if(!b[k]) playerType = 1 - playerType;
        }

        System.out.println(current.getWinner());
    }
}