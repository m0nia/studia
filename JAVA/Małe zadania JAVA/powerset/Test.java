package powerset;

import java.util.*;

public class Test {
    public static void main(String args[]) {
        ArrayList<Integer> l = new ArrayList<Integer>();
        l.add(5); l.add(8); l.add(9); l.add(10); l.add(12);
        PowerArrayListImpl<Integer> p = new PowerArrayListImpl<Integer>(l);
        for(Iterator<ArrayList<Integer>> it = p.iterator(); it.hasNext(); ) {
            System.out.print("{ ");
            for(Integer x : it.next()) {
                System.out.print(x+" ");
            }
            System.out.println("}");
        }
    }
}