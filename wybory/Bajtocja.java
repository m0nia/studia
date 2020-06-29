package wybory;

import java.util.ArrayList;

public class Bajtocja {

    private Partia[] partie;

    private Wektor[] działania;

    private OkręgWyborczy[] podstawoweOkręgiWyborcze; //podstawowe okręgi, pod i-tym indeksem znajduje się okręg o numerze i + 1

    private OkręgWyborczy[] uformowaneOkręgiWyborcze; // okręgi po scaleniu (scalone i te niescalane)

    private ArrayList<Para<Integer, Integer>> numeryScalanychOkręgów;

    private Komisja[] komisjeOkręgów; //pod i-tym indeksem znajduje się komisja okręgu wyborczego znajdującego się pod i-tym indeksem w tablicy uformowaneOkręgiWyborcze

    public Bajtocja(Partia[] partie, OkręgWyborczy[] podstawoweOkręgiWyborcze, Wektor[] działania) {
        this.partie = partie;
        this.podstawoweOkręgiWyborcze = podstawoweOkręgiWyborcze;
        this.działania = działania;
        this.uformowaneOkręgiWyborcze = null;
        this.komisjeOkręgów = null;
        this.numeryScalanychOkręgów = null;
    }

    public void wypiszPodsumowanie() {
        int i = 0;
        while (i < uformowaneOkręgiWyborcze.length) {
            int numerOkręgu = uformowaneOkręgiWyborcze[i].numer();
            if (scalony(numerOkręgu)) {
                System.out.println("Okręg złączony z okręgów o numerach " + numerOkręgu + " i " + (numerOkręgu + 1));
            } else {
                System.out.println("Okręg numer " + numerOkręgu);
            }
            komisjeOkręgów[i].wypiszGłosy();
            komisjeOkręgów[i].wypiszKandydatów();
            komisjeOkręgów[i].wypiszMandaty();
            i++;
        }

        for (Partia p : partie) {
            int suma = 0;
            for (Komisja komisja : komisjeOkręgów) {
                suma += komisja.ileMandatów(p.nazwaPartii());
            }
            System.out.println(p.nazwaPartii + ", mandaty: " + suma);
        }
    }

    private boolean scalony(int numerOkręgu) {
        if (numeryScalanychOkręgów == null)
            return false;
        for (Para<Integer, Integer> para : numeryScalanychOkręgów) {
            if (para.pierwszy() == numerOkręgu || para.drugi() == numerOkręgu)
                return true;
        }
        return false;
    }

    public void formujOkręgiWyborcze(ArrayList<Para<Integer, Integer>> numeryScalanychOkręgów) {
        this.numeryScalanychOkręgów = numeryScalanychOkręgów;

        if (numeryScalanychOkręgów.size() == 0) {
            uformowaneOkręgiWyborcze = podstawoweOkręgiWyborcze;
            return;
        }
        // scalanie okręgów
        uformowaneOkręgiWyborcze = new OkręgWyborczy[podstawoweOkręgiWyborcze.length - numeryScalanychOkręgów.size()];
        int k = 0;
        int ostatniScalony = 1;
        for (Para<Integer, Integer> scal : numeryScalanychOkręgów) {
            // dodaję do ostatecznej tablicy okręgów okręgi podstawowe które nie są scalane
            while (ostatniScalony < scal.pierwszy() - 1) {
                uformowaneOkręgiWyborcze[k++] = podstawoweOkręgiWyborcze[ostatniScalony + 1 - 1];
                ostatniScalony++;
            }

            OkręgWyborczy o1 = podstawoweOkręgiWyborcze[scal.pierwszy() - 1];
            OkręgWyborczy o2 = podstawoweOkręgiWyborcze[scal.drugi() - 1];
            uformowaneOkręgiWyborcze[k++] = o1.scal(o2);
            ostatniScalony = scal.drugi();
        }
        // dodaję podstawowe okręgi, które nie są scalane i o numerach większych niż ostatni scalony
        for (int i = ostatniScalony; i < podstawoweOkręgiWyborcze.length; i++) {
            uformowaneOkręgiWyborcze[k++] = podstawoweOkręgiWyborcze[i];
        }
    }

    public void kampaniaWyborcza() {
        boolean koniec = false;
        while (!koniec) {
            int ilePrzeprowadziłoKampanię = 0;

            for (Partia partia : partie) {
                if (partia.czyStaćCięNaKampanię(uformowaneOkręgiWyborcze, działania)) {
                    partia.przeprowadźKampanię(uformowaneOkręgiWyborcze, działania);
                    ilePrzeprowadziłoKampanię++;
                }
            }

            if (ilePrzeprowadziłoKampanię == 0)
                koniec = true;
        }
    }

    public void głosowanie() {
        this.komisjeOkręgów = new Komisja[uformowaneOkręgiWyborcze.length];
        int k = 0;
        for (OkręgWyborczy okręg : uformowaneOkręgiWyborcze) {
            komisjeOkręgów[k++] = okręg.przeprowadźGłosowanie();
        }
    }

    public void liczenieGłosów() {
        for (Komisja k : komisjeOkręgów)
            k.policzGłosy();

    }

    public void rozdzielMandaty(PrzeliczanieMandatów metoda) {
        for (Komisja k : komisjeOkręgów)
            k.przeliczNaMandaty(metoda);
    }
}