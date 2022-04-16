package game;

import java.util.ArrayList;
import java.util.List;

import strategies.DisplacePiece;
import strategies.PlacePiece;
import utils.Utils;
import utils.Utils.Color;

public class Game {

    private Square[][] table;

    public Game() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
    }

    public int place(Color color, Square[][] table) {
        PlacePiece d = new PlacePiece(color, table);
        return -1;
    }

    public int move(Color color, Square[][] table) {
        DisplacePiece d = new DisplacePiece(color, table);
        return -1;
    }
}
