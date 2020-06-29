package lista;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class IteratorLista<E> implements Iterator<E> {

    private ElementListy<E> aktualny;

    public IteratorLista(ElementListy<E> początek){
        aktualny = początek;
    }

    @Override
    public boolean hasNext() {
        return aktualny != null;
    }

    @Override
    public E next() {
        if(!hasNext())
            throw new NoSuchElementException();
        E ret = aktualny.wartość();
        aktualny = aktualny.następny();
        return ret;
    }
}