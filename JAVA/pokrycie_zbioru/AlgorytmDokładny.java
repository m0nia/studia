package cover;

import java.util.ArrayList;

public class AlgorytmDokładny extends Rozwiązanie {

    private int najlepsze; //długość najlepszego dotąd rozwiązania

    public AlgorytmDokładny() {
        super();
        najlepsze = Integer.MAX_VALUE;
    }

    public void rozwiąż(RodzinaZbiorów R, ZbiórPokrywany Z) {

        //W tej liście przechowywane będą wszystkie możliwe rozwiązania.
        ArrayList<Integer> kombinacjaZbiorów = new ArrayList<>();
        pomocnicza(R, Z, kombinacjaZbiorów, 0);

    }

    private void pomocnicza(RodzinaZbiorów R, ZbiórPokrywany Z, ArrayList<Integer> kombinacjaZbiorów, int i) {
        if (ilePokrytych == Z.rozmiar() && kombinacjaZbiorów.size() <= najlepsze) {
            rozwiązanie = new ArrayList<>(kombinacjaZbiorów);
            najlepsze = rozwiązanie.size();
            czyPokryty = true;
            return;
        }
        if (i < R.rozmiar()) {
            pomocnicza(R, Z, kombinacjaZbiorów, i + 1); //nie dodaję i-tego zbioru do rozwiązania

            int ile = Z.ilePokryje(R.daj(i), pokryte);
            if(ile > 0){
                ilePokrytych += ile;
                kombinacjaZbiorów.add(R.daj(i).numer());

                //Tworzę kopię aktualnie pokrytych elementów, żeby móc wykonać backtracking
                ZbiórZłożony pokrytePom = new ZbiórZłożony(pokryte);
                pokryte.dodaj(R.daj(i));

                pomocnicza(R, Z, kombinacjaZbiorów, i + 1); //dodaję i-ty zbiór do rozwiązania

                // backtracking
                pokryte = pokrytePom;
                ilePokrytych -= ile;
                kombinacjaZbiorów.remove(kombinacjaZbiorów.size() - 1);
            }
        }
    }
}
