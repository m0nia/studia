package wybory;

import java.util.Arrays;
import java.util.LinkedList;

public class MojaPartia extends Partia {

    public MojaPartia(int budżet, String nazwaPartii) {
        super(budżet, nazwaPartii);
    }

    @Override
    public void przeprowadźKampanię(OkręgWyborczy[] okręgi, Wektor[] działania) { //wybiera najtańsze działanie i potem okręg, w którym jest najmniej głosujących na jedną partię
        int najniższaCena = Integer.MAX_VALUE;
        Wektor najtańszeDziałanie = null;
        for (Wektor w : działania) {
            int cena = w.sumaModułówWspółrzędnych();
            if (cena < najniższaCena) {
                najniższaCena = cena;
                najtańszeDziałanie = w;
            }
        }

        LinkedList<OkręgWyborczy> listaOkręgów = new LinkedList<>(Arrays.asList(okręgi));

        OkręgWyborczy najlepszy = null;
        boolean wybrany = false;
        while (!wybrany) {
            najlepszy = wybierzNajmniejPartyjnych(listaOkręgów);
            if (budżet - najlepszy.koszt(najtańszeDziałanie) < 0) {
                listaOkręgów.remove(najlepszy);
            } else {
                wybrany = true;
            }
        }
        assert (budżet - najlepszy.koszt(najtańszeDziałanie) >= 0);
        budżet -= najlepszy.koszt(najtańszeDziałanie);
        najlepszy.wykonajDziałanie(najtańszeDziałanie);
    }

    private OkręgWyborczy wybierzNajmniejPartyjnych(LinkedList<OkręgWyborczy> listaOkręgów) {
        int najmniejPartyjnych = Integer.MAX_VALUE;
        OkręgWyborczy najlepszy = null;
        for (OkręgWyborczy okręg : listaOkręgów) {
            int ile = okręg.sondaż().ileWyborcówPartyjnych();
            if (ile < najmniejPartyjnych) {
                najlepszy = okręg;
                najmniejPartyjnych = ile;
            }
        }
        return najlepszy;
    }
}
