import java.util.List;

import game.Piece;

public class pions {

    private List<Piece> FreePieces;
    private List<Piece> BlockedPieces;

    public List<Piece> getFreePieces() {
        return FreePieces;
    }

    public List<Piece> getBlockedPieces() {
        return BlockedPieces;
    }

    public void addFreePiece(Piece onePion) {
        FreePieces.add(onePion);
    }

    public void RendeBlockedPiece(Piece onePion) {
        FreePieces.remove(onePion);
        BlockedPieces.add(onePion);
    }

    public void RendeFreePiece(Piece onePion) {
        BlockedPieces.remove(onePion);
        FreePieces.add(onePion);
    }

}
