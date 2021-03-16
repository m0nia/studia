package wybory;

public class Głos {

    private String imięWyborcy;

    private String nazwiskoWyborcy;

    private Kandydat naKogo;

    public Głos(String imięWyborcy, String nazwiskoWyborcy, Kandydat naKogo) {
        this.imięWyborcy = imięWyborcy;
        this.nazwiskoWyborcy = nazwiskoWyborcy;
        this.naKogo = naKogo;
    }

    public Kandydat naKogo() {
        return naKogo;
    }

    @Override
    public String toString() {
        return imięWyborcy + " " + nazwiskoWyborcy + " głosował(a) na : " + naKogo;
    }
}
