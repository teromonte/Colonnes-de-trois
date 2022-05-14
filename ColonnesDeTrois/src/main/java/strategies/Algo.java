package strategies;

import java.util.ArrayList;
import java.util.List;

import game.GameMove;
import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils;

public class Algo {
    private Square[][] table;
    private int color;
    private int matchRound;
    private GameMove bestMove;

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

    // Alpha-Beta
    public GameMove decideMove() {
        maximizer(Integer.MIN_VALUE, Integer.MAX_VALUE);
        return bestMove;
    }

    private int maximizer(int alpha, int beta) {
        
        List<GameMove> legalMoves = getDisplaceMovements();
        legalMoves.addAll(getValidSquaresToGo());

        for (GameMove move : legalMoves) {
            makeMove(move);
            side = side.opposite();
            int rating = minimizer(alpha, beta);
            side = side.opposite();
            undoMove(move);

            if (rating > alpha) {
                alpha = rating;
                bestMove = move;
            }

            if (alpha >= beta) {
                return alpha;
            }
        }
        return alpha;
    }

    private int minimizer(int alpha, int beta) {

        List<GameMove> legalMoves = getDisplaceMovements();
        legalMoves.addAll(getValidSquaresToGo());

        for (GameMove move : legalMoves) {
            makeMove(move);
            side = side.opposite();
            int rating = maximizer(alpha, beta);
            side = side.opposite();
            undoMove(move);

            if (rating <= beta) {
                beta = rating;
            }

            if (alpha >= beta) {
                return beta;
            }
        }
        return beta;
    }

    // Return 0 if player white won, 1 if player black won, 2 if nobody won
    // -1 if nobody won and there is no possible moves anymore
    private int GetGameState() {
        boolean hasOwn = false;
        int colorOfWhoWon = -1;

        for (int i = 0; i < Utils.N_ROWS; i++)
            for (int j = 0; j < Utils.N_COLS; j++) {
                hasOwn = table[i][j].hasPileOfSameColor();
                colorOfWhoWon = table[i][j].getTop().getColor();
            }

        if (hasOwn)
            return colorOfWhoWon;
        else if (getDisplaceMovements().isEmpty() && getValidSquaresToGo().isEmpty())
            return -1;

        return 2;

    }

    // get all displace movements
    private List<GameMove> getDisplaceMovements() {
        List<GameMove> res = new ArrayList<>();
        List<GameMove> placesToGo = getValidSquaresToGo();// all the places i can go
        List<Pair> myPiecesTop = getMovablePieces();// all the pices that i can move

        for (Pair p1 : myPiecesTop)
            for (GameMove p2 : placesToGo)
                if (Utils.checkIfNeighbor(p1, p2.getDep())) { // check ig paire is naigboor with the other
                    res.add(new GameMove(p1, p2.getDep()));
                }
        return res;
    }

    // get all valid squares to go
    private List<GameMove> getValidSquaresToGo() {
        List<GameMove> res = new ArrayList<>();// initialise a list
        // return every square in the table that has < 3 pieces
        for (int i = 0; i < Utils.N_COLS; i++)
            for (int j = 0; j < Utils.N_ROWS; j++)
                if (table[i][j].getSize() < Utils.MAX_PILE_SIZE) {
                    Pair p = new Pair(i, j);
                    res.add(new GameMove(p, null));
                }

        return res;
    }

    // get all pieces fron the top of the piles from my color
    private List<Pair> getMovablePieces() {
        List<Pair> tmp = new ArrayList<>();
        for (int i = 0; i < Utils.N_COLS; i++)
            for (int j = 0; j < Utils.N_ROWS; j++) {
                if (table[i][j].getTop() != null) {
                    int topColor = table[i][j].getTop().getColor();
                    if (topColor == Utils.BLANC) {
                        if (matchRound == Utils.FIRST_MATCH) {
                            if (color == Utils.BLANC) {
                                Pair p = new Pair(i, j);
                                tmp.add(p);
                            }
                        } else {
                            if (color == Utils.NOIR) {
                                Pair p = new Pair(i, j);
                                tmp.add(p);
                            }
                        }
                    } else {
                        if (matchRound == Utils.FIRST_MATCH) {
                            if (color == Utils.NOIR) {
                                Pair p = new Pair(i, j);
                                tmp.add(p);
                            }
                        } else {
                            if (color == Utils.BLANC) {
                                Pair p = new Pair(i, j);
                                tmp.add(p);
                            }
                        }
                    }
                }

            }
        return tmp;
    }
}
