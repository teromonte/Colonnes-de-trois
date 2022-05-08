package game;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

import entities.Response;
import strategies.Algo;
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
        Response res;

        // if (matchRound == 0) {
        // if (blanc.size() != 8) { // NOIR TURN
        // } else if (blanc.size() == noir.size() && blanc.size() == 0) { // BLANC TURN
        // }
        // } else {
        // }
        // Algo d = new Algo(color, table);
        // Move m = d.getMove();
        // take cara if its a move or a place - TODO
        // table[m.getPiece().getX()][m.getPiece().getY()].removeTop();
        // table[m.getMove().getX()][m.getMove().getY()].addPiece(color);
        // res = new Response(0,ThreadLocalRandom.current().nextInt(0, 3),
        // ThreadLocalRandom.current().nextInt(0, 3));

        if (counter == 3)
            lin = 1;
        if (counter == 6)
            lin = 2;
        if (counter == 9) {
            lin = 0;
            col = 1;
        }
        if (counter == 12)
            lin = 1;
        if (counter == 15)
            lin = 2;
        if (counter == 18) {
            counter = 0;
            lin = 0;
            col = 0;
        }

        counter++;

        if (color == Utils.BLANC) {
            res = new Response(0, col, lin);
        } else {
            res = new Response(0, col, lin);
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
