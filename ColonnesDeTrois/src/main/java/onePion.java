enum couleur {White , Black}

public class onePion {

    private square square ; //position actuelle
    couleur couleur ;

    public onePion(square square , couleur couleur) {
        this.square = square;
        this.square.setPion(this);
        this.couleur = couleur;

    }

}
