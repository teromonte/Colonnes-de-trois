package strategies;

import java.util.ArrayList;
import java.util.List;

import game.Piece;
import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils;

public class Algo {
    private Square[][] table;
    private int color;
    private int matchRound;

    public Algo(int color, int matchRound, Square[][] table) {
        this.color = color;
        this.table = table;
        this.matchRound = matchRound;
    }

    public Pair getBestPlace() {
        List<Pair> p = getValidSquaresToGo();

        return p.remove(p.size() - 1);
    }

    public Move getBestDisplace() {
        List<Move> m = getDisplaceMovements();
        if (m.isEmpty()) {
            System.out.println("(javaAPI) No more displace movements found!"); // TODO- remove after algorithm
            return null;
        } else
            return m.remove(m.size() - 1);
    }

    private List<Move> getDisplaceMovements() {
        List<Move> res = new ArrayList<>();
        List<Pair> placesToGo = getValidSquaresToGo();// all the places i can go
        List<Pair> myPiecesTop = getMovablePieces();// all the pices that i can move

        for (Pair p1 : myPiecesTop)
            for (Pair p2 : placesToGo)
                if (Utils.checkIfNeighbor(p1, p2)) { // check ig paire is naigboor with the other
                    System.out.println("DEPL_PION Case (" + p1.getX() + "," + p1.getY() + ") -> Case (" + p2.getX()
                            + "," + p2.getY() + ").");
                    res.add(new Move(p1, p2));
                }
        return res;
    }

    private List<Pair> getValidSquaresToGo() {
        List<Pair> res = new ArrayList<>();// initialise a list
        // return every square in the table that has < 3 pieces
        for (int i = 0; i < Utils.N_COLS; i++)
            for (int j = 0; j < Utils.N_ROWS; j++)
                if (table[i][j].getSize() < Utils.MAX_PILE_SIZE) {
                    Pair p = new Pair(i, j);
                    System.out.println("POS_PION Case (" + p.getX() + "," + p.getY() + ").");
                    res.add(p);
                }

        return res;
    }

    // get all pieces fron the top of the piles from my color
    private List<Pair> getMovablePieces() {
        List<Pair> tmp = new ArrayList<>();
        for (int i = 0; i < Utils.N_COLS; i++)
            for (int j = 0; j < Utils.N_ROWS; j++) {
                if (table[i][j].getTop() != null) {
                    Piece top = new Piece(table[i][j].getTop().getColor());
                    if (matchRound == Utils.FIRST_MATCH) {
                        if (top.getColor() == color) {
                            Pair p = new Pair(i, j);
                            System.out.println("MOVABLE: Case (" + p.getX() + "," + p.getY() + ").");
                            tmp.add(p);
                        }

                    } else {
                        if (top.getColor() != color) {
                            Pair p = new Pair(i, j);
                            System.out.println("MOVABLE: Case (" + p.getX() + "," + p.getY() + ").");
                            tmp.add(p);
                        }
                    }
                }

            }
        return tmp;
    }
}
