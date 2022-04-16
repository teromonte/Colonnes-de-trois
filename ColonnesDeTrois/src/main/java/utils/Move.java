package utils;

public class Move {
    private Pair piece;
    private Pair move;

    public Move(Pair piece, Pair move) {
        this.piece = piece;
        this.move = move;
    }

    public Pair getPiece() {
        return piece; 
    }

    public Pair getMove() {
        return move; 
    }
}
