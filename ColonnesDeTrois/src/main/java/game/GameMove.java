package game;

import utils.Pair;

public class GameMove {
    private Pair dep;
    private Pair arr;

    public GameMove(Pair dep, Pair arr) {
        this.dep = dep;
        this.arr = arr;
    }

    public GameMove(Pair dep) {
        this.dep = dep;
        this.arr = new Pair(-1, -1);
    }

    public Pair getDep() {
        return dep;
    }
    
    public Pair getArr() {
        return arr;
    }

}
