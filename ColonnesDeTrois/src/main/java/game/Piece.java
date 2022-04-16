package game;

import utils.Utils.Color;

public class Piece {

    private Color couleur;
    private boolean isLocked;

    public Piece(Color couleur) {
        this.couleur = couleur;
    }

    public boolean isLocked() {
        return isLocked;
    }

    public Color getColor() {
        return couleur;
    }

}
