package cover;

import java.util.*;

public class HeurystykaZachłanna extends Rozwiązanie {

    public HeurystykaZachłanna() {
        super();
    }

    public void rozwiąż(RodzinaZbiorów R, ZbiórPokrywany Z) {

        // Pod i-tym indeksem jest true, jeśli w rodzinie zbiorów R pod i-tym indeksem znajduje się zbiór, który ma
        // puste przecięcie ze zbiorem Z lub jeśli ten zbiór już dodaliśmy do rozwiązania.
        boolean[] nieużywaneZbiory = new boolean[R.rozmiar()];
        int ileNieużywanychZbiórów = 0; // ile elementów w tablicy nieużywane zbiory ma wartość true;

        int max, ile;
        ZbiórZłożony najlepszy = new ZbiórZłożony();
        ZbiórZłożony A;

        while (ileNieużywanychZbiórów != nieużywaneZbiory.length) {
            max = 0;
            for (int i = 0; i < R.rozmiar(); i++) {
                if(nieużywaneZbiory[i] != true) {
                    A = R.daj(i);
                    ile = Z.ilePokryje(A, pokryte);
                    if (ile > max) {
                        max = ile;
                        najlepszy = A;
                    }
                    if (ile == 0) {
                        ileNieużywanychZbiórów++;
                        nieużywaneZbiory[i] = true;
                    }
                }
            }
            if (max > 0) {
                rozwiązanie.add(najlepszy.numer());
                pokryte.dodaj(najlepszy);
                nieużywaneZbiory[najlepszy.numer() - 1] = true;
                ileNieużywanychZbiórów++;
                ilePokrytych += max;
            }
        }
        Collections.sort(rozwiązanie);
        czyPokryty = ilePokrytych == Z.rozmiar();

    }
}
