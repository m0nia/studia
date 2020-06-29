package wybory;

import java.util.ArrayList;
import java.util.Random;

public abstract class Maksymalizujący extends Wyborca {

    public Maksymalizujący(String imię, String nazwisko, String ulubionaPartia, int numerOkręgu) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
    }

    @Override
    protected Kandydat wybierzKandydata(ListaWyborcza lista) {
        int największa = Integer.MIN_VALUE;
        ArrayList<Kandydat> listaNajlepszych = new ArrayList<>();

        for (Kandydat k : lista) {
            assert (k != null);
            int ocena = oceńKandydata(k);
            if (ocena == największa) {
                listaNajlepszych.add(k);
            } else if (ocena > największa) {
                największa = ocena;
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
