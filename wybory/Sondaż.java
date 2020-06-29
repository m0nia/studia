package wybory;

public class Sondaż {

    private Wyborca[] wyborcy;

    public Sondaż(Wyborca[] wyborcy) {
        this.wyborcy = wyborcy;
    }

    public int ileWyborcówZmieniaOpinię() {
        int licznik = 0;
        for (Wyborca w : wyborcy) {
            if (w.czyZmieniaszOpinię())
                licznik++;
        }
        return licznik;
    }

    public int ileWyborcówPartyjnych() {
        int licznik = 0;
        for (Wyborca w : wyborcy) {
            if (w.ulubionaPartia() != null)
                licznik++;
        }
        return licznik;
    }

}
