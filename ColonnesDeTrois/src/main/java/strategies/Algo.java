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
        return m.remove(m.size() - 1);
    }

    private List<Pair> getValidSquaresToGo() {
        List<Pair> res = new ArrayList<>();// initialise à list
        // return every square in the table that has < 3 pieces
        for (int i = 0; i < Utils.N_COLS; i++)
            for (int j = 0; j < Utils.N_ROWS; j++)
                if (table[i][j].getSize() < Utils.MAX_PILE_SIZE) {
                    Pair p = new Pair(i, j);
                    res.add(p);
                }

        return res;
    }

    private List<Move> getDisplaceMovements() {
        List<Move> res = new ArrayList<>();
        List<Pair> placesToGo = getValidSquaresToGo();// all the places i can go
        List<Pair> myPiecesTop = getMovablePieces();// all the pices that i can move

        for (Pair p1 : myPiecesTop)
            for (Pair p2 : placesToGo)
                if (Utils.checkIfNeighbor(p1, p2)) // check ig paire is naigboor with the other
                    res.add(new Move(p1, p2));

        return res;
    }

    private List<Pair> getMovablePieces() {
        List<Pair> tmp = new ArrayList<>();
        // get all pieces fron the top of the piles from my color
        for (int i = 0; i < Utils.N_COLS; i++)
            for (int j = 0; j < Utils.N_ROWS; j++)
            
                if (table[i][j].getTop() != null)
                    if (matchRound == Utils.FIRST_MATCH) {
                        if (table[i][j].getTop().getColor() == color) { 
                            Pair paire = new Pair(i, j);
                            tmp.add(paire);
                        }
                    } else {
                        if (color == Utils.BLANC) {
                            if (table[i][j].getTop().getColor() == Utils.NOIR) { 
                                Pair paire = new Pair(i, j);
                                tmp.add(paire);
                            }
                        } else {
                            if (table[i][j].getTop().getColor() == Utils.BLANC) { 
                                Pair paire = new Pair(i, j);
                                tmp.add(paire);
                            }
                        }
                    }

        return tmp;
    }
}
