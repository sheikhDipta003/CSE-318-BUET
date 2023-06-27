import java.util.Scanner;

//max-player = '1'
//min-player = '0'
public class Board{
    int[] bins;
    Board(){
        bins = new int[14];
        for(int i = 0; i < bins.length; i++) {
            if (i != 6 && i != 13) bins[i] = 4;
            else bins[i] = 0;
        }
    }

    int heuristic(){     //evaluation function / heuristic to determine the next state for optimal gameplay
        return (bins[13]-bins[6]);  //'1' will try to maximize this heuristic, and '0' will try to minimize it
    }

    void printSpace(int k){     // print 'k' tabs in a single line
        for(int i = 0; i < k; i++) System.out.print("\t");
    }

    void showBoard(){
        System.out.print("--------------------------------------------------------------------------------\n");
        printSpace(1);
        for(int i = 12; i >= 7; i--){
            System.out.print("< " + i + " : " + bins[i] + " >");
            printSpace(1);
        }

        System.out.print("\n< 13 : " + bins[13] + " >");
        printSpace(15);
        System.out.print("< 6 : " + bins[6] + " >\n");

        printSpace(1);
        for(int i = 0; i <= 5; i++){
            System.out.print("< " + i + " : " + bins[i] + " >");
            printSpace(1);
        }
        System.out.print("\n--------------------------------------------------------------------------------\n");
    }

    //play the game
    void play(){
        showBoard();  //initially

        System.out.print("Select player-type {0, 1}: ");
        int playerType = new Scanner(System.in).nextInt();
        if(playerType==0)   System.out.print("<" + playerType + ">: Select bin no. [0,5]: ");
        else if(playerType==1)   System.out.print("<" + playerType + ">: Select bin no. [7,12]: ");
        int k = new Scanner(System.in).nextInt();
        boolean result = moveStones(playerType, k);
        showBoard();    //after 1st move

        while(true){
            if(isGameOver()){
                System.out.print(getWinner());
                break;
            }
            if(!result) playerType = 1 - playerType;
            if(playerType==0)   System.out.print("<" + playerType + ">: Select bin no. [0,5]: ");
            else if(playerType==1)   System.out.print("<" + playerType + ">: Select bin no. [7,12]: ");
            k = new Scanner(System.in).nextInt();
            result = moveStones(playerType, k);
            showBoard();    //after subsequent moves
        }
    }

    //moves the stones and returns whether this player wil get another turn or not
    boolean moveStones(int playerType, int k){   //playerType="0" or playerType="1"; k = [selected bin no.]
        while(!(playerType==0 || playerType==1)){
            System.out.print("Invalid player-type: enter either '0' or '1': ");
            playerType = new Scanner(System.in).nextInt();
        }
        if(playerType==0){
            while(!(k >= 0 && k <= 5)) {
                System.out.print("<0>: Invalid bin no., enter a value in range [0,5]: ");
                k = new Scanner(System.in).nextInt();
            }
            for(int i = k+1; bins[k] > 0; i++){
                if(i==13) continue;
                bins[k]--;
                if(i > 13) i=(i-1)%13;
                bins[i] += 1;

                if(bins[k]==0){         //for the last stone that just went into i-th bin,
                    if(i < 6 && bins[i]==1 && bins[12-i] > 0){     //if i-th bin was empty before placing this stone in it
                        bins[6] += (bins[i]+bins[12-i]);  //then move opponent's stones from the opposite bin to my store
                        bins[12-i]=0;
                        bins[i]=0;
                    }
                    else if(i==6){    //otherwise, if this last stone went into my store, I get a turn again
                        return true;
                    }
                }
            }
        }
        else{
            while(!(k >= 7 && k <= 12)) {
                System.out.print("<1>: Invalid bin no., enter a value in range [7,12]: ");
                k = new Scanner(System.in).nextInt();
            }
            for(int i = k+1; bins[k] > 0; i++){
                if(i==6) continue;
                bins[k]--;
                if(i > 13) i=(i-1)%13;
                bins[i] += 1;

                if(bins[k]==0){         //for the last stone that just went into i-th bin,
                    if(i >= 7 && i < 13 && bins[i] == 1 && bins[12-i] > 0){     //if i-th bin was empty before placing this stone in it
                        bins[13] += (bins[i]+bins[12-i]);  //then move opponent's stones from the opposite bin to my store
                        bins[12-i]=0;
                        bins[i]=0;
                    }
                    else if(i==13){     //otherwise, if this last stone went into my store, I get a turn again
                        return true;
                    }
                }
            }
        }

        return false;
    }

    boolean isGameOver(){
        int emptyMin=0, emptyMax=0;
        for(int i = 0; i <= 5; i++){
            if(bins[i]==0) emptyMin++;
        }
        for(int i = 7; i <= 12; i++){
            if(bins[i]==0) emptyMax++;
        }
        return (emptyMin==6 || emptyMax==6);
    }

    String getWinner(){
        String result = "winner = ";
        if(bins[6] > bins[13]) return result+"0\n";
        else if(bins[13] > bins[6]) return result+"1\n";
        return "draw\n";
    }
}
