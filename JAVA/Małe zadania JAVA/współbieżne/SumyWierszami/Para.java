package com.company;

import java.util.concurrent.atomic.AtomicInteger;

public class Para<T1, T2> {

    private T1 pierwszy;
    private T2 drugi;

    public Para(T1 pierwszy, T2 drugi) {
        this.pierwszy = pierwszy;
        this.drugi = drugi;
    }

    public T1 pierwszy(){
        return pierwszy;
    }

    public T2 drugi(){
        return drugi;
    }

}
