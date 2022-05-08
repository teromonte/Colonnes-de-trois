package strategies;

import java.util.ArrayList;
import java.util.List;

import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils;

public class Algo {
    private Square[][] table;
    private int color;

    public Algo(int color, Square[][] table) {
        this.color = color;
        this.table = table;
    }

    public Pair getPair() {
        return getValidSquares().get(0);
    }

    public Move getMove() {
        return getValidMoves().get(0);
    }

    private List<Move> getValidMoves() {
        List<Move> res = new ArrayList<>();
        List<Pair> placesToGo = getValidSquares();// all the places i can go
        List<Pair> myPiecesTop = getPiecesFromTop(this.color);// all the pices that i can move

        for (Pair p1 : myPiecesTop)
            for (Pair p2 : placesToGo)
                if (Utils.checkIfNeighbor(p1, p2)) // check ig paire is naigboor with the other
                    res.add(new Move(p1, p2));

        return res;
    }

    private List<Pair> getValidSquares() {
        List<Pair> res = new ArrayList<>();// initialise à list
        // return every square in the table that has < 3 pieces
        for (int i = 0; i < Utils.MAX_PILE_SIZE; i++) {
            for (int j = 0; j < Utils.MAX_PILE_SIZE; j++) {
                if (table[i][j].getSize() < 3) {
                    Pair p = new Pair(i, j);
                    res.add(p);
                }
            }
        }
        return res;
    }

    private List<Pair> getPiecesFromTop(int color) {
        List<Pair> tmp = new ArrayList<>();
        // get all pieces fron the top of the piles from my color
        for (int i = 0; i < Utils.MAX_PILE_SIZE; i++) {
            for (int j = 0; j < Utils.MAX_PILE_SIZE; j++) {
                if (table[i][j].getTop().getColor() == color) {
                    table[i][j].getTop();
                    Pair paire = new Pair(i, j);
                    tmp.add(paire);
                }
            }
        }
        return tmp;
    }
}
