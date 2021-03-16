package com.company;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.IntBinaryOperator;

public class SumyWierszami {

    private static final int WIERSZE = 10;
    private static final int KOLUMNY = 100;

    private static final Thread[] wątki_kolumnowe = new Thread[KOLUMNY];

    // Klucz: numer wiersza, Wartość: Para <suma wartości kolumn w danym wierszu, liczba przetworzonych kolumn w tym wierszu>
    private static final ConcurrentMap<Integer, Para<AtomicInteger, AtomicInteger>> sumy = new ConcurrentHashMap<>();

    private static class Liczący implements Runnable {

        private IntBinaryOperator macierz;
        private int moja_kolumna;

        public Liczący(IntBinaryOperator macierz, int kolumna) {
            this.macierz = macierz;
            this.moja_kolumna = kolumna;
        }

        @Override
        public void run() {
            for(int w = 0; w < WIERSZE; ++w) {
                sumy.putIfAbsent(w, new Para(new AtomicInteger(), new AtomicInteger()));
                int x = macierz.applyAsInt(w, moja_kolumna);
                Para<AtomicInteger, AtomicInteger> p = sumy.get(w);
                p.pierwszy().getAndAdd(x);

                if(p.drugi().incrementAndGet() == KOLUMNY){
                    System.out.println(w + " " + p.pierwszy());
                }
            }
        }
    }

    private static void piszSumyWierszy(int kolumny, IntBinaryOperator macierz) {
        for(int k = 0; k < kolumny; ++k) {
            wątki_kolumnowe[k] = new Thread(new Liczący(macierz, k));
            wątki_kolumnowe[k].start();
        }
    }

    public static void main(String[] args) {

        piszSumyWierszy(KOLUMNY, (wiersz, kolumna) -> {
            int a = 2 * kolumna + 1;
            return (wiersz + 1) * (a % 4 - 2) * a;
        });

    }

}