package game;

import java.util.ArrayList;
import java.util.List;
import utils.*;

public class Square {
    private List<Piece> pieces;
    private int size;
    int thiago;

    public Square() {
        pieces = new ArrayList<Piece>(Utils.MAX_PILE_SIZE);
        this.size = 0;
    }

    public int getSize() {
        return pieces.size();
    }

    public void addPiece(int color) {
        if (size < 3) {
            if (size == 0) {
                pieces.add(new Piece(color));
                size++;
            } else {
                for (Piece piece : pieces) {
                    piece.swichState();
                }
                pieces.add(new Piece(color));
                size++;
            }
        }
    }

    public Piece removeTop() {
        size--;
        if (size > 0) {
            // unlock others
        }
        return pieces.remove(pieces.size() - 1);
    }

    public Piece getTop() {
        return pieces.get(pieces.size() - 1);
    }
}
