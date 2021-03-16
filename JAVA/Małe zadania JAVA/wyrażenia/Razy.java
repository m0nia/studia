package wyrażenia;

public class Razy extends Wyr2Arg {

    public Razy(Wyrażenie lewe, Wyrażenie prawe) {
        super(lewe, prawe);
    }

    @Override
    public double policz(double x) {
        return lewe.policz(x) * prawe.policz(x);
    }

    @Override
    public Wyrażenie pochodna() {
        return new Plus(new Razy(lewe, prawe.pochodna()), new Razy(lewe.pochodna(), prawe));
    }

    @Override
    protected int priorytet() {
        return 2;
    }

    @Override
    protected String operator() {
        return "*";
    }
}
