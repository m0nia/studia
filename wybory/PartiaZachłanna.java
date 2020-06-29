package wybory;

public class PartiaZachłanna extends Partia {

    public PartiaZachłanna(int budżet, String nazwaPartii) {
        super(budżet, nazwaPartii);
    }

    @Override
    public void przeprowadźKampanię(OkręgWyborczy[] okręgi, Wektor[] działania) { // funkcję wywoływać tylko wtedy gdy można wykonać jakieś działanie
        Wektor najlepszeDziałanie = działania[0];
        OkręgWyborczy najlepszyOkręg = okręgi[0];
        int największaZmianaSumWażonych = Integer.MIN_VALUE;
        //działanie zwiększa sumę ważoną tylko wyborców wszechstronnyvch - to o ile zwiększy w okręgu to liczba wyborców wszechstronnych * suma ważona działania
        for (OkręgWyborczy okręg : okręgi) {
            for (Wektor działanie : działania) {
                int ile = okręg.sondaż().ileWyborcówZmieniaOpinię();
                int zmianaSumWażonych = 0;
                for (Kandydat k : okręg.dajListęWyborczą(nazwaPartii)) {
                    zmianaSumWażonych += działanie.iloczynSkalarny(k.cechy());
                }
                if (zmianaSumWażonych * ile > największaZmianaSumWażonych && budżet - okręg.koszt(działanie) >= 0) {
                    najlepszeDziałanie = działanie;
                    najlepszyOkręg = okręg;
                    największaZmianaSumWażonych = zmianaSumWażonych * ile;
                }
            }
        }
        int koszt = najlepszyOkręg.koszt(najlepszeDziałanie);
        assert (budżet - koszt >= 0);
        budżet -= koszt;
        najlepszyOkręg.wykonajDziałanie(najlepszeDziałanie);
    }
}
