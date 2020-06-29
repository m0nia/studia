package wybory;

public class ŻelaznyElektoratKandydata extends Wyborca {

    private int numerKandydataNaLiście;

    public ŻelaznyElektoratKandydata(String imię, String nazwisko, int numerOkręgu, String ulubionaPartia, int numerKandydataNaLiście) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
        this.numerKandydataNaLiście = numerKandydataNaLiście;
    }

    @Override
    protected Kandydat wybierzKandydata(ListaWyborcza lista) {

        for (Kandydat k : lista) {
            if (k.numerNaLiście() == numerKandydataNaLiście && k.numerOkręgu() == numerOkręgu)
                return k;
        }
        return null;
    }

    @Override
    protected void zmieńOpinię(Wektor działanie) {
        // nie zmienia opinii;
    }
}
