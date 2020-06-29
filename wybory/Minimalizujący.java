package wybory;

import java.util.ArrayList;
import java.util.Random;

public abstract class Minimalizujący extends Wyborca {

    public Minimalizujący(String imię, String nazwisko, String ulubionaPartia, int numerOkręgu) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
    }

    @Override
    protected Kandydat wybierzKandydata(ListaWyborcza lista) {
        int najmniejsza = Integer.MAX_VALUE;
        ArrayList<Kandydat> listaNajlepszych = new ArrayList<>();

        for (Kandydat k : lista) {
            int ocena = oceńKandydata(k);
            if (ocena == najmniejsza) {
                listaNajlepszych.add(k);
            } else if (ocena < najmniejsza) {
                najmniejsza = ocena;
                listaNajlepszych = new ArrayList<>();
                listaNajlepszych.add(k);
            }
        }

        Random r = new Random();
        int indeksKandydata = r.nextInt(listaNajlepszych.size());
        return listaNajlepszych.get(indeksKandydata);
    }

    protected abstract int oceńKandydata(Kandydat k);
}
