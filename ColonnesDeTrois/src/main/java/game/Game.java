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

        if (blanc.isEmpty() && noir.isEmpty()) {
            if (matchRound == Utils.FIRST_MATCH)
                alg = new Algo(color, matchRound, table, false);
            else
                alg = new Algo(1 - color, matchRound, table, false);

            GameMove move = alg.decideMove(); //
            if (move.getArr() == null) {
                // move type = passe
                return new Response(Utils.PASSE);
            } else {
                // move type = move
                Piece removed = table[move.getDep().getX()][move.getDep().getY()].removeTop();
                table[move.getArr().getX()][move.getArr().getY()].addPiece(removed.getColor());

                return new Response(Utils.MOVE, move.getDep().getX(), move.getDep().getY(), move.getArr().getX(),
                        move.getArr().getY());
            }
        } else {
            if (matchRound == Utils.FIRST_MATCH)
                alg = new Algo(color, matchRound, table, true);
            else
                alg = new Algo(1 - color, matchRound, table, true);

            GameMove pair = alg.decideMove(); //
            if (pair.getDep() == null) {
                // move type = passe
                return new Response(Utils.PASSE);
            } else {
                // move type = place
                if (matchRound == Utils.FIRST_MATCH) {
                    if (color == Utils.BLANC) {
                        blanc.remove(blanc.size() - 1);
                        table[pair.getDep().getX()][pair.getDep().getY()].addPiece(Utils.BLANC);
                    } else {
                        noir.remove(noir.size() - 1);
                        table[pair.getDep().getX()][pair.getDep().getY()].addPiece(Utils.NOIR);
                    }

                } else {
                    if (color == Utils.BLANC) {
                        noir.remove(noir.size() - 1);
                        table[pair.getDep().getX()][pair.getDep().getY()].addPiece(Utils.NOIR);

                    } else {
                        blanc.remove(blanc.size() - 1);
                        table[pair.getDep().getX()][pair.getDep().getY()].addPiece(Utils.BLANC);
                    }
                }

                return new Response(Utils.PLACE, pair.getDep().getX(), pair.getDep().getY());
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
