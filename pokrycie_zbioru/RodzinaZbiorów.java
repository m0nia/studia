package cover;

import java.util.ArrayList;

public class RodzinaZbiorów {

    ArrayList<ZbiórZłożony> R;

    public RodzinaZbiorów(){
        R = new ArrayList<>();
    }

    public void dodaj(ZbiórZłożony Z){
        R.add(Z);
    }

    public ZbiórZłożony daj(int indeks){
        return R.get(indeks);
    }

    public int rozmiar(){
        return R.size();
    }

}
