package wybory;

public class WyboryWBajtocji {

    private static PrzeliczanieMandatów[] metody = {new MetodaDHondta(), new MetodaSainteLaguë(), new MetodaHareaNiemeyera()};


    public static void main(String[] args) {

        Input dane = new Input();
        dane.czytajDane();

        Bajtocja Bajtocja = new Bajtocja(dane.dajPartie(), dane.dajOkręgi(), dane.dajDziałania());

        Bajtocja.formujOkręgiWyborcze(dane.dajOkręgiDoScalenia());

        Bajtocja.kampaniaWyborcza();

        Bajtocja.głosowanie();

        Bajtocja.liczenieGłosów();

        for (PrzeliczanieMandatów metoda : metody) {
            System.out.println(metoda);
            Bajtocja.rozdzielMandaty(metoda);
            Bajtocja.wypiszPodsumowanie();
            System.out.println();
        }
    }
}
