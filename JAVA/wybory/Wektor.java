package wybory;

public class Wektor {

    private int[] wektor;

    public Wektor(int[] wektor) {
        this.wektor = wektor;
    }

    public Wektor(Wektor w) {
        int[] kopia = new int[w.wektor.length];
        System.arraycopy(w.wektor, 0, kopia, 0, w.wektor.length);
        this.wektor = kopia;
    }

    public int iloczynSkalarny(Wektor w) {
        int iloczyn = 0;
        for (int i = 0; i < wektor.length; i++)
            iloczyn += wektor[i] * w.wektor[i];
        return iloczyn;
    }

    public int sumaModułówWspółrzędnych() {
        int suma = 0;
        for (int x : wektor) {
            suma += Math.abs(x);
        }
        return suma;
    }

    public int wartość(int indeks) {
        return wektor[indeks];
    }


    public void dodaj(Wektor działanie) {
        for (int i = 0; i < wektor.length; i++) {
            if (wektor[i] + działanie.wektor[i] > 100)
                wektor[i] = 100;
            else if (wektor[i] + działanie.wektor[i] < -100)
                wektor[i] = -100;
            else
                wektor[i] += działanie.wektor[i];
        }
    }
}
