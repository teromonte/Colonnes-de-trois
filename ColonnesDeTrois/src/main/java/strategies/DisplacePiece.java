package strategies;

import game.Square;
import utils.Utils.Color;

public class DisplacePiece {
    private Square[][] table;
    private Color color;

    public DisplacePiece (Color color, Square[][] table) {
        this.table = table;
        this.color = color;
    }
    
}
