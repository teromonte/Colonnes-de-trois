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
    private List<Move> validMoves;

    public Algo(int color, Square[][] table) {
        this.color = color;
        this.table = table;
    }

    private List<Move> calcValisMoves() {
        List<Move> res = new ArrayList<>();
        List<Pair> tmp = calcValisPairs();// all the places i can go
        List<Pair> myPiecesTop = getPiecesFromTop(this.color);// all the pices that i can move

        for (Pair p1 : myPiecesTop) {
            for (Pair p2 : tmp) {
                if (Utils.checkIfNeighbor(p1, p2))
                    res.add(new Move(p1, p2));// check ig paire is naigboor with the other
            }
        }

        return null;
    }

    private List<Pair> getPiecesFromTop(int color) {
        List<Pair> tmp = new ArrayList<>();
        // get all pieces fron the top of the piles from my color
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (table[i][j].getTop().getColor() == color) {
                    table[i][j].getTop();
                    Pair paire = new Pair(i, j);
                    tmp.add(paire);
                }
            }
        }
        return tmp;
    }

    private List<Pair> calcValisPairs() {
        List<Pair> res = new ArrayList<>();// initialise à list

        // return every square in the table that has < 3 pieces

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (table[i][j].getSize() < 3) {
                    Pair p = new Pair(i, j);
                    res.add(p);
                }
            }
        }

        return null;
    }

    public Move getMove() {
        // algo
        Pair origin = new Pair(-1, -1);
        Pair destin = new Pair(-1, -1);
        return new Move(origin, destin);
    }

    public Pair getPair() {
        return new Pair(-1, -1);
    }
}
