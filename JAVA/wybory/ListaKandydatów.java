package wybory;

import java.util.Arrays;
import java.util.Iterator;

public class ListaKandydatów implements Iterable<Kandydat> { //TODO: CZy lista kandydatów powinna mieć numerOkręgu? jeśli tak, to jaki dla okręgów scalonych?

    private Kandydat[] kandydaci;

    private String nazwaPartii;

    public ListaKandydatów(Kandydat[] kandydaci, String nazwaPartii) {
        this.kandydaci = kandydaci;
        this.nazwaPartii = nazwaPartii;
    }

    public ListaKandydatów(ListaKandydatów lista) { //konstruktor kopiujący
        this.kandydaci = new Kandydat[lista.kandydaci.length];
        System.arraycopy(lista.kandydaci, 0, this.kandydaci, 0, this.kandydaci.length);
        this.nazwaPartii = lista.nazwaPartii;
    }

    public String partia() {
        return nazwaPartii;
    }

    public ListaKandydatów scal(ListaKandydatów lista) { //scala dwie listy tej samej partii z różnych okręgów

        Kandydat[] nowaLista = new Kandydat[kandydaci.length + lista.kandydaci.length];
        int k = 0;
        for (int i = 0; i < kandydaci.length; i++)
            nowaLista[k++] = kandydaci[i];

        for (int i = 0; i < lista.kandydaci.length; i++)
            nowaLista[k++] = lista.kandydaci[i];

        return new ListaKandydatów(nowaLista, nazwaPartii);
    }

    @Override
    public Iterator<Kandydat> iterator() {
        return Arrays.asList(kandydaci).iterator();
    }
}
