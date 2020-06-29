package wyrażenia;

public class Plus extends Wyr2Arg {

    public Plus(Wyrażenie lewe, Wyrażenie prawe) {
        super(lewe, prawe);
    }

    @Override
    public double policz(double x) {
        return lewe.policz(x) + prawe.policz(x);
    }

    @Override
    public Wyrażenie pochodna() {
        return new Plus(lewe.pochodna(), prawe.pochodna());
    }

    @Override
    protected int priorytet() {
        return 1;
    }

    @Override
    protected String operator() { // znowu - private czy protected? chyba gdyby był private to niebyłby widoczny w nadklasie
        return "+";
    }
}
