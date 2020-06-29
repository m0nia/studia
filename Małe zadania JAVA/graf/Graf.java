package graf;

import java.util.*;


public class Graf {

    Map<String, Set<String>> graf = new TreeMap<String, Set<String>>();

    private Iterator<String> przechodźWgłąb(String w) {
        return new IteratorDFS(w);
    }

    private Iterator<String> przechodźWszerz(String w) {
        return new IteratorBFS(w);
    }

    private Set<String> zbiórWierzchołkow() {
        return graf.keySet();
    }

    private void połącz(String w1, String w2) { // dodaje krawędź pomiędzy wierzchołkami w1 i w2 w grafie
        if(w1 == null || w2 == null)
            throw new IllegalArgumentException("Wierzchołki nie mogą być nullem.");

        if(!graf.containsKey(w1)) {
            throw new IllegalArgumentException("Wierzchołka ("+w1+") nie ma w grafie");
        }
        if(!graf.containsKey(w2)) {
            throw new IllegalArgumentException("Wierzchołka ("+w1+") nie ma w grafie");
        }

        Set<String> zbiórKrawędzi1 = graf.get(w1);
        Set<String> zbiórKrawędzi2 = graf.get(w2);
        zbiórKrawędzi1.add(w2);
        zbiórKrawędzi2.add(w1);
    }

    private void dodajWierzchołek(String w) { // dodaje wierzchołek w do zbioru wierzchołków w grafie
        if(w == null)
            throw new IllegalArgumentException("Wierzchołek nie może być nullem");

        if(!graf.containsKey(w)){
            graf.put(w, new TreeSet<>());
        }
    }

    private class IteratorBFS implements Iterator<String> {

        private Queue<String> queue;

        private Set<String> visited;

        IteratorBFS(String w){
            queue = new LinkedList<>();
            visited = new HashSet<>();
            queue.add(w);
            visited.add(w);
        }

        @Override
        public boolean hasNext() {
            return !queue.isEmpty();
        }

        @Override
        public String next() {
            if(!hasNext())
                throw new NoSuchElementException();
            String aktualny = queue.remove();

            Set<String> sąsiedzi = graf.get(aktualny);
            for(String w: sąsiedzi){
                if(!visited.contains(w)) {
                    queue.add(w);
                    visited.add(w);
                }
            }
            return aktualny;
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

    private class IteratorDFS implements Iterator<String> {

        private Stack<String> stos;

        private Set<String> visited;

        IteratorDFS(String w){
            stos = new Stack<>();
            visited = new HashSet<>();
            stos.push(w);
            visited.add(w);
        }

        @Override
        public boolean hasNext() {
            return !stos.empty();
        }

        @Override
        public String next() {
            if(!hasNext())
                throw new NoSuchElementException();

            String aktualny = stos.pop();
            Set<String> sąsiedzi = graf.get(aktualny);
            for(String w: sąsiedzi){
                if(!visited.contains(w)) {
                    stos.push(w);
                    visited.add(w);
                }
            }
            return aktualny;
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }
    }

    public static void main(String[] args) {
        Graf graf = new Graf();
        graf.dodajWierzchołek("Kasia");
        graf.dodajWierzchołek("Zosia");
        graf.dodajWierzchołek("Ola");
        graf.dodajWierzchołek("Marysia");
        graf.połącz("Kasia", "Zosia");
        graf.połącz("Zosia", "Marysia");
        graf.połącz("Zosia", "Ola");

        System.out.println(graf.zbiórWierzchołkow());

        System.out.println("Wszerz:");
        Iterator<String> iterator1 = graf.przechodźWszerz("Kasia");
        while (iterator1.hasNext()) {
            System.out.print(iterator1.next() + " ");
        }
        System.out.println("");
        System.out.println("W glab:");
        Iterator<String> iterator2 = graf.przechodźWgłąb("Kasia");
        while (iterator2.hasNext()) {
            System.out.print(iterator2.next() + " ");
        }
    }
}