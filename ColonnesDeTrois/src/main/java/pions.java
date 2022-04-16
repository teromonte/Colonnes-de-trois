import java.util.List;

public class pions {

    private  List<onePion> FreePieces;
    private  List<onePion> BlockedPieces;



    public List<onePion> getFreePieces()
    {
        return FreePieces;
    }
    public List<onePion> getBlockedPieces()
    {
        return BlockedPieces;
    }


    public void addFreePiece(onePion onePion)
    {
        FreePieces.add(onePion);
    }


    public void RendeBlockedPiece(onePion onePion)
    {
        FreePieces.remove(onePion);
        BlockedPieces.add(onePion);
    }

    public void RendeFreePiece(onePion onePion)
    {
        BlockedPieces.remove(onePion);
        FreePieces.add(onePion);
    }

}
