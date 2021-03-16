package wyrażenia;

public class Log extends Wyr1Arg {

    public Log(Wyrażenie arg) {
        this.arg = arg;
    }

    @Override
    public double policz(double x) {
        return Math.log(arg.policz(x));
    }

    @Override
    public Wyrażenie pochodna() {
        Wyrażenie p1 = new Podzielić(new Stała(1), arg);
        return new Razy(p1, arg.pochodna());
    }

    @Override
    protected String nazwa() {
        return "log";
    }
}
