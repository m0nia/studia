package wybory;

public class PartiaZRozmachem extends Partia {

    public PartiaZRozmachem(int budżet, String nazwaPartii) {
        super(budżet, nazwaPartii);
    }

    @Override
    public void przeprowadźKampanię(OkręgWyborczy[] okręgi, Wektor[] działania) {
        Wektor najdroższeDziałanie = działania[0];
        OkręgWyborczy najdroższyOkręg = okręgi[0];
        int najwyższaCena = Integer.MIN_VALUE;

        for (OkręgWyborczy okręg : okręgi) {
            for (Wektor działanie : działania) {
                int koszt = okręg.koszt(działanie);
                if (koszt > najwyższaCena && budżet - koszt >= 0) {
                    najdroższeDziałanie = działanie;
                    najdroższyOkręg = okręg;
                    najwyższaCena = koszt;
                }
            }
        }
        assert (budżet - najwyższaCena >= 0);
        budżet -= najwyższaCena;
        najdroższyOkręg.wykonajDziałanie(najdroższeDziałanie);
    }
}
