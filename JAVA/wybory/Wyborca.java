package wybory;

public abstract class Wyborca {

    protected String imię;

    protected String nazwisko;

    protected String ulubionaPartia; // = null, jeśli Wyborca nie ma ulubionej partii

    protected int numerOkręgu;

    public Wyborca(String imię, String nazwisko, String ulubionaPartia, int numerOkręgu) {
        this.ulubionaPartia = ulubionaPartia;
        this.imię = imię;
        this.nazwisko = nazwisko;
        this.numerOkręgu = numerOkręgu;
    }

    public Głos głosuj(OkręgWyborczy okręg) {
        ListaWyborcza mojaLista;

        if (ulubionaPartia == null)
            mojaLista = okręg.dajListęWyborczą();
        else
            mojaLista = okręg.dajListęWyborczą(ulubionaPartia);

        Kandydat k = wybierzKandydata(mojaLista);
        return new Głos(imię, nazwisko, k);
    }

    public String ulubionaPartia() {
        return ulubionaPartia;
    }

    protected abstract Kandydat wybierzKandydata(ListaWyborcza lista);

    protected abstract void zmieńOpinię(Wektor działanie);

    public boolean czyZmieniaszOpinię() {
        return false;
    }
}