package tablica;

public class Tablica <T, P extends Porównywacz<T> > {

    public void sortuj(T[] tablica, P porównywacz){
        T temp;

        for (int i = 0; i < tablica.length; ++i)
            for (int j = 0; j < i; ++j)
                if (porównywacz.porównaj(tablica[i], tablica[j])) { //==true if tablica[i] < tablica[j]
                    temp = tablica[i];
                    tablica[i] = tablica[j];
                    tablica[j] = temp;
                }
    }

    public void wypisz(T[] tablica){
        for(int i = 0; i < tablica.length; i++) {
            System.out.print(tablica[i]+" ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        Tablica test = new Tablica();

        Integer[] tab1 = {4, 5, 6, 10, 2, 13, 7};
        System.out.print("Przed sortowaniem: ");
        test.wypisz(tab1);
        test.sortuj(tab1, new PorównywaczInt());
        System.out.print("Po sortowaniu: ");
        test.wypisz(tab1);

        String[] tab2 = {"Ala", "Basia", "Ania", "Kamila", "Beata"};
        System.out.print("Przed sortowaniem: ");
        test.wypisz(tab2);
        test.sortuj(tab2, new PorównywaczString());
        System.out.print("Po sortowaniu: ");
        test.wypisz(tab2);

        Para p1 = new Para<Integer, String>(1, "Kasia");
        Para p2 = new Para<Integer, String>(2, "Ania");
        Para p3 = new Para<Integer, String>(3, "Beata");
        Para p4 = new Para<Integer, String>(4, "Ala");

        Para<?, ?>[] tab3 = {p2,p4,p3,p1};
        System.out.print("Przed sortowaniem: ");
        test.wypisz(tab3);
        System.out.print("Sortowanie po pierwszej współrzędnej: ");
        test.sortuj(tab3, new PorównywaczPara1(new PorównywaczInt(), new PorównywaczString()));
        test.wypisz(tab3);
        System.out.print("Sortowanie po drugiej współrzędnej: ");
        test.sortuj(tab3, new PorównywaczPara2(new PorównywaczInt(), new PorównywaczString()));
        test.wypisz(tab3);

    }
}
