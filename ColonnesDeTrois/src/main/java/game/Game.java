package game;

import java.util.concurrent.ThreadLocalRandom;

import entities.Response;
import utils.Utils;

public class Game {

    private Square[][] table;

    public Game() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
    }

    public void reset() {
        this.table = new Square[Utils.N_ROWS][Utils.N_COLS];
    }

    public Response getNextMove(int color) {
        Response res = new Response();

        //Algo d = new Algo(color, table);
        //Move m = d.getMove();
        // take cara if its a move or a place  - TODO
        //table[m.getPiece().getX()][m.getPiece().getY()].removeTop();
        //table[m.getMove().getX()][m.getMove().getY()].addPiece(color);

        res.moveType = 0;
        res.depCol = ThreadLocalRandom.current().nextInt(0, 3);
        res.depLg = ThreadLocalRandom.current().nextInt(0, 3);
        res.arrCol = 0;
        res.arrLg = 0;

        return res;
    }
}
