package cover;

import java.util.ArrayList;

public abstract class Rozwiązanie {

    protected ArrayList<Integer> rozwiązanie; // Ciąg numerów zbiórów, będący wynikiem odpowiednich algorytmów.

    protected ZbiórZłożony pokryte; // Suma teoriomnogościowa dotychczas wybranych zbiorów.

    protected int ilePokrytych; // Ile elementów zbioru pokrywanego zostało pokrytych.

    protected boolean czyPokryty; // Czy zastosowany algorytm powiódł się, tzn. czy zbiór został pokryty.

    public Rozwiązanie() {
        rozwiązanie = new ArrayList<>();
        pokryte = new ZbiórZłożony();
        ilePokrytych = 0;
        czyPokryty = false;
    }

    public abstract void rozwiąż(RodzinaZbiorów R, ZbiórPokrywany Z);

    @Override
    public String toString() {
        if (rozwiązanie.isEmpty() || czyPokryty == false)
            return 0 + "";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < rozwiązanie.size() - 1; i++) {
            sb.append(rozwiązanie.get(i));
            sb.append(" ");
        }
        sb.append(rozwiązanie.get(rozwiązanie.size() - 1));
        return sb.toString();
    }

}
