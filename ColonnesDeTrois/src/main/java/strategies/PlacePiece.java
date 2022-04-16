package strategies;

import game.Square;
import utils.Utils.Color;

public class PlacePiece {
    private Square[][] table;
    private Color color;

    public PlacePiece (Color color, Square[][] table) {
        this.table = table;
        this.color = color;

    }
}
