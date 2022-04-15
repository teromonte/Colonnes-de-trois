import java.util.ArrayList;
import java.util.List;

public class pions {

    private  List<pions> FreePieces;
    private  List<pions> BlockedPieces;

    private static int[][] position ;


    public pions(int[][] position) {
        this.position = position;
    }


    public static int[][] getPosition() {
        return position;
    }

    public void setPosition(int[][] position) {
        this.position = position;
    }




    public List<pions> getFreePieces()
    {
        return FreePieces;
    }

    public List<pions> getBlockedPieces()
    {
        return BlockedPieces;
    }

    public void addFreePiece(pions piece)
    {
        FreePieces.add(piece);
    }

    public void addBlockedPiece(pions piece)
    {
        BlockedPieces.add(piece);
    }



}
