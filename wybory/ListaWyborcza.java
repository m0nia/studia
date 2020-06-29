package wybory;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.NoSuchElementException;

public class ListaWyborcza implements Iterable<Kandydat> {

    private ArrayList<ListaKandydatów> listyPartii;

    public ListaWyborcza() {
        listyPartii = new ArrayList<>();
    }

    public ListaWyborcza(ListaWyborcza listaWyborcza) { //konstruktor kopiujący
        listyPartii = new ArrayList<>();
        for (ListaKandydatów kandydaci : listaWyborcza.listyPartii) {
            listyPartii.add(new ListaKandydatów(kandydaci));
        }
    }

    public void dodajListęPartii(ListaKandydatów lista) {
        listyPartii.add(lista);
    }

    public ListaKandydatów dajListęPartii(String partia) {

        for (ListaKandydatów lista : listyPartii) {
            if (lista.partia().equals(partia))
                return lista;
        }
        return null;
    }

    public ListaWyborcza scal(ListaWyborcza lista) {
        ListaWyborcza nowaListaWyborcza = new ListaWyborcza();

        for (ListaKandydatów l1 : listyPartii) {
            String partia = l1.partia();
            ListaKandydatów l2 = lista.dajListęPartii(partia);
            nowaListaWyborcza.dodajListęPartii(l1.scal(l2));
        }

        return nowaListaWyborcza;
    }

    @Override
    public Iterator<Kandydat> iterator() {
        return new ListaWyborczaIterator(listyPartii);
    }
}

class ListaWyborczaIterator implements Iterator<Kandydat> {

    private ArrayList<ListaKandydatów> listyPartii;

    private int aktualnaListaIndeks;

    private Iterator<Kandydat> listaKandydatów;

    ListaWyborczaIterator(ArrayList<ListaKandydatów> listyPartii) {
        this.listyPartii = listyPartii;
        aktualnaListaIndeks = 0;
        listaKandydatów = listyPartii.get(0).iterator();
    }

    @Override
    public boolean hasNext() {
        return listaKandydatów.hasNext() || aktualnaListaIndeks + 1 < listyPartii.size();
    }

    @Override
    public Kandydat next() {
        if (!hasNext())
            throw new NoSuchElementException();

        if (!listaKandydatów.hasNext()) {
            aktualnaListaIndeks++;
            listaKandydatów = listyPartii.get(aktualnaListaIndeks).iterator();
        }

        return listaKandydatów.next();
    }
}