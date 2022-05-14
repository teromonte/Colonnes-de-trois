package game;

import java.util.ArrayList;
import java.util.List;

import entities.Response;
import strategies.Algo;
import utils.Move;
import utils.Pair;
import utils.Utils;

public class Game {
    private Square[][] table;
    private List<Piece> blanc;
    private List<Piece> noir;
    private int matchRound; // maybe we use that to know who starts the game
    private int color;

    public Game(int color) {
        this.color = color;
        this.table = initialiseTable();
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        this.matchRound = 0;
    }

    public Response getNextMove() {
        Algo alg;
        if (matchRound == Utils.FIRST_MATCH)
            alg = new Algo(color, matchRound, table);
        else
            alg = new Algo(1 - color, matchRound, table);

        if (blanc.isEmpty() && noir.isEmpty()) {
            Move move = alg.getBestDisplace();
            if (move == null) {
                // move type = passe
                return new Response(Utils.PASSE);
            } else {
                // move type = move
                Piece removed = table[move.getPiece().getX()][move.getPiece().getY()].removeTop();
                table[move.getMove().getX()][move.getMove().getY()].addPiece(removed.getColor());

                return new Response(Utils.MOVE, move.getPiece().getX(), move.getPiece().getY(), move.getMove().getX(),
                        move.getMove().getY());
            }
        } else {
            Pair pair = alg.getBestPlace();
            if (pair == null) {
                // move type = passe
                return new Response(Utils.PASSE);
            } else {
                // move type = place
                if (matchRound == Utils.FIRST_MATCH) {
                    if (color == Utils.BLANC) {
                        blanc.remove(blanc.size() - 1);
                        table[pair.getX()][pair.getY()].addPiece(Utils.BLANC);
                    } else {
                        noir.remove(noir.size() - 1);
                        table[pair.getX()][pair.getY()].addPiece(Utils.NOIR);
                    }

                } else {
                    if (color == Utils.BLANC) {
                        noir.remove(noir.size() - 1);
                        table[pair.getX()][pair.getY()].addPiece(Utils.NOIR);

                    } else {
                        blanc.remove(blanc.size() - 1);
                        table[pair.getX()][pair.getY()].addPiece(Utils.BLANC);
                    }
                }
                
                return new Response(Utils.PLACE, pair.getX(), pair.getY());
            }
        }

    }

    public void saveOpponentMove(Response res) {
        switch (res.moveType) {
            case Utils.PLACE:
                if (matchRound == Utils.FIRST_MATCH) {
                    if (color == Utils.BLANC) {
                        noir.remove(noir.size() - 1);
                        table[res.depCol][res.depLg].addPiece(Utils.NOIR);
                    } else {
                        blanc.remove(blanc.size() - 1);
                        table[res.depCol][res.depLg].addPiece(Utils.BLANC);
                    }

                } else {
                    if (color == Utils.BLANC) {
                        blanc.remove(blanc.size() - 1);
                        table[res.depCol][res.depLg].addPiece(Utils.BLANC);
                    } else {
                        noir.remove(noir.size() - 1);
                        table[res.depCol][res.depLg].addPiece(Utils.NOIR);
                    }
                }
                break;
            case Utils.MOVE:
                Piece removed = table[res.depCol][res.depLg].removeTop();
                table[res.arrCol][res.arrLg].addPiece(removed.getColor());
                break;
            case Utils.PASSE:
                break;
        }

    }

    public void reset() {
        this.table = initialiseTable();
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        matchRound++;
    }

    private List<Piece> initialiseArray(int color) {
        List<Piece> p = new ArrayList<>(Utils.N_PIECES);
        for (int i = 0; i < Utils.N_PIECES; i++)
            p.add(new Piece(color));

        return p;
    }

    private Square[][] initialiseTable() {
        Square[][] temp = new Square[Utils.N_ROWS][Utils.N_COLS];
        for (int i = 0; i < Utils.N_ROWS; i++)
            for (int j = 0; j < Utils.N_COLS; j++)
                temp[i][j] = new Square();

        return temp;
    }
}
