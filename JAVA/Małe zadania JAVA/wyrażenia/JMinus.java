package wyrażenia;

public class JMinus extends Wyr1Arg {

    public JMinus(Wyrażenie arg) {
        this.arg = arg;
    }

    @Override
    public double policz(double x) {
        return (-1) * arg.policz(x);
    }

    @Override
    public Wyrażenie pochodna() {
        return new JMinus(arg.pochodna());
    }

    @Override
    protected String nazwa() {
        return "-";
    }
}
