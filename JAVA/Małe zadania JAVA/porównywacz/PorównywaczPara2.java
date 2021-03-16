package tablica;

public class PorównywaczPara2 <S, T> implements Porównywacz<Para<S, T>>{

    private Porównywacz<S> pS;
    private Porównywacz<T> pT;

    public PorównywaczPara2(Porównywacz<S> pS, Porównywacz<T> pT){
        this.pS = pS;
        this.pT = pT;
    }

    @Override
    public boolean porównaj(Para<S, T> a, Para<S, T> b) {
        PorównywaczPara1<T, S> p = new PorównywaczPara1<T, S>(pT, pS);
        Para p1 = new Para(a.drugi(), a.pierwszy());
        Para p2 = new Para(b.drugi(), b.pierwszy());
        return p.porównaj(p1, p2);
    }

}
