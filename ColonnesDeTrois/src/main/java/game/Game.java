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
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        matchRound = 0;
    }

    public Response reset() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
        this.blanc = initialiseArray(Utils.BLANC);
        this.noir = initialiseArray(Utils.NOIR);
        matchRound++;
        return new Response(Utils.RESET);
    }

    public Response getNextMove(int color) {
        Algo alg = new Algo(color, table);
        Response res;

        if (blanc.size() != 0 || noir.size() != 0) {
            // still needs to place pieces in the table
            Pair pair = alg.getPair();
            if (pair == null) {
                // move type = passe
                res = new Response(2);
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
            Move m = alg.getMove();
            if (Utils.BLANC == color) {
                table[m.getPiece().getX()][m.getPiece().getY()].removeTop();
                table[m.getMove().getX()][m.getMove().getY()].addPiece(Utils.BLANC);
            } else {
                table[m.getPiece().getX()][m.getPiece().getY()].removeTop();
                table[m.getMove().getX()][m.getMove().getY()].addPiece(Utils.NOIR);
            }
            res = new Response(0, m.getPiece().getX(), m.getPiece().getY(), m.getMove().getX(), m.getMove().getY());
        }
        return res;
    }

    private List<Piece> initialiseArray(int color) {
        List<Piece> p = new ArrayList<>(Utils.N_PIECES);
        for (int i = 0; i < Utils.N_PIECES; i++)
            p.add(new Piece(color));

        return p;
    }
}
