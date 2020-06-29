package powerset;

import java.util.ArrayList;
import java.util.Iterator;

public class PowerArrayListImpl <T> implements Iterable<ArrayList<T>> {

    private ArrayList<ArrayList<T>> zbiórPotęgowy;

    public PowerArrayListImpl(ArrayList<T> zbiór){
        zbiórPotęgowy = new ArrayList<>();
        ArrayList<T> podzbiór = new ArrayList<>();
        znajdźPodzbiory(zbiór, podzbiór, 0);
        System.out.println("Liczba podzbiorów: "+ zbiórPotęgowy.size());
    }

    private void znajdźPodzbiory(ArrayList<T> zbiór, ArrayList<T> podzbiór, int i){
        if(i == zbiór.size()) {
            zbiórPotęgowy.add(podzbiór);
            return;
        }

        ArrayList<T> X = new ArrayList<>(podzbiór);
        X.add(zbiór.get(i));

        znajdźPodzbiory(zbiór, X, i + 1);

        znajdźPodzbiory(zbiór, podzbiór, i + 1);

    }

    @Override
    public Iterator<ArrayList<T>> iterator() {
        return zbiórPotęgowy.iterator();
    }
}
