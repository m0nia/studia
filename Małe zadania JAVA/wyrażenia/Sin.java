package wyrażenia;

public class Sin extends Wyr1Arg {

    public Sin(Wyrażenie arg) {
        this.arg = arg;
    }

    @Override
    public double policz(double x) {
        return Math.sin(arg.policz(x));
    }

    @Override
    public Wyrażenie pochodna() {
        return new Razy(new Cos(arg), arg.pochodna());
    }

    @Override
    protected String nazwa() {
        return "sin";
    }
}
