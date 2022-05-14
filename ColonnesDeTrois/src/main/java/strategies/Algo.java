package strategies;

import java.util.ArrayList;
import java.util.List;

import game.GameMove;
import game.Piece;
import game.Square;
import utils.Move;
import utils.Pair;
import utils.Utils;

public class Algo {
    private Square[][] table;
    private int color;
    private int matchRound;
    private GameMove bestMove;
    private boolean isPart1;

    public Algo(int color, int matchRound, Square[][] table, boolean isPart1) {
        this.color = color;
        this.table = table;
        this.matchRound = matchRound;
        this.isPart1 = isPart1;
        this.bestMove = null;
    }

    // Alpha-Beta
    public GameMove decideMove() {
        maximizer(Utils.DEPTH, Integer.MIN_VALUE, Integer.MAX_VALUE);
        return bestMove;
    }

    private int maximizer(int depth, int alpha, int beta) {
        if (depth == 0) {
            return computeRating(color);
        }

        List<GameMove> legalMoves = getValidMoves(isPart1);

        for (GameMove move : legalMoves) {
            makeMove(move, color);
            color = 1 - color;
            int rating = minimizer(depth - 1, alpha, beta);
            color = 1 - color;
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

    private int minimizer(int depth, int alpha, int beta) {
        if (depth == 0) {
            return computeRating(color);
        }

        List<GameMove> legalMoves = getValidMoves(isPart1);

        for (GameMove move : legalMoves) {
            makeMove(move, color);
            color = 1 - color;
            int rating = maximizer(depth - 1, alpha, beta);
            color = 1 - color;
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

    private void makeMove(GameMove move, int currColor) {
        // this is a place move
        if (move.getArr() != null) {
            table[move.getDep().getX()][move.getDep().getY()].addPiece(currColor);
        } else {
            Piece removed = table[move.getDep().getX()][move.getDep().getY()].removeTop();
            table[move.getArr().getX()][move.getArr().getY()].addPiece(removed.getColor());
        }
    }

    private void undoMove(GameMove move) {
        if (move.getArr() != null) {
            table[move.getDep().getX()][move.getDep().getY()].removeTop();
        } else {
            Piece removed = table[move.getArr().getX()][move.getArr().getY()].removeTop();
            table[move.getDep().getX()][move.getDep().getY()].addPiece(removed.getColor());
        }
    }

    // Return 0 if someboody won 1 if nobody won
    // 2 if nobody won and there is no possible moves anymore
    private int computeRating(int color) {
        Pair p = hasWon();
        if (p != null) {
            if (table[p.getX()][p.getY()].getTop().getColor() == color) {
                return 1;
            } else {
                return -1;
            }
        }
        return 0;
    }

    // check if somebody wons
    private Pair hasWon() {
        for (int i = 0; i < Utils.N_ROWS; i++)
            for (int j = 0; j < Utils.N_COLS; j++) {
                if (table[i][j].hasPileOfSameColor())
                    return new Pair(i, j);

            }
        return null;
    }

    // get moves depending on the game part
    private List<GameMove> getValidMoves(boolean isPart1) {
        if (isPart1) {
            return getValidSquaresToGo();
        } else {
            return getDisplaceMovements();
        }
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
