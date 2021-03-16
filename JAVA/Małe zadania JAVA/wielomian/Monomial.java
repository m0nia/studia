package com.company;

public class Monomial {
    private double coefficient; // współczynnik przy x^n
    private int degree; // potęga do której podnosimy zmienną x

    public Monomial(double coefficient, int degree) {
        this.degree = degree;
        this.coefficient = coefficient;
    }

    //Tylko jeśli potęgi dwóch jednomianów są równe
    Monomial plus(Monomial m) {
        if (this.degree != m.degree)
            return null;
        return new Monomial(this.coefficient + m.coefficient, this.degree);
    }

    double getCoefficient() {
        return this.coefficient;
    }

    int getDegree() {
        return this.degree;
    }

    Monomial times(Monomial m) {
        int new_power = this.degree + m.degree;
        double new_coefficient = this.coefficient * m.coefficient;

        return new Monomial(new_coefficient, new_power);
    }

    Monomial compose(Monomial m) {
        int new_power = m.degree * this.degree;
        double new_coefficient = this.coefficient * Math.pow(m.coefficient, this.degree);
        return new Monomial(new_coefficient, new_power);
    }

    double evaluate(int x) {
        return this.coefficient * Math.pow(x, this.degree);
    }

    public String toString() {
        if (this.coefficient == 0)
            return 0 + "";
        if (this.degree == 0)
            return this.coefficient + "";
        return this.coefficient + "*x^" + this.degree;
    }
}
