package wybory;

public abstract class Partia {

    protected int budżet;

    protected String nazwaPartii;

    public Partia(int budżet, String nazwaPartii) {
        this.budżet = budżet;
        this.nazwaPartii = nazwaPartii;
    }

    public abstract void przeprowadźKampanię(OkręgWyborczy[] okręgi, Wektor[] działania);

    public boolean czyStaćCięNaKampanię(OkręgWyborczy[] okręgi, Wektor[] działania) {
        for (OkręgWyborczy okręg : okręgi) {
            for (Wektor działanie : działania) {
                if (budżet - okręg.koszt(działanie) >= 0)
                    return true;
            }
        }
        return false;
    }

    public String nazwaPartii() {
        return nazwaPartii;
    }
}

