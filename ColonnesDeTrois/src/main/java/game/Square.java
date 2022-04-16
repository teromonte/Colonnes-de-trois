package game;

import java.util.ArrayList;
import java.util.List;
import utils.*;

public class Square {
    private List<Piece> pieces;

    public Square() {
        pieces = new ArrayList<Piece>(Utils.MAX_PILE_SIZE);
    }

    public int getSize() {
        return pieces.size();
    }

    public void addPiece(Piece p) {
        pieces.add(p);
    }

    public Piece removeTop() {
        return pieces.remove(pieces.size() - 1);
    }

    public Piece getTop() {
        return pieces.get(pieces.size() - 1);
    }
}
