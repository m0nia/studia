package multiset;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class Multiset<E> {

    private Map<E, Integer> multiset;

    public Multiset(){
        multiset = new HashMap<>();
    }

    public Multiset(E[] elementy){
        multiset = new HashMap<>();
        for(E element: elementy){
            add(element);
        }
    }

    private Multiset<E> copy(){ //zwraca kopię zbioru
        Multiset<E> copy = new Multiset<>();
        for(E element: this.multiset.keySet()){
            copy.multiset.put(element, this.multiset.get(element));
        }
        return copy;
    }


    public boolean isEmpty() {
        return multiset.isEmpty();
    }

    public void add(E e) {
        if(!contains(e)){
            multiset.put(e, 1);
        } else {
            multiset.put(e, multiset.get(e) + 1);
        }
    }

    public void del(E e) {
        int occurrences;
        if(contains(e)){
            occurrences = multiset.get(e);
            if(occurrences == 1)
                multiset.remove(e);
            else
                multiset.put(e, occurrences - 1);
        }
    }

    public boolean contains(E e) {
        return multiset.containsKey(e);
    }

    public <S extends Collection <E>> Multiset<E> union(S s) {
        Multiset<E> union = this.copy();
        for(E element: s){
            union.add(element);
        }
        return union;
    }

    public <S extends Collection <E>> Multiset<E> intersection(S s){
        Multiset<E> temp = this.copy();
        Multiset<E> intersection = new Multiset<>();
        for(E element: s){
            if(temp.contains(element)){
                intersection.add(element);
                temp.del(element);
            }
        }
        return intersection;
    }

    public <S extends Collection <E>> Multiset<E> difference(S s){
        Multiset<E> difference = this.copy();
        for(E element: s){
            difference.del(element);
        }
        return difference;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("{");
        for(E element: multiset.keySet()){
            for(int i = 0; i < multiset.get(element); i++){
                sb.append(element);
                sb.append(" ");
            }
        }
        sb.append("}");
        return sb.toString();
    }
}
