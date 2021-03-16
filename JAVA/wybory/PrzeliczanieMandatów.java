package wybory;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;

public abstract class PrzeliczanieMandatów {

    // Dodaje do posortowanej malejąco listy parę (nazwaPartii, wartość obliczana z liczby głosów partii zależnie od metody)
    // Lista jest posortowana względem drugiego elementu pary
    protected static void dodajDoPosortowanej(LinkedList<Para<String, Double>> listaMalejąca, Para<String, Double> para) {
        Iterator<Para<String, Double>> it = listaMalejąca.iterator();
        boolean znaleziono = false;
        int indeks = 0;

        while (it.hasNext() && !znaleziono) {
            double aktualny = it.next().drugi();
            if (para.drugi() > aktualny)
                znaleziono = true;
            else
                indeks++;
        }

        listaMalejąca.add(indeks, para);
    }

    public abstract Map<String, Integer> przeliczNaMandaty(Map<String, Integer> sumaGłosówPartii, int ilośćMandatów);

    // Zapisuje mandat zdobyty przez partię
    protected void zarejestrujMandat(Map<String, Integer> mandaty, String partia) {
        int liczbaMandatówPartii = 0;
        if (mandaty.containsKey(partia))
            liczbaMandatówPartii = mandaty.get(partia);
        mandaty.put(partia, liczbaMandatówPartii + 1);
    }

}