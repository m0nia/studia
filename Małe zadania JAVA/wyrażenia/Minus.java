package wyrażenia;

public class Minus extends Wyr2Arg {

    public Minus(Wyrażenie lewe, Wyrażenie prawe){
        super(lewe, prawe);
    }

    @Override
    public double policz(double x) {
        return lewe.policz(x) - prawe.policz(x);
    }

    @Override
    public Wyrażenie pochodna() {
        return new Minus(lewe.pochodna(), prawe.pochodna());
    }

    @Override
    protected int priorytet() {
        return 1;
    }

    @Override
    protected String operator() {
        return "-";
    }
}
