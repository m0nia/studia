package com.company;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.function.IntBinaryOperator;

public class SumyWierszamiWspółbieżnie {

    private static final int WIERSZE = 10;
    private static final int KOLUMNY = 100;

    private static final int[] wartości_kolumn = new int[KOLUMNY];

    private static final Thread[] wątki_kolumnowe = new Thread[KOLUMNY];

    private static final CyclicBarrier bariera = new CyclicBarrier(KOLUMNY, new Runnable() {

        private int aktualny_wiersz = 0;

        @Override
        public void run() {
            int sum = 0;
            for (int i = 0; i < KOLUMNY; i++) {
                sum += wartości_kolumn[i];
            }
            System.out.println(aktualny_wiersz + " "+ sum);
            aktualny_wiersz++;
        }
    });

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
                try {

                    wartości_kolumn[moja_kolumna] = macierz.applyAsInt(w, moja_kolumna);
                    bariera.await();

                } catch (InterruptedException | BrokenBarrierException e) {
                    Thread t = Thread.currentThread();
                    t.interrupt();
                    System.err.println(t.getName() + " przerwany");
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
