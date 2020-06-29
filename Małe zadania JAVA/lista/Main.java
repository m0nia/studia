package lista;

import java.util.Iterator;

public class Main
{
    public static void main(String[] args)
    {
        Lista<String> lista = new Lista<String>();
        lista.dodajNaPoczątek("a");
        lista.dodajNaPoczątek("b");
        lista.dodajNaPoczątek("c");
        lista.dodajNaKoniec("d");
        lista.dodajNaKoniec("e");
        lista.dodajNaKoniec("f");
        System.out.println("Od początku:");

        for(String str : lista)
        {
            System.out.print(str + " ");
        }
        System.out.println();
        System.out.println("Od końca:");
        Iterator<String> iterator = lista.iterator(false);
        while(iterator.hasNext())
        {
            System.out.print(iterator.next() + " ");
        }
    }
}