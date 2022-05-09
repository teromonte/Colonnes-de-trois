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

    public void lock() {

        isLocked = true;
    }

    public void unlock() {
        isLocked = false;

    }

    public int getColor() {
        return color;
    }

}
