package game;

import utils.Utils.Color;

public class Piece {

    private Color couleur;
    private boolean isLocked;

    public Piece(Color couleur) {
        this.couleur = couleur;
        this.isLocked = false;
    }

    public boolean isLocked() {
        return isLocked;
    }

    public boolean swichState() {
        if (isLocked)
            isLocked = false;
        else
            isLocked = true;
        return isLocked;
    }

    public Color getColor() {
        return couleur;
    }

}
