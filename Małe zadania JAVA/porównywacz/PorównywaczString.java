package tablica;

public class PorównywaczString implements Porównywacz<String> {

    @Override
    public boolean porównaj(String a, String b) {
        if(a.compareTo(b) < 0)
            return true;
        return false;
    }
}
