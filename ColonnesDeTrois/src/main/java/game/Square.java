public class Square {
    private final int row;
    private final int col;
    private Piece pion ;
    private  int nbPions ;

    public Square(Game board, int row, int col) {
        this.board = board;
        this.row = row;
        this.col = col;
    }

    public void setPion(Piece pion){
        this.pion = pion ;
    }
}
