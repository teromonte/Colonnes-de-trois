package strategies;

import java.util.List;

import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils;
import utils.Utils.Color;

public class Algo {
    private Square[][] table;
    private Color color;
    private List<Move> validMoves;

    public Algo(Color color, Square[][] table) {
        this.table = table;
        this.color = color;
    }

    private List<Move> calcValisMoves() {
        List<Move> res;
        if(Utils.countPieces(table) <= 16) {
            
        } else {

        }
        return res;
    }

    public Move getMove() {
        // algo
        Pair origin = new Pair(-1, -1);
        Pair destin = new Pair(-1, -1);
        return new Move(origin, destin);
    }
}
