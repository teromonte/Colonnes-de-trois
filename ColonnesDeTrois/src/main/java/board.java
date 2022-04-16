public class board {

    public static final int length = 3;

    private final square[][] squares; // i added final
    private final pions whiteArmy;//
    private final pions blackArmy;//


    private static final int[][] gameBoard = new int[length][length];

    public board(pions whiteArmy, pions blackArmy)
    {
        this.whiteArmy = whiteArmy;
        this.blackArmy = blackArmy;
        squares = new square[length][length];

        for (int r = 0; r < length; r++)
        {
            for (int c = 0; c < length; c++)
            {
                squares[r][c] = new square(this, r, c);
            }
        }
    }

    public pions getArmy(couleur couleur)
    {
        return (couleur.equals(couleur.White) ) ? whiteArmy : blackArmy ;
    }

    public square getSquare(int row, int col)
    {
        return (row < 0 || row >= length || col < 0 || col >= length) ? null : squares[row][col];
    }

    static int[][] getGameBoard() {
        return gameBoard;
    }

    /*private void printBoard(int[][] gameBoard){
        for(int y = 2; y >= 0; y--){
            System.out.println();
            for(int x = 0; x < length; x++){
                System.out.print(gameBoard[x][y] + " ");
            }
        }
        System.out.println();
    }*/

}
