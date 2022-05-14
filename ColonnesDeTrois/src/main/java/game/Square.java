package game;

import java.util.ArrayList;
import java.util.List;
import utils.*;

public class Square {
    private List<Piece> pieces;
    private int size;

    public Square() {
        pieces = new ArrayList<Piece>(Utils.MAX_PILE_SIZE);
        this.size = 0;
    }

    public int getSize() {
        return size;
    }

    public List<Piece> getPieces() {return pieces;}

    public void addPiece(int color) {
        if (size < 3) {
            if (size == 0) {
                pieces.add(new Piece(color));
                size++;
            } else {
                getTop().lock();
                pieces.add(new Piece(color));
                size++;
            }
        }
    }

    public Piece removeTop() {
        if (size == 0) {
            return null;
        } else if (size == 1) {
            Piece p = pieces.remove(size - 1);
            size--;
            return p;
        } else {
            Piece p = pieces.remove(size - 1);
            size--;
            getTop().unlock();
            return p;
        }

    }

    public Piece getTop() {
        if (size > 0) {
            return pieces.get(size - 1);
        } else {
            return null;
        }
    }

    public boolean hasPileOfSameColor() {
        boolean isSameColor = false;
        if (isFull()) {
            if (pieces.get(0).getColor() == pieces.get(1).getColor()
                    && pieces.get(1).getColor() == pieces.get(2).getColor()) {
                isSameColor = true;
            }
        }

        return isSameColor;
    }

    private boolean isFull() {
        return pieces.size() == 3;
    }
}
