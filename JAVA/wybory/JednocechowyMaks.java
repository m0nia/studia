package wybory;

public class JednocechowyMaks extends Maksymalizujący {

    private int maksymalizowanaCecha;

    public JednocechowyMaks(String imię, String nazwisko, int numerOkręgu, int maksymalizowanaCecha, String ulubionaPartia) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
        this.maksymalizowanaCecha = maksymalizowanaCecha;
    }

    @Override
    protected int oceńKandydata(Kandydat k) {
        return k.cecha(maksymalizowanaCecha);
    }

    @Override
    protected void zmieńOpinię(Wektor działanie) {
        // nie zmienia opinii
    }
}
