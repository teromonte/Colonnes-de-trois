package strategies;

import java.util.List;

import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils.Color;

public class DisplacePiece {
    private Square[][] table;
    private Color color;
    private List<Move> validMoves;

    public DisplacePiece(Color color, Square[][] table) {
        this.table = table;
        this.color = color;
    }

    public Move displacePiece() {
        // algo
        Pair origin = new Pair(-1, -1);
        Pair destin = new Pair(-1, -1);
        return new Move(origin, destin);
    }
}
