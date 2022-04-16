package strategies;

import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils.Color;

public class PlacePiece {
    private Square[][] table;
    private Color color;

    public PlacePiece(Color color, Square[][] table) {
        this.table = table;
        this.color = color;

    }

    public Pair placePiece() {
        int x = -1, y = -1;
        // algo
        return new Pair(x, y);
    }
}
