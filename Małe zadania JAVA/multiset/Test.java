package multiset;

import java.util.ArrayList;
import java.util.Arrays;

public class Test {

    public static void main(String[] args) {
        Integer[] arr1 = {2, 3, 4, 5, 6, 7, 8, 2, 2, 4, 4, 4, 6};
        Multiset<Integer> z1 = new Multiset<>(arr1);

        Multiset<Integer> z2 = new Multiset<>();
        Integer[] arr2 = {2, 2, 2, 4, 4, 6, 9, 9};
        for (int x : arr2)
            z2.add(x);

        System.out.println("z1 = "+z1);
        System.out.println("z2 = "+z2);


        ArrayList<Integer> s1 = new ArrayList<>(Arrays.asList(arr2));
        ArrayList<Integer> s2 = new ArrayList<>(Arrays.asList(arr1));

        System.out.println("Union:");
        System.out.println(z1.union(s1));
        System.out.println(z2.union(s2));

        System.out.println("Intersection:");
        System.out.println(z1.intersection(s1));
        System.out.println(z2.intersection(s2));

        System.out.println("Difference:");
        System.out.println(z1.difference(s1));
        System.out.println(z2.difference(s2));

    }
}
