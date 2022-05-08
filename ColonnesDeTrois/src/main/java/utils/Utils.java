package utils;

import game.Square;

public class Utils {
    public static final int MAX_PILE_SIZE = 3;
    public static final int N_COLS = 3;
    public static final int N_ROWS = 3;
    public static final int N_PIECES = 8;
    public static final int BLANC = 0;
    public static final int NOIR = 1;
    public static final int PLACE = 0;
    public static final int MOVE = 1;
    public static final int RESET = 2;
    public static final int N_PLAYERS = 2;


    public static int countPieces(Square[][] table) {
        int res = 0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                res += table[i][j].getSize();
            }
        }
        return res;
    }

    public static boolean isValid(Square[][] table, Move m) {
        if (table[m.getMove().getX()][m.getMove().getY()].getSize() > 2) {
            return false;
        }

        return m.getMove().getX() >= 0 && m.getMove().getX() <= 2 && m.getMove().getY() >= 0 && m.getMove().getY() <= 2;
    }

    public static boolean isValid(Square[][] table, Pair p) {
        if (table[p.getX()][p.getY()].getSize() > 2) {
            return false;
        }

        return p.getX() >= 0 && p.getX() <= 2 && p.getY() >= 0 && p.getY() <= 2;
    }
}
