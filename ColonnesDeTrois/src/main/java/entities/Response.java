package entities;

public class Response {
    public int moveType;
    public int depCol;
    public int depLg;
    public int arrCol;
    public int arrLg;

    // DISPLACE
    public Response(int moveType, int depCol, int depLg, int arrCol, int arrLg) {
        this.moveType = moveType;
        this.depCol = depCol;
        this.depLg = depLg;
        this.arrCol = arrCol;
        this.arrLg = arrLg;
    }

    // PLACE
    public Response(int moveType, int depCol, int depLg) {
        this.moveType = moveType;
        this.depCol = depCol;
        this.depLg = depLg;
        this.arrCol = 0;
        this.arrLg = 0;
    }

    // PASSE
    public Response(int moveType) {
        this.moveType = moveType;
        this.depCol = 0;
        this.depLg = 0;
        this.arrCol = 0;
        this.arrLg = 0;
    }
}
