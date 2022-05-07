package strategies;

import java.util.List;

import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils;

public class Algo {
    private Square[][] table;
    private int color;
    private List<Move> validMoves;

    public Algo(int color, Square[][] table) {
        this.color = color;
        this.table = table;
    }

    private List<Move> calcValisMoves() {
        List<Move> res;
        if(Utils.countPieces(table) <= 16) {
            
        } else {

        }
        return null;
    }

    public Move getMove() {
        // algo
        Pair origin = new Pair(-1, -1);
        Pair destin = new Pair(-1, -1);
        return new Move(origin, destin);
    }
}
