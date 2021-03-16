package wybory;

public class Wszechstronny extends Maksymalizujący {
    private Wektor wagiCech;

    public Wszechstronny(String imię, String nazwisko, int numerOkręgu, Wektor wagiCech, String ulubionaPartia) {
        super(imię, nazwisko, ulubionaPartia, numerOkręgu);
        this.wagiCech = wagiCech;
    }

    @Override
    protected int oceńKandydata(Kandydat k) {
        if (k == null) System.out.println("nulllll");
        return wagiCech.iloczynSkalarny(k.cechy());
    }

    @Override
    protected void zmieńOpinię(Wektor działanie) {

        wagiCech.dodaj(działanie);

    }

    @Override
    public boolean czyZmieniaszOpinię() {
        return true;
    }
}
