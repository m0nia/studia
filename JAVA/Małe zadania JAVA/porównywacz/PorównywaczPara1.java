package tablica;

public class PorównywaczPara1<S, T> implements Porównywacz<Para<S, T>>{

    private Porównywacz<S> pS;
    private Porównywacz<T> pT;

    public PorównywaczPara1(Porównywacz<S> pS, Porównywacz<T> pT){
        this.pS = pS;
        this.pT = pT;
    }

    @Override
    public boolean porównaj(Para<S, T> a, Para<S, T> b) {
        if(pS.porównaj(a.pierwszy(), b.pierwszy())){
            return true;
        } else if(pS.porównaj(b.pierwszy(), a.pierwszy())){
            return false;
        } else { //pierwsze elementy w parze są równe
            if(pT.porównaj(a.drugi(), b.drugi()))
                return true;
            return false;
        }
    }
}
