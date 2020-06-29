package wybory;

import java.util.Objects;

public class Kandydat {

    private Wektor cechy;

    private String nazwaPartii;

    private int numerNaLiście;

    private int numerOkręgu;

    private String imię;

    private String nazwisko;


    public Kandydat(String imię, String nazwisko, int numerOkręgu, String nazwaPartii, int numerNaLiście, Wektor cechy) {
        this.cechy = cechy;
        this.nazwaPartii = nazwaPartii;
        this.numerNaLiście = numerNaLiście;
        this.numerOkręgu = numerOkręgu;
        this.imię = imię;
        this.nazwisko = nazwisko;
    }

    public int cecha(int numer) {
        return cechy.wartość(numer - 1);
    }

    public Wektor cechy() {
        return new Wektor(cechy);
    }

    public String partia() {
        return nazwaPartii;
    }

    public int numerNaLiście() {
        return numerNaLiście;
    }

    public int numerOkręgu() {
        return numerOkręgu;
    }

    @Override
    public String toString() {
        return imię + " " + nazwisko;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Kandydat kandydat = (Kandydat) o;
        return numerNaLiście == kandydat.numerNaLiście &&
                numerOkręgu == kandydat.numerOkręgu &&
                nazwaPartii.equals(kandydat.nazwaPartii);
    }

    @Override
    public int hashCode() {
        return Objects.hash(nazwaPartii, numerNaLiście, numerOkręgu);
    }

}
