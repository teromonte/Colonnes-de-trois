package game;


public class Piece {

    private int color;
    private boolean isLocked;

    public Piece(int color) {
        this.color = color;
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

    public int getColor() {
        return color;
    }

}
