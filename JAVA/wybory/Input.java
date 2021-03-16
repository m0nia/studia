package wybory;

import java.util.ArrayList;
import java.util.Scanner;

public class Input {

    Scanner in = new Scanner(System.in);

    private int liczbaOkręgów;

    private int liczbaPartii;

    private int liczbaDziałań;

    private int liczbaCech;

    private Partia[] partieWBajtocji;

    private ArrayList<Para<Integer, Integer>> okręgiDoScalenia;

    private int[] liczbaWyborcówWOkręgach; //pod i-tym indeksem znajduje się liczba wyborców w i+1 okręgu

    private ListaWyborcza[] listyWyborczeOkręgów; //pod i-tym indeksem znajduje się lista wyborcza w i+1 okręgu

    private Wektor[] działania;

    private OkręgWyborczy[] okręgiWBajtocji;

    public Partia[] dajPartie() {
        return partieWBajtocji;
    }

    public OkręgWyborczy[] dajOkręgi() {
        return okręgiWBajtocji;
    }

    public Wektor[] dajDziałania() {
        return działania;
    }

    public ArrayList<Para<Integer, Integer>> dajOkręgiDoScalenia() {
        return okręgiDoScalenia;
    }

    public void czytajDane() {

        String[] pierwszaLinia = in.nextLine().split(" ");
        this.liczbaOkręgów = Integer.parseInt(pierwszaLinia[0]);
        this.liczbaPartii = Integer.parseInt(pierwszaLinia[1]);
        this.liczbaDziałań = Integer.parseInt(pierwszaLinia[2]);
        this.liczbaCech = Integer.parseInt(pierwszaLinia[3]);

        // 2 wiersz
        czytajOkręgiDoScalenia();

        // 3, 4, 5 wiersz
        czytajITwórzPartie();

        // 6 wiersz
        czytajLiczbęWyborcówWOkręgach();

        // czytanie kandydatów
        czytajKandydatów();

        // czytanie wyborców i tworzenie okręgów
        czytajWyborców();


        // czytanie możliwych działań w kampanii
        czytajDziałania();
    }

    public void czytajWyborców() {
        this.okręgiWBajtocji = new OkręgWyborczy[liczbaOkręgów];
        for (int i = 0; i < liczbaOkręgów; i++) {
            Wyborca[] wyborcy = new Wyborca[liczbaWyborcówWOkręgach[i]];
            for (int j = 0; j < liczbaWyborcówWOkręgach[i]; j++) {
                String[] opisW = in.nextLine().split(" ");  // opisW[0], opisW[1], opisW[2] - imię, nazwisko, numerOkręgu
                String imię = opisW[0];
                String nazwisko = opisW[1];
                int numerOkręgu = Integer.parseInt(opisW[2]);
                int typWyborcy = Integer.parseInt(opisW[3]);
                int[] cechy;
                switch (typWyborcy) {
                    case 1:
                        wyborcy[j] = new ŻelaznyElektoratPartyjny(imię, nazwisko, numerOkręgu, opisW[4]);
                        break;
                    case 2:
                        wyborcy[j] = new ŻelaznyElektoratKandydata(imię, nazwisko, numerOkręgu, opisW[4], Integer.parseInt(opisW[5]));
                        break;
                    case 3:
                        wyborcy[j] = new JednocechowyMin(imię, nazwisko, numerOkręgu, Integer.parseInt(opisW[4]), null);
                        break;
                    case 4:
                        wyborcy[j] = new JednocechowyMaks(imię, nazwisko, numerOkręgu, Integer.parseInt(opisW[4]), null);
                        break;
                    case 5:
                        cechy = new int[liczbaCech];
                        int l = 0;
                        for (int k = 4; k < liczbaCech + 4; k++)
                            cechy[l++] = Integer.parseInt(opisW[k]);
                        wyborcy[j] = new Wszechstronny(imię, nazwisko, numerOkręgu, new Wektor(cechy), null);
                        break;
                    case 6:
                        wyborcy[j] = new JednocechowyMin(imię, nazwisko, numerOkręgu, Integer.parseInt(opisW[4]), opisW[5]);
                        break;
                    case 7:
                        wyborcy[j] = new JednocechowyMaks(imię, nazwisko, numerOkręgu, Integer.parseInt(opisW[4]), opisW[5]);
                        break;
                    case 8:
                        cechy = new int[liczbaCech];
                        int k = 4;
                        for (int m = 0; m < liczbaCech; m++)
                            cechy[m] = Integer.parseInt(opisW[k++]);
                        wyborcy[j] = new Wszechstronny(imię, nazwisko, numerOkręgu, new Wektor(cechy), opisW[k]);
                        break;
                }
            }
            okręgiWBajtocji[i] = new OkręgWyborczy(i + 1, wyborcy, listyWyborczeOkręgów[i]);
        }
    }

    public void czytajITwórzPartie() {
        // trzeci wiersz
        String[] nazwyPartii = in.nextLine().split(" ");

        // czwarty wiersz
        String[] budżetyPartiiString = in.nextLine().split(" ");
        int[] budżetyPartii = new int[liczbaPartii];
        for (int i = 0; i < liczbaPartii; i++)
            budżetyPartii[i] = Integer.parseInt(budżetyPartiiString[i]);

        // piąty wiersz
        String[] strategiePartii = in.nextLine().split(" ");

        //Tworzę partie
        this.partieWBajtocji = new Partia[liczbaPartii];
        for (int i = 0; i < liczbaPartii; i++) {
            switch (strategiePartii[i]) {
                case "R":
                    partieWBajtocji[i] = new PartiaZRozmachem(budżetyPartii[i], nazwyPartii[i]);
                    break;
                case "S":
                    partieWBajtocji[i] = new PartiaSkromna(budżetyPartii[i], nazwyPartii[i]);
                    break;
                case "W":
                    partieWBajtocji[i] = new MojaPartia(budżetyPartii[i], nazwyPartii[i]);
                    break;
                case "Z":
                    partieWBajtocji[i] = new PartiaZachłanna(budżetyPartii[i], nazwyPartii[i]);
                    break;
            }
        }
    }

    public void czytajOkręgiDoScalenia() {
        String[] pary = in.nextLine().split(" ");
        okręgiDoScalenia = new ArrayList<>();

        for (int i = 1; i < pary.length; i++) { //zaczynam od 1, bo pod 0 indeksem jest liczba par do scalenia
            String[] numery = pary[i].split(","); // tablica dwuelementowa, numery[0] = "(<numer>" numery[1] = "<numer>)"
            int numerOkręgu = Integer.parseInt(numery[0].substring(1));
            okręgiDoScalenia.add(new Para(numerOkręgu, numerOkręgu + 1)); //zakładam że dane są poprawne i pary scalanych okręgów są postaci (o, o + 1)
        }
    }

    public void czytajLiczbęWyborcówWOkręgach() {
        String[] liczbaWyborcówWOkręgachString = in.nextLine().split(" ");
        this.liczbaWyborcówWOkręgach = new int[liczbaOkręgów];
        for (int i = 0; i < liczbaOkręgów; i++)
            liczbaWyborcówWOkręgach[i] = Integer.parseInt(liczbaWyborcówWOkręgachString[i]);

    }

    public void czytajKandydatów() {
        this.listyWyborczeOkręgów = new ListaWyborcza[liczbaOkręgów];

        for (int i = 0; i < liczbaOkręgów; i++) {
            // wczytuje kandydatów w danym okręgu
            int liczbaKandydatów = liczbaWyborcówWOkręgach[i] / 10; //liczba kandydatów w (i+1)-tym okręgu
            listyWyborczeOkręgów[i] = new ListaWyborcza();
            for (int j = 0; j < liczbaPartii; j++) {
                //wczytuje kandydatów w kolejnych partiach
                Kandydat[] kandydaciPartii = new Kandydat[liczbaKandydatów];

                for (int k = 0; k < liczbaKandydatów; k++) {
                    String[] opisK = in.nextLine().split(" "); // opis kandydata

                    //tworzę wektor cech kandydata
                    int[] cechy = new int[liczbaCech];
                    for (int l = 0; l < liczbaCech; l++)
                        cechy[l] = Integer.parseInt(opisK[l + 5]);

                    //dodaję kandydata do tablicy kandydatów Partii
                    kandydaciPartii[k] = new Kandydat(opisK[0], opisK[1], Integer.parseInt(opisK[2]), opisK[3], Integer.parseInt(opisK[4]), new Wektor(cechy));
                }

                ListaKandydatów listaPartiiWOkręgu = new ListaKandydatów(kandydaciPartii, partieWBajtocji[j].nazwaPartii());
                listyWyborczeOkręgów[i].dodajListęPartii(listaPartiiWOkręgu);
            }
        }
    }

    public void czytajDziałania() {
        this.działania = new Wektor[liczbaDziałań];
        for (int i = 0; i < liczbaDziałań; i++) {
            int[] zmiany = new int[liczbaCech];
            for (int j = 0; j < liczbaCech; j++) {
                zmiany[j] = in.nextInt();
            }
            działania[i] = new Wektor(zmiany);
        }
    }
}