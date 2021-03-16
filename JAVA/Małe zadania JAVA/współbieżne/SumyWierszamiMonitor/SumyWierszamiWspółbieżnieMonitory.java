package com.company;

import java.util.function.IntBinaryOperator;

public class SumyWierszamiWspółbieżnieMonitory {

    private static final int WIERSZE = 10;
    private static final int KOLUMNY = 100;

    private static Sumujący suma = new Sumujący();

    private static class Sumujący {

        private int suma = 0;

        private int ilePoliczonych = 0;

        private int wiersz = 0;

        public synchronized void dodaj(int x) throws InterruptedException {
            int stary = wiersz;
            suma += x;
            ilePoliczonych++;

            if(ilePoliczonych == KOLUMNY) {
                notifyAll();
            }
            while(wiersz != stary + 1)
                wait();
        }

        public synchronized void wypisz() throws InterruptedException {
            while(ilePoliczonych < KOLUMNY)
                wait();
            System.out.println(wiersz + " " + suma);
            suma = 0;
            ilePoliczonych = 0;
            wiersz++;
            notifyAll();
        }
    }

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
                int x = macierz.applyAsInt(w, moja_kolumna);
                try {
                    suma.dodaj(x);
                } catch (InterruptedException e) {
                    Thread t = Thread.currentThread();
                    t.interrupt();
                    System.err.println(t.getName() + " przerwany");
                }
            }
        }
    }

    private static void piszSumyWierszy(int kolumny, IntBinaryOperator macierz) {
        for(int k = 0; k < kolumny; ++k) {
            Thread t = new Thread(new Liczący(macierz, k));
            t.start();
        }

        for(int w = 0; w < WIERSZE; w++) {
            try {
                suma.wypisz();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    public static void main(String[] args) {

        piszSumyWierszy(KOLUMNY, (wiersz, kolumna) -> {
            int a = 2 * kolumna + 1;
            return (wiersz + 1) * (a % 4 - 2) * a;
        });

    }

}