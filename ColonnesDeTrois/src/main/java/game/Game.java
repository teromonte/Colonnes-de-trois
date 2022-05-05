package game;

import strategies.Algo;
import utils.Move;
import utils.Pair;
import utils.Utils;
import utils.Utils.Color;

public class Game {

    private Square[][] table;

    public Game() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
    }

    public void reset() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
    }

    public Move move(Color color) {
        Algo d = new Algo(color, table);
        Move m = d.getMove();
        // take cara if its a move or a place  - TODO
        table[m.getPiece().getX()][m.getPiece().getY()].removeTop();
        table[m.getMove().getX()][m.getMove().getY()].addPiece(color);
        return m;
    }
}
