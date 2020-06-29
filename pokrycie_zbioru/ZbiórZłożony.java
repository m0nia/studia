package cover;

import java.util.ArrayList;

// Zbiór Złożony zawiera w sobie Zbiory Proste
public class ZbiórZłożony extends ZbiórPokrywający {

    ArrayList<ZbiórProsty> elementy;

    private int numer;

    public ZbiórZłożony() {
        elementy = new ArrayList<>();
        numer = 0;
    }

    public ZbiórZłożony(ZbiórZłożony klon) {
        elementy = new ArrayList<>(klon.elementy);
        this.numer = klon.numer;
    }

    public ZbiórZłożony(int numer) {
        elementy = new ArrayList<>();
        this.numer = numer;
    }

    public int numer() {
        return numer;
    }

    public void dodaj(ZbiórProsty X) {
        elementy.add(X);
    }

    public void dodaj(ZbiórZłożony A) { //dodaje do zbiory this wszystkie elementy zbioru złożonego A
        for (ZbiórProsty X : A.elementy)
            dodaj(X);
    }

    public boolean zawiera(int x) { // zwraca true jeśli this zawiera element o wartości x, false w przeciwnym wypadku
        for (ZbiórProsty X : elementy) {
            if (X.zawiera(x))
                return true;
        }
        return false;
    }
}
