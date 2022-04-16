public class square {

    private final board board;
    private final int row;
    private final int col;
    private onePion pion ;
    private  int nbPions ;

    public square(board board, int row, int col) {
        this.board = board;
        this.row = row;
        this.col = col;
    }

    public void setPion(onePion pion){
        this.pion = pion ;
    }
}
