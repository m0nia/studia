package wybory;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;

public class MetodaHareaNiemeyera extends PrzeliczanieMandatów {

    @Override
    public Map<String, Integer> przeliczNaMandaty(Map<String, Integer> sumaGłosówPartii, int liczbaMandatów) {
        Map<String, Integer> mandaty = new HashMap<>();
        int łącznaLiczbaGłosów = sumaGłosów(sumaGłosówPartii);

        LinkedList<Para<String, Double>> listaReszt = new LinkedList<>();

        int ileZostałoMandatów = liczbaMandatów;

        for (String partia : sumaGłosówPartii.keySet()) {
            double iloraz = (sumaGłosówPartii.get(partia) * liczbaMandatów) / (double) łącznaLiczbaGłosów;
            int mandatyPartii = (int) Math.floor(iloraz);
            mandaty.put(partia, mandatyPartii);
            ileZostałoMandatów -= mandatyPartii;
            dodajDoPosortowanej(listaReszt, new Para<>(partia, iloraz - mandatyPartii));
        }

        Iterator<Para<String, Double>> it = listaReszt.iterator();
        assert (ileZostałoMandatów <= listaReszt.size());
        while (ileZostałoMandatów != 0) {
            zarejestrujMandat(mandaty, it.next().pierwszy());
            ileZostałoMandatów--;
        }
        return mandaty;
    }

    private int sumaGłosów(Map<String, Integer> sumaGłosówPartii) {
        int łącznaLiczbaGłosów = 0;
        for (Integer x : sumaGłosówPartii.values())
            łącznaLiczbaGłosów += x;
        return łącznaLiczbaGłosów;
    }

    @Override
    public String toString() {
        return "Metoda Hare’a-Niemeyera";
    }
}
