package wyrażenia;

public abstract class Wyrażenie {

    public abstract double policz(double x);

    public abstract Wyrażenie pochodna();

    protected abstract int priorytet();

    public double całka(double a, double b, int n) {
        double x_1, y_1, y_2;
        double x_2 = a;
        double całka = 0;
        double d = (b - a) / n;

        while (x_2 <= b) {
            x_1 = x_2;
            x_2 += d;
            y_1 = policz(x_1);
            y_2 = policz(x_2);
            całka += (y_1 + y_2) / 2 * d;
        }
        return całka;
    }
}
