import java.util.Random;
import java.util.Scanner;

public class mancala_play {
    static final int MANCALA_0 = 6;
    static final int MANCALA_1 = 13;
    static final int MAX_DEPTH = 6;

    boolean exitGame = false;
    boolean AIvsAI = false;
    int[] board;
    int currentPlayer;
    int additional_moves;
    int stones_captured;

    mancala_play() {
        board = new int[14];
        //randomly assign currentPlayer
        Random random = new Random();
        currentPlayer = random.nextInt(2);
        for (int i = 0; i < 14; i++) {
            if (i != MANCALA_0 && i != MANCALA_1) {
                board[i] = 4;
            }
        }
        additional_moves = 0;
        stones_captured = 0;
    }

    void play() {
        while (true) {
            showBoard();

            if(isGameOver() || exitGame){
                //show final scores and the winner
                if(AIvsAI) {
                    System.out.println("Scores:\n\tplayer-0 : " + board[MANCALA_0] + "\n\tplayer-1 : " + board[MANCALA_1]);
                    if (board[MANCALA_0] > board[MANCALA_1]) {
                        System.out.println("Winner: player-0");
                    } else if (board[MANCALA_1] > board[MANCALA_0]) {
                        System.out.println("Winner: player-1");
                    } else {
                        System.out.println("Draw");
                    }
                }
                else{
                    System.out.println("Scores:\n\tYou : " + board[MANCALA_0] + "\n\tAI : " + board[MANCALA_1]);
                    if (board[MANCALA_0] > board[MANCALA_1]) {
                        System.out.println("Winner: You");
                    } else if (board[MANCALA_1] > board[MANCALA_0]) {
                        System.out.println("Winner: AI");
                    } else {
                        System.out.println("Draw");
                    }
                }
                break;
            }

            //human vs AI
            if (currentPlayer == 0) {
                System.out.println("Your turn");
                System.out.print("Enter bin no. (1-6) [Enter 0 to exit game]: ");
                int bin = new Scanner(System.in).nextInt();

                if(bin == 0) exitGame = true;

                if (isValidMove(bin-1)) {
                    moveStones(bin-1);
                } else {
                    if(bin != 0)    System.out.println("Invalid bin no.");
                    //show error message only if the human player doesn't exit the game
                }
            } else {
                System.out.println("AI's turn");
                int i = getOptimalMove();
                moveStones(i);
                System.out.println("Selected bin - " + (i+1));
            }

            //AI vs AI
//            System.out.println("AI-" + currentPlayer + "'s turn");
//            int i = getOptimalMove();
//            moveStones(i);
//            System.out.println("Selected bin - " + (i+1));
        }
    }

    boolean isValidMove(int bin) {
        if (currentPlayer == 0 && bin >= 0 && bin <= 5 && board[bin] != 0) {
            return true;
        } else if (currentPlayer == 1 && bin >= 7 && bin <= 12 && board[bin] != 0) {
            return true;
        }
        return false;
    }

    void printSpace(int k){     // print 'k' tabs in a single line
        for(int i = 0; i < k; i++) System.out.print("\t");
    }
    void showBoard() {
        System.out.print("\n--------------------------------------------------------------------------------\n");
        printSpace(1);
        for(int i = 12; i >= 7; i--){
            System.out.print("< " + (i+1) + " : " + board[i] + " >");
            printSpace(1);
        }

        System.out.print("\n< 14 : " + board[13] + " >");
        printSpace(15);
        System.out.print("< 7 : " + board[6] + " >\n");

        printSpace(1);
        for(int i = 0; i <= 5; i++){
            System.out.print("< " + (i+1) + " : " + board[i] + " >");
            printSpace(1);
        }
        System.out.print("\n--------------------------------------------------------------------------------\n");
    }

    void moveStones(int bin) {
        int stones = board[bin];
        board[bin] = 0;

        while (stones > 0) {
            bin = (bin + 1) % 14;
            if ((currentPlayer == 0 && bin == MANCALA_1) || (currentPlayer == 1 && bin == MANCALA_0)) {
                continue;
            }
            board[bin]++;
            stones--;
        }

        if (bin != MANCALA_0 && bin != MANCALA_1 && board[bin] == 1 &&
                ((currentPlayer == 0 && bin >= 0 && bin <= 5 && board[12 - bin] > 0) ||
                        (currentPlayer == 1 && bin >= 7 && bin <= 12 && board[12 - bin] > 0))) {
            int to_capture = board[12 - bin] + board[bin];
            board[12 - bin] = 0;
            board[bin] = 0;
            if (currentPlayer == 0) {
                board[MANCALA_0] += to_capture;
            } else {
                board[MANCALA_1] += to_capture;
            }
            stones_captured += to_capture;
        }

        if(!(bin == MANCALA_0 || bin == MANCALA_1)) {
            currentPlayer = 1 - currentPlayer;
        }
        else additional_moves++;
    }

    boolean isGameOver() {
        int emptyMin=0, emptyMax=0;
        for(int i = 0; i <= 5; i++){
            if(board[i]==0) emptyMin++;
        }
        for(int i = 7; i <= 12; i++){
            if(board[i]==0) emptyMax++;
        }

        return (emptyMin==6 || emptyMax==6);
    }

    int getOptimalMove() {
        int bestMove = -1;
        int bestScore;

        if(currentPlayer==0){
            bestScore = Integer.MAX_VALUE;
            for (int bin = 0; bin <= 5; bin++) {
                if (board[bin] == 0) {
                    continue;
                }

                //save current board state and current player
                int[] temp = board.clone();
                int tempPlayer = currentPlayer;

                moveStones(bin);

                int score = minimax(0, Integer.MIN_VALUE, Integer.MAX_VALUE, true);

                //update if lower score
                if (score < bestScore) {
                    bestScore = score;
                    bestMove = bin;
                }

                //restore values after branch traversal
                board = temp;
                currentPlayer = tempPlayer;
            }
        }
        else {
            bestScore = Integer.MIN_VALUE;
            for (int bin = 7; bin <= 12; bin++) {
                if (board[bin] == 0) {
                    continue;
                }

                //save current board state and current player
                int[] temp = board.clone();
                int tempPlayer = currentPlayer;

                moveStones(bin);

                int score = minimax(0, Integer.MIN_VALUE, Integer.MAX_VALUE, false);

                //update if higher score
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = bin;
                }

                //restore values after branch traversal
                board = temp;
                currentPlayer = tempPlayer;
            }
        }

        return bestMove;
    }

    int minimax(int depth, int alpha, int beta, boolean isMaxPlayer) {
        if (depth >= MAX_DEPTH || isGameOver()) {
            return heuristic();
        }

        if (isMaxPlayer) {
            int maxVal = Integer.MIN_VALUE;

            for (int bin = 7; bin <= 12; bin++) {
                if (board[bin] == 0) {
                    continue;
                }

                //save current board state and current player
                int[] temp = board.clone();
                int tempPlayer = currentPlayer;

                moveStones(bin);

                int evalScore = minimax(depth + 1, alpha, beta, false);
                maxVal = Math.max(maxVal, evalScore);
                alpha = Math.max(alpha, evalScore);

                //restore values after branch traversal
                board = temp;
                currentPlayer = tempPlayer;

                if (alpha >= beta) break;
            }

            return maxVal;
        } else {
            int minVal = Integer.MAX_VALUE;

            for (int bin = 0; bin <= 5; bin++) {
                if (board[bin] == 0) {
                    continue;
                }

                //save current board state and current player
                int[] temp = board.clone();
                int tempPlayer = currentPlayer;

                moveStones(bin);

                int evalScore = minimax(depth + 1, alpha, beta, true);
                minVal = Math.min(minVal, evalScore);
                beta = Math.min(beta, evalScore);

                //restore values after branch traversal
                board = temp;
                currentPlayer = tempPlayer;

                if (alpha >= beta) break;
            }

            return minVal;
        }
    }

    int heuristic() {
        return board[MANCALA_1] - board[MANCALA_0];

        //W1 * (stones_in_my_store – stones_in_opponents_store) + W2 * (stones_on_my_bins – stones_on_opponents_bins)
//        int x=0, y=0;
//        for(int i = 0; i < 6; i++) x += board[i];    //total #stones on 1's bins
//        for(int i = 7; i < 12; i++) y += board[i];   //total #stones on 2's bins
//        return 25 * (board[13]-board[6]) +  2 * (y-x);

        //W1 * (stones_in_my_storage – stones_in_opponents_storage) + W2 * (stones_on_my_side –
        // stones_on_opponents_side) + W3 * (additional_move_earned)
//        return 25 * (board[13]-board[6]) +  2 * (y-x) + 10 * additional_moves;

        //W1 * (stones_in_my_storage – stones_in_opponents_storage) + W2 * (stones_on_my_side –
        //stones_on_opponents_side) + W3 * (additional_move_earned) + W4 * (stones_captured)
//        return 25 * (board[13]-board[6]) +  10 * (y-x) + 6 * additional_moves + 2 * stones_captured;
    }

    public static void main(String[] args) {
        mancala_play game = new mancala_play();
        game.play();
    }
}
