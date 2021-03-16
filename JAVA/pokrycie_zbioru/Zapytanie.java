package cover;

public class Zapytanie {

    private ZbiórPokrywany Z;
    private int strategia;

    public Zapytanie(){
        Z = null;
        strategia = 0;
    }

    public void setZapytanie(int rozmiar, int strategia){
        Z = new ZbiórPokrywany(rozmiar);
        this.strategia = strategia;
    }

    public int dajStrategię(){
        return strategia;
    }

    public ZbiórPokrywany dajZbiórPokrywany(){
        return Z;
    }


}
