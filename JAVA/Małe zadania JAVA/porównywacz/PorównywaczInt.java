package tablica;

public class PorównywaczInt implements Porównywacz<Integer> {

    @Override
    public boolean porównaj(Integer a, Integer b) {
        if(a < b)
            return true;
        return false;
    }
}
