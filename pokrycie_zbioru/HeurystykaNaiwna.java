package cover;

import java.util.ArrayList;

public class HeurystykaNaiwna extends Rozwiązanie {

    public HeurystykaNaiwna() {
        super();
    }

    public void rozwiąż(RodzinaZbiorów R, ZbiórPokrywany Z) {
        int ile;
        ZbiórZłożony A;
        for (int i = 0; i < R.rozmiar(); i++) {
            A = R.daj(i);
            ile = Z.ilePokryje(A, pokryte);
            if (ile > 0) {
                rozwiązanie.add(A.numer());
                pokryte.dodaj(A);
                ilePokrytych += ile;
            }
        }
        czyPokryty = ilePokrytych == Z.rozmiar();
    }

}
