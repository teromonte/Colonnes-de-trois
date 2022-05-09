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

    public Pair getBestPlace() {
        return getValidSquaresToGo().remove(getValidSquaresToGo().size()-1);
    }

    public Move getBestDisplace() {
        return getDisplaceMovements().remove(getDisplaceMovements().size()-1);
    }

    private List<Pair> getValidSquaresToGo() {
        List<Pair> res = new ArrayList<>();// initialise à list
        // return every square in the table that has < 3 pieces
        for (int i = 0; i < Utils.MAX_PILE_SIZE; i++)
            for (int j = 0; j < Utils.MAX_PILE_SIZE; j++)
                if (table[i][j].getSize() < 3) {
                    Pair p = new Pair(i, j);
                    res.add(p);
                }

        return res;
    }

    private List<Move> getDisplaceMovements() {
        List<Move> res = new ArrayList<>();
        List<Pair> placesToGo = getValidSquaresToGo();// all the places i can go
        List<Pair> myPiecesTop = getMovablePieces(this.color);// all the pices that i can move

        for (Pair p1 : myPiecesTop)
            for (Pair p2 : placesToGo)
                if (Utils.checkIfNeighbor(p1, p2)) // check ig paire is naigboor with the other
                    res.add(new Move(p1, p2));

        return res;
    }

    private List<Pair> getMovablePieces(int color) {
        List<Pair> tmp = new ArrayList<>();
        // get all pieces fron the top of the piles from my color
        for (int i = 0; i < Utils.MAX_PILE_SIZE; i++)
            for (int j = 0; j < Utils.MAX_PILE_SIZE; j++)
                if (table[i][j].getTop() != null)
                    if (table[i][j].getTop().getColor() == color) {
                        Pair paire = new Pair(i, j);
                        tmp.add(paire);
                    }

        return tmp;
    }
}
