package wybory;

import java.util.*;

// Metoda dzielników przelicza głosy na mandaty dzieląc głos przez kolejne liczby (dzielniki)
public abstract class MetodaDzielników extends PrzeliczanieMandatów {

    protected int poprzedniDzielnik;

    protected abstract int policzNastępnyDzielnik();

    protected abstract void zacznijGenerować();

    @Override
    public Map<String, Integer> przeliczNaMandaty(Map<String, Integer> sumaGłosówPartii, int liczbaMandatów) {

        LinkedList<Para<String, Double>> listaIlorazów = new LinkedList<>();
        int liczbaPartii = sumaGłosówPartii.size();
        boolean koniec = false;

        zacznijGenerować();
        while (!koniec) {
            double maxIloraz = 0;

            for (String nazwaPartii : sumaGłosówPartii.keySet()) {
                double dzielnik = policzNastępnyDzielnik();
                double iloraz = sumaGłosówPartii.get(nazwaPartii) / dzielnik;

                dodajDoPosortowanej(listaIlorazów, new Para(nazwaPartii, iloraz)); //dodaj w odpowiednim miejscu, tzn malejąco

                maxIloraz = Math.max(iloraz, maxIloraz);

            }
            if (listaIlorazów.size() >= liczbaMandatów) {
                if (listaIlorazów.get(liczbaMandatów - 1).drugi() > maxIloraz) // przerywa działanie, kiedy kolejne dzielenia nie dadzą już żadnych mandatów
                    koniec = true;
            }
        }

        return rozdajMandaty(listaIlorazów, liczbaMandatów);

    }

    protected Map<String, Integer> rozdajMandaty(LinkedList<Para<String, Double>> listaIlorazów, int liczbaMandatów) {
        Map<String, Integer> mandaty = new HashMap<>();
        Iterator<Para<String, Double>> it = listaIlorazów.iterator();

        int i = 0;
        assert (listaIlorazów.size() >= liczbaMandatów);
        while (i < liczbaMandatów - 1) { // jeśli ostatni iloraz jest taki sam dla kilku partii to losuję kto dostanie mandat
            String nazwaPartii = it.next().pierwszy();
            zarejestrujMandat(mandaty, nazwaPartii);
            i++;
        }
        //losuje kto dostanie ostatni mandat
        double ostatniIloraz = it.next().drugi();
        int licznik = 1;
        while (it.hasNext()) {
            if (it.next().drugi() == ostatniIloraz)
                licznik++;
        }
        Random r = new Random();
        int ktoDostanieMandat = liczbaMandatów - 1 + r.nextInt(licznik);
        String szczęśliwiec = listaIlorazów.get(ktoDostanieMandat).pierwszy();
        zarejestrujMandat(mandaty, szczęśliwiec);

        return mandaty;
    }

}
