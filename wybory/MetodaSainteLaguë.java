package wybory;

public class MetodaSainteLaguë extends MetodaDzielników {

    public MetodaSainteLaguë() {
        poprzedniDzielnik = -1;
    }

    @Override
    protected void zacznijGenerować() {
        poprzedniDzielnik = -1;
    }

    @Override
    protected int policzNastępnyDzielnik() {
        int następny = poprzedniDzielnik + 2;
        poprzedniDzielnik = następny;
        return następny;
    }

    @Override
    public String toString() {
        return "Metoda Sainte-Laguë";
    }
}
