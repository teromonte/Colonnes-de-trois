import Utils;

public class Piece {

    private Square square ; //position actuelle
    couleur couleur ;

    public Piece(Square square , couleur couleur) {
        this.square = square;
        this.square.setPion(this);
        this.couleur = couleur;

    }

}
