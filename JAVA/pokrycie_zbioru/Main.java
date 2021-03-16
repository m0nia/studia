package cover;

import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {

        RodzinaZbiorów R = new RodzinaZbiorów();
        Zapytanie Z = new Zapytanie();
        Rozwiązanie r = null;

        Dane input = new Dane();

        while (!input.koniecDanych()) {
            input.czytajDane(R, Z);
            if(input.podanoZapytanie()){
                switch(Z.dajStrategię()){
                    case 1:
                        r = new AlgorytmDokładny();
                        break;
                    case 2:
                        r = new HeurystykaZachłanna();
                        break;
                    case 3:
                        r = new HeurystykaNaiwna();
                        break;
                }
                r.rozwiąż(R, Z.dajZbiórPokrywany());
                System.out.println(r);
            }
        }
    }
}
