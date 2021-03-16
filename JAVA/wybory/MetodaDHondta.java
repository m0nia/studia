package wybory;

public class MetodaDHondta extends MetodaDzielników {

    public MetodaDHondta() {
        poprzedniDzielnik = 0;
    }

    @Override
    protected void zacznijGenerować() {
        poprzedniDzielnik = 0;
    }

    @Override
    protected int policzNastępnyDzielnik() {
        int następny = poprzedniDzielnik + 1;
        poprzedniDzielnik = następny;
        return następny;
    }

    @Override
    public String toString() {
        return "Metoda D’Hondta";
    }
}
