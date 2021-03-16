package cover;

import java.util.ArrayList;
import java.util.Scanner;

public class Dane {

    static final Scanner in = new Scanner(System.in);

    private int ostatni;

    private int licznik;

    private boolean podanoZapytanie;

    public Dane() {
        ostatni = 0;
        licznik = 1;
        podanoZapytanie = false;
    }

    public void czytajDane(RodzinaZbiorów R, Zapytanie Z) { //Wczytuje zbiór albo zapytanie
        ostatni = in.nextInt(); //pierwszy znak definicji zbioru lub zapytania

        if (ostatni >= 0) {
            R.dodaj(czytajZbiór());
            licznik++;
            podanoZapytanie = false;
        } else {
            czytajZapytanie(Z);
            podanoZapytanie = true;
        }
    }

    public boolean podanoZapytanie(){
        return podanoZapytanie;
    }

    public ZbiórZłożony czytajZbiór() { //po wykonaniu nastąpi podanie kolejnego zbioru lub zapytanie

        ZbiórZłożony A = new ZbiórZłożony(licznik);

        ArrayList<Integer> elementy = new ArrayList<>();

        while (ostatni != 0) { //definicja zbioru zawsze kończy się zerem
            elementy.add(ostatni);
            ostatni = in.nextInt();
        }

        int i = 0;
        while (i < elementy.size()) {
            if (i + 1 == elementy.size() || elementy.get(i + 1) > 0) {
                A.dodaj(new Jednoelementowy(elementy.get(i)));
                i += 1;
            } else { //wtedy i + 1 != elementy.size() i elementy.get(i+1) < 0
                if (i + 2 == elementy.size() || elementy.get(i + 2) > 0) {
                    A.dodaj(new ArytmetycznyNieskończony(elementy.get(i), (-1) * elementy.get(i + 1)));
                    i += 2;
                } else { //wtedy i + 2 != elementy.size() i elementy.get(i+2) < 0
                    A.dodaj(new ArytmetycznySkończony(elementy.get(i), (-1) * elementy.get(i + 1), (-1) * elementy.get(i + 2)));
                    i += 3;
                }
            }
        }
        return A;
    }

    public void czytajZapytanie(Zapytanie Z) {
        Z.setZapytanie(ostatni*(-1), in.nextInt());
    }

    public boolean koniecDanych() {
        if(in.hasNext())
            return false;
        return true;
    }

}