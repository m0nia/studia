package wybory;

public class PartiaSkromna extends Partia {

    public PartiaSkromna(int budżet, String nazwaPartii) {
        super(budżet, nazwaPartii);
    }

    @Override
    public void przeprowadźKampanię(OkręgWyborczy[] okręgi, Wektor[] działania) {
        Wektor najtańszeDziałanie = działania[0];
        OkręgWyborczy najtańszyOkręg = okręgi[0];
        int najniższaCena = Integer.MAX_VALUE;

        for (OkręgWyborczy okręg : okręgi) {
            for (Wektor działanie : działania) {
                int koszt = okręg.koszt(działanie);
                if (koszt < najniższaCena) { // Jak jest kilka najtańszych par <okręg, działanie> to nie losuję, tylko biorę to pierwsze
                    najtańszeDziałanie = działanie;
                    najtańszyOkręg = okręg;
                    najniższaCena = koszt;
                }
            }
        }
        assert (budżet - najniższaCena >= 0);
        budżet -= najniższaCena;
        najtańszyOkręg.wykonajDziałanie(najtańszeDziałanie);
    }

}
