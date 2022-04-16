package game;

import java.util.ArrayList;
import java.util.List;

import strategies.DisplacePiece;
import strategies.PlacePiece;
import utils.Move;
import utils.Pair;
import utils.Utils;
import utils.Utils.Color;

public class Game {

    private Square[][] table;


    public Game() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
    }

    public Pair place(Color color) {
        PlacePiece d = new PlacePiece(color, table);
        Pair p = d.placePiece();
        table[p.getX()][p.getY()].addPiece(color);
        return p;
    }

    public Move move(Color color) {
        DisplacePiece d = new DisplacePiece(color, table);
        Move m = d.displacePiece();
        table[m.getPiece().getX()][m.getPiece().getY()].removeTop();
        table[m.getMove().getX()][m.getMove().getY()].addPiece(color);
        return m;
    }
}
