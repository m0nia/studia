package com.company;

import java.util.ArrayList;

public class Polynomial {

    //Tablica jednomianów uporządkowana malejąco według potęg i w postaci "nieskracalnej", tzn jednomian danej potęgi
    // występuje co najwyżej raz w tablicy i nie ma zer w tablicy

    private Monomial[] poly;

    //Tworzy wielomian stopnia zerowego o współczynniku równym n.
    public Polynomial(double n) {
        this.poly = new Monomial[]{new Monomial(n, 0)};
    }

    // Z tablicy jednomianów tworzy wielomian
    // Tablica musi spełniać warunki opisane przy atrybucie poly
    public Polynomial(Monomial[] poly) {
        this.poly = new Monomial[poly.length];
        for (int i = 0; i < poly.length; i++)
            this.poly[i] = poly[i];
    }

    public Polynomial plus(Polynomial p) {
        ArrayList<Monomial> new_poly = new ArrayList<Monomial>();
        Monomial m;
        int i, j;
        i = j = 0;
        int degree1, degree2;
        while (i < this.poly.length && j < p.poly.length) {

            degree1 = this.poly[i].getDegree();
            degree2 = p.poly[j].getDegree();

            if (degree1 == degree2) {
                m = this.poly[i].plus(p.poly[j]);
                if (m.getCoefficient() != 0)
                    new_poly.add(m);
                i++;
                j++;
            } else if (degree1 > degree2) {
                new_poly.add(this.poly[i]);
                i++;
            } else {
                new_poly.add(p.poly[j]);
                j++;
            }
        }
        for (int k = i; k < this.poly.length; k++)
            new_poly.add(this.poly[k]);

        for (int k = j; k < p.poly.length; k++)
            new_poly.add(p.poly[k]);

        Monomial[] monomials = new Monomial[new_poly.size()];
        monomials = new_poly.toArray(monomials);

        return new Polynomial(monomials);
    }

    public Polynomial minus(Polynomial p) {
        Monomial[] opposite = new Monomial[p.poly.length];
        for (int i = 0; i < p.poly.length; i++)
            opposite[i] = new Monomial(p.poly[i].getCoefficient() * (-1), p.poly[i].getDegree());
        return this.plus(new Polynomial(opposite));
    }

    public Polynomial times(Polynomial p) {
        Polynomial new_poly = new Polynomial(0);
        Monomial[] mult = new Monomial[p.poly.length];
        int i;

        for (Monomial m1 : this.poly) {
            i = 0;
            for (Monomial m2 : p.poly) {
                mult[i] = m1.times(m2);
                i++;
            }
            new_poly = new_poly.plus(new Polynomial(mult));
        }

        return new_poly;
    }

    private Polynomial raiseToPower(int n) {

        Polynomial p = new Polynomial(1);
        for (int i = 0; i < n; i++) {
            p = p.times(this);
        }
        return p;
    }

    public Polynomial compose(Polynomial p) {
        Polynomial new_poly = new Polynomial(0);
        Polynomial temp;
        for (Monomial m1 : this.poly) {
            temp = p.raiseToPower(m1.getDegree()).times(new Polynomial(m1.getCoefficient()));
            new_poly = new_poly.plus(temp);
        }
        return new_poly;
    }


    public boolean equals(Polynomial p) {
        int i = 0;
        while (this.poly[i] == p.poly[i])
            i++;
        if (i == this.poly.length && i == p.poly.length)
            return true;
        return false;
    }

    public double evaluate(int x) {
        double value = 0;
        for (Monomial m : this.poly) {
            value += m.evaluate(x);
        }
        return value;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (Monomial m : this.poly) {
            sb.append(m.toString());
            sb.append(" + ");
        }
        sb.delete(sb.length() - 3, sb.length() - 1);
        return sb.toString();
    }
}

