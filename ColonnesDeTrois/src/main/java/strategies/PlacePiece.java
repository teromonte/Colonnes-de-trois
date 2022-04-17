package strategies;

import java.util.List;

import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils.Color;

public class PlacePiece {
    private Square[][] table;
    private Color color;
    private List<Pair> validPos;

    public PlacePiece(Color color, Square[][] table) {
        this.table = table;
        this.color = color;
        this.validPos = calcValisPos();

    }

    public List<Pair> calcValisPos() {
        return null;
    }

    public Pair placePiece() {
        // algo
        int x = -1, y = -1;
        return new Pair(x, y);
    }
}
