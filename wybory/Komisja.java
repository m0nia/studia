package wybory;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Komisja {

    private int liczbaMandatów;

    private ArrayList<Głos> głosy;

    private Map<Kandydat, Integer> rejestrGłosów; // mapuje do każdego kandydat liczbę głosów oddanych na niego

    private Map<String, Integer> sumaGłosówPartii; // mapuje do każdej partii liczbę głosów na nią oddanych

    private Map<String, Integer> liczbaMandatówPartii; // mapuje do każdej partii liczbę zdobytych przez nią mandatów

    public Komisja(int liczbaMandatów, ListaWyborcza listaWyborcza) {
        this.liczbaMandatów = liczbaMandatów;
        this.rejestrGłosów = new HashMap<>();

        for (Kandydat k : listaWyborcza)
            rejestrGłosów.put(k, 0);

        this.sumaGłosówPartii = new HashMap<>();
        this.liczbaMandatówPartii = null;
        this.głosy = new ArrayList<>();
    }

    public void zarejestrujGłos(Głos głos) {
        głosy.add(głos);
    }

    public void policzGłosy() {
        for (Głos głos : głosy) {
            dodajGłosNaKandydata(głos.naKogo());
            dodajGłosNaPartię(głos.naKogo().partia());
        }
    }

    private void dodajGłosNaKandydata(Kandydat kandydat) {
        int ileGłosów = 0;
        if (rejestrGłosów.containsKey(kandydat)) {
            ileGłosów = rejestrGłosów.get(kandydat);
        }
        rejestrGłosów.put(kandydat, ileGłosów + 1);
    }

    private void dodajGłosNaPartię(String nazwaPartii) {
        int ileGłosów = 0;
        if (sumaGłosówPartii.containsKey(nazwaPartii)) {
            ileGłosów = sumaGłosówPartii.get(nazwaPartii);
        }
        sumaGłosówPartii.put(nazwaPartii, ileGłosów + 1);
    }

    // przelicza głosy na mandaty z użyciem metody podanej w argumencie
    public void przeliczNaMandaty(PrzeliczanieMandatów metoda) {
        liczbaMandatówPartii = metoda.przeliczNaMandaty(sumaGłosówPartii, liczbaMandatów);
    }

    // Zwraca ile mandatów zdobyła partia o danej nazwie
    public int ileMandatów(String nazwaPartii) {
        return liczbaMandatówPartii.get(nazwaPartii);
    }

    public void wypiszGłosy() {
        for (Głos głos : głosy) {
            System.out.println(głos);
        }
    }

    public void wypiszKandydatów() {
        for (Kandydat k : rejestrGłosów.keySet()) {
            System.out.println(k + " " + k.partia() + " " + k.numerNaLiście() + "; głosy: " + rejestrGłosów.get(k));
        }
    }

    public void wypiszMandaty() {
        for (String partia : sumaGłosówPartii.keySet()) {
            System.out.print("(" + partia + "," + liczbaMandatówPartii.get(partia) + ") ");
        }
        System.out.println();
    }
}


