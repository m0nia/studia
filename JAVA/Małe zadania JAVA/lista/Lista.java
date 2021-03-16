package lista;

import java.util.Iterator;

public class Lista <T> implements Iterable<T> {
    private ElementListy<T> początek;
    private ElementListy<T> koniec;

    public Lista(){
        początek = null;
        koniec = null;
    }

    public void dodajNaPoczątek(T element){
        ElementListy<T> nowy = new ElementListy<>(element, początek);
        if(początek == null){
            początek = koniec = nowy;
        } else {
            początek = nowy;
        }
    }

    public void dodajNaKoniec(T element){
        ElementListy<T> nowy = new ElementListy<>(element, null);
        if(koniec == null){
            początek = koniec = nowy;
        } else {
            koniec.następny(nowy);
            koniec = nowy;
        }
    }

    private Lista<T> odwróć(){
        Lista<T> odwrócona = new Lista<>();
        for(T element: this){
            odwrócona.dodajNaPoczątek(element);
        }
        return odwrócona;
    }

    @Override
    public Iterator<T> iterator() { //domyślnie od początku
        return new IteratorLista<T>(początek);
    }

    public Iterator<T> iterator(boolean czyOdPoczątku) {
        if(czyOdPoczątku)
            return iterator();
        return odwróć().iterator();
    }
}

class ElementListy <T> {
    private T element;
    private ElementListy<T> następny;

    ElementListy(T element, ElementListy<T> następny){
        this.element = element;
        this.następny = następny;
    }

    public void następny(ElementListy<T> następny){
        this.następny = następny;
    }

    public ElementListy<T> następny(){
        return następny;
    }

    public T wartość(){
        return element;
    }

}