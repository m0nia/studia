package wybory;

public class JednocechowyMin extends Minimalizujący {

    private int minimalizowanaCecha;

    public JednocechowyMin(String imię, String nazwisko, int numerOkręgu, int minimalizowanaCecha, String ulubionaPartia) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
        this.minimalizowanaCecha = minimalizowanaCecha;
    }

    @Override
    protected int oceńKandydata(Kandydat k) {
        return k.cecha(minimalizowanaCecha);
    }

    @Override
    protected void zmieńOpinię(Wektor działanie) {
        //nie zmienia Opinii
    }
}
