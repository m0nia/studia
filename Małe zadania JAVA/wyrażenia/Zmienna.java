package wyrażenia;

public class Zmienna extends Wyrażenie {

    //TODO : SINGLETON

    public Zmienna(){}

    @Override
    public double policz(double x){
        return x;
    }

    @Override
    public Wyrażenie pochodna(){
        return new Stała(1);
    }

    protected int priorytet(){
        return Integer.MAX_VALUE; // Nigdy nie otaczamy nawiasami zmiennej
    }

    @Override
    public String toString(){
        return "x";
    }
}
