package wyrażenia;

public class Podzielić extends Wyr2Arg {

    public Podzielić(Wyrażenie lewe, Wyrażenie prawe) {
        super(lewe, prawe);
    }

    @Override
    public double policz(double x) {
        return lewe.policz(x) / prawe.policz(x);
    }

    @Override
    public Wyrażenie pochodna() {
        Wyrażenie licznik = new Minus(new Razy(lewe.pochodna(), prawe), new Razy(lewe, prawe.pochodna()));
        Wyrażenie mianownik = new Razy(prawe, prawe);
        return new Podzielić(licznik, mianownik);
    }

    @Override
    protected int priorytet() {
        return 2;
    }

    @Override
    protected String operator() {
        return "÷";
    }

    @Override
    public String toString() {
        String lewyStr = lewe.toString();
        String prawyStr = prawe.toString();

        if(priorytet() > lewe.priorytet())
            lewyStr = "("+lewyStr+")";

        if(priorytet() >= prawe.priorytet()) // Zmiana w stosunku do reszty ponieważ chcemy 2*3/(4*3) a nie 2*3/4*3 co jest niejednoznaczne
            prawyStr = "("+prawyStr+")";

        return lewyStr+operator()+prawyStr;
    }
}
