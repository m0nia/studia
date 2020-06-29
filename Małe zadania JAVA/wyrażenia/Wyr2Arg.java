package wyrażenia;

public abstract class Wyr2Arg extends Wyrażenie{

    protected Wyrażenie lewe;

    protected Wyrażenie prawe;

    public Wyr2Arg(Wyrażenie lewe, Wyrażenie prawe){
        this.lewe = lewe;
        this.prawe = prawe;
    }

    protected abstract String operator();

    @Override
    public String toString() {
        String lewyStr = lewe.toString();
        String prawyStr = prawe.toString();

        if(priorytet() > lewe.priorytet())
            lewyStr = "("+lewyStr+")";

        if(priorytet() > prawe.priorytet())
            prawyStr = "("+prawyStr+")";

        return lewyStr+operator()+prawyStr;
    }
}

