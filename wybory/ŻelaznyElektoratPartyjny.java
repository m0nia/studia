package wybory;

import java.util.Random;

public class ŻelaznyElektoratPartyjny extends Wyborca {

    public ŻelaznyElektoratPartyjny(String imię, String nazwisko, int numerOkręgu, String ulubionaPartia) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
    }

    @Override
    protected Kandydat wybierzKandydata(ListaWyborcza lista) {
        int licznik = 0;
        for (Kandydat k : lista) licznik++;
        Random r = new Random();
        int numerKandydata = r.nextInt(licznik) + 1;

        int i = 1;
        for (Kandydat k : lista) {
            if (i == numerKandydata)
                return k;
            i++;
        }
        return null;
    }

    @Override
    protected void zmieńOpinię(Wektor działanie) {
        // nie zmienia opinii
    }
}
