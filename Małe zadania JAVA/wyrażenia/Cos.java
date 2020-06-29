package wyrażenia;

public class Cos extends Wyr1Arg {

    public Cos(Wyrażenie arg) {
        this.arg = arg;
    }

    @Override
    public double policz(double x) {
        return Math.cos(arg.policz(x));
    }

    @Override
    public Wyrażenie pochodna() {
        return new Razy(new JMinus(new Sin(arg)), arg.pochodna());
    }

    @Override
    public String nazwa() {
        return "cos";
    }
}
