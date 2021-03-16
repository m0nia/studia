package cover;

public class ZbiórPokrywany {

    private int n;

    public ZbiórPokrywany(int n) {
        this.n = n;
    }

    // Zwraca ile jest elementów w zbiorze pokrywanym, które należą do zbioru A i które nie są jeszcze pokryte.
    public int ilePokryje(ZbiórPokrywający A, ZbiórPokrywający pokryte) {
        int x = 1;
        int licznik = 0;
        while (x <= n) {
            if (A.zawiera(x) && !pokryte.zawiera(x))
                licznik++;
            x++;
        }
        return licznik;
    }

    public int rozmiar() {
        return n;
    }
}
