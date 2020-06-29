package wyrażenia;

public class Stała extends Wyrażenie {

    private double wartość;

    public Stała(double x) {
        this.wartość = x;
    }

    @Override
    public double policz(double x) {
        return wartość;
    }

    @Override
    public Wyrażenie pochodna() {
        return new Stała(0);
    }

    @Override
    protected int priorytet() {
        return Integer.MAX_VALUE; // Nigdy nie otaczamy nawiasami stałej
    }

    @Override
    public String toString() {
        return wartość + "";
    }
}
