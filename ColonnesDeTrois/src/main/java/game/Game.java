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
    private int lin = 0;
    private int col = 0;
    private int counter = 0;
    private int color;

    public Game(int color) {
        this.color = color;
        this.table = initialiseTable();
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        this.matchRound = 0;
    }

    public void reset() {
        this.table = initialiseTable();
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        matchRound++;
    }

    public void saveOpponentMove(Response res) {
        switch (res.moveType) {
            case Utils.PLACE:
                if (matchRound == Utils.FIRST_MATCH) {
                    noir.remove(noir.size() - 1);
                    table[res.depCol][res.depLg].addPiece(Utils.NOIR);
                } else {
                    blanc.remove(blanc.size() - 1);
                    table[res.depCol][res.depLg].addPiece(Utils.BLANC);
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

    // SOMETHING BAD, PUTS 3 PIECES IN SAME PILE
    public Response getNextMove() {
        Algo alg = new Algo(color, matchRound, table);

        // still needs to place pieces in the table
        if (blanc.size() != 0 || noir.size() != 0) {
            Pair pair = alg.getBestPlace();
            if (pair == null) {
                // move type = passe
                return new Response(Utils.PASSE);
            } else {
                // move type = place
                if (matchRound == Utils.FIRST_MATCH) {
                    blanc.remove(blanc.size() - 1);
                    table[pair.getX()][pair.getY()].addPiece(Utils.BLANC);
                } else {
                    noir.remove(noir.size() - 1);
                    table[pair.getX()][pair.getY()].addPiece(Utils.NOIR);
                }
                return new Response(Utils.PLACE, pair.getX(), pair.getY());
            }
        } else { // all pieces are already in the table
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
        }
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
