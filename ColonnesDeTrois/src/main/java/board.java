public class board {

    public static final int length = 3;

    private static int[][] gameBoard = new int[length][length];

    private void setGameBoard(int[][] gameBoard) {
        this.gameBoard = gameBoard;
    }

    //initialise toutes les cases de board à zéro (vide)
    private void InitBoard(int[][] gameBoard){
        for(int y = 2; y >= 0; y--){
            for(int x = 0; x < length; x++){
                System.out.print(gameBoard[x][y] + "vide");
            }
        }
    }


    static int[][] getGameBoard() {
        return gameBoard;
    }

    private void printBoard(int[][] gameBoard){
        for(int y = 2; y >= 0; y--){
            System.out.println();
            for(int x = 0; x < length; x++){
                System.out.print(gameBoard[x][y] + " ");
            }
        }
        System.out.println();
    }

}
