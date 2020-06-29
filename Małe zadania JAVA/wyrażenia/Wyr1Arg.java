package wyrażenia;

public abstract class Wyr1Arg extends Wyrażenie {

    protected Wyrażenie arg;

    protected int priorytet() {
        return Integer.MAX_VALUE;
    }

    abstract protected String nazwa();

    @Override
    public String toString() {
        String argStr = arg.toString();
        if (arg.priorytet() <= priorytet())
            argStr = "(" + argStr + ")";
        return nazwa() + argStr;
    }
}

