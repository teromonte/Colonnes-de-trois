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
    private int matchRound;
    private int lin = 0;
    private int col = 0;
    private int counter = 0;

    public Game() {
        this.table = initialiseTable();
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        matchRound = 0;
    }

    public void reset() {
        this.table = initialiseTable();
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        matchRound++;
    }

    public Response getNextMove(int color) {
        Algo alg = new Algo(color, table);
        Response res;

        if (blanc.size() != 0 || noir.size() != 0) {
            // still needs to place pieces in the table
            Pair pair = alg.getBestPlace();
            if (pair == null) {
                // move type = passe
                return new Response(Utils.PASSE);
            } else {
                // move type = place
                if (Utils.BLANC == color) {
                    blanc.remove(blanc.size() - 1);
                    table[pair.getX()][pair.getY()].addPiece(Utils.BLANC);
                } else {
                    noir.remove(noir.size() - 1);
                    table[pair.getX()][pair.getY()].addPiece(Utils.NOIR);
                }
                res = new Response(0, pair.getX(), pair.getY());
            }
        } else { // all pieces are already in the table
            Move move = alg.getBestDisplace();
            if (move == null) {
                res = new Response(Utils.PASSE);
            } else {
                Piece removed = table[move.getPiece().getX()][move.getPiece().getY()].removeTop();
                table[move.getMove().getX()][move.getMove().getY()].addPiece(removed.getColor());
                res = new Response(0, move.getPiece().getX(), move.getPiece().getY(), move.getMove().getX(),
                        move.getMove().getY());
            }
        }
        return res;
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
