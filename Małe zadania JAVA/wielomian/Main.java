package com.company;

//@author: Monika Michaluk
//numer indeksu: 395135

public class Main {

    public static void main(String[] args) {

        Monomial m1 = new Monomial(2, 2);
        Monomial m2 = new Monomial(3, 3);
        Monomial m3 = new Monomial(0, 6);
        Monomial m4 = new Monomial(10, 0);
        System.out.println(m1);
        System.out.println(m2);
        System.out.println(m3);
        System.out.println(m4);
        System.out.println(m1.compose(m2));
        System.out.println(m1.compose(m3));
        System.out.println(m2.compose(m4));

        Polynomial p1 = new Polynomial(new Monomial[]{m2, m1, m4});
        System.out.println(p1);

        Monomial m5 = new Monomial(2, 1);
        Monomial m6 = new Monomial(1, 0);
        Polynomial p2 = new Polynomial(new Monomial[]{m5, m6});
        System.out.println(p1.plus(p2));
        System.out.println(p1.minus(p2));
        System.out.println(p1.times(p2));
        System.out.println(p1.compose(p2));
        Polynomial p3 = new Polynomial(new Monomial[]{m1, m4});
        System.out.println(p3.compose(p2));

    }
}
