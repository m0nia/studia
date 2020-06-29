package cover;

public abstract class ZbiórProsty extends ZbiórPokrywający{
}

class Jednoelementowy extends ZbiórProsty{
    private int wartość;

    public Jednoelementowy(int wartość){
        this.wartość = wartość;
    }

    public boolean zawiera(int x){
        return wartość == x;
    }

}

class ArytmetycznySkończony extends ZbiórProsty{
    private int a;
    private int r;
    private int max;

    public ArytmetycznySkończony(int a, int r, int max){
        this.a = a;
        this.r = r;
        this.max = max;
    }

    public boolean zawiera(int x){
        if(x >= a && x <= max && (x - a) % r == 0)
            return true;
        return false;
    }

}

class ArytmetycznyNieskończony extends ZbiórProsty{
    private int a;
    private int r;

    public ArytmetycznyNieskończony(int a, int r){
        this.a = a;
        this.r = r;
    }

    public boolean zawiera(int x){
        if(x >= a && (x - a) % r == 0)
            return true;
        return false;
    }

}