package wybory;

class OkręgWyborczy {

    private ListaWyborcza listaWyborcza;

    private Wyborca[] wyborcy;

    private int numerOkręgu;

    public OkręgWyborczy(int numerOkręgu, Wyborca[] wyborcy, ListaWyborcza listaWyborcza) {
        this.numerOkręgu = numerOkręgu;
        this.wyborcy = wyborcy;
        this.listaWyborcza = listaWyborcza;
    }

    public ListaWyborcza dajListęWyborczą() {
        return new ListaWyborcza(listaWyborcza);
    }

    public ListaWyborcza dajListęWyborczą(String nazwaPartii) {
        ListaWyborcza kopia = new ListaWyborcza();
        ListaKandydatów listaPartii = listaWyborcza.dajListęPartii(nazwaPartii);
        kopia.dodajListęPartii(new ListaKandydatów(listaPartii));
        return kopia;
    }

    public Komisja przeprowadźGłosowanie() {
        Komisja komisja = new Komisja(wyborcy.length / 10, listaWyborcza);
        for (Wyborca w : wyborcy) {
            komisja.zarejestrujGłos(w.głosuj(this));
        }
        return komisja;
    }

    public void wykonajDziałanie(Wektor działanie) {
        for (Wyborca w : wyborcy) {
            w.zmieńOpinię(działanie);
        }
    }

    // Zwraca koszt danego działania w tym okręgu
    public int koszt(Wektor działanie) {
        return wyborcy.length * działanie.sumaModułówWspółrzędnych();
    }

    public int numer() {
        return numerOkręgu;
    }

    // Zwraca obiekt, który przekazuje informacje o preferencjach wyborczów wyborców w tym okręgu
    public Sondaż sondaż() {
        return new Sondaż(wyborcy);
    }

    public OkręgWyborczy scal(OkręgWyborczy o) { //Scalony okręg wyborczy ma taki numer jak mniejszy ze scalanych
        Wyborca[] nowiWyborcy = new Wyborca[wyborcy.length + o.wyborcy.length];
        int k = 0;
        for (int i = 0; i < wyborcy.length; i++)
            nowiWyborcy[k++] = wyborcy[i];

        for (int i = 0; i < o.wyborcy.length; i++)
            nowiWyborcy[k++] = o.wyborcy[i];

        ListaWyborcza nowaLista = listaWyborcza.scal(o.listaWyborcza);
        return new OkręgWyborczy(numerOkręgu, nowiWyborcy, nowaLista);
    }
}