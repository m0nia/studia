/**
* Metoda testująca.
*/

public class Main{
    public static void main(String[] args) {
        Ulamek r0 = new Ulamek(0, 1),
    	       r1 = new Ulamek(1, 1),
               r2 = new Ulamek(1, 2),
               r3 = new Ulamek( -1, 1),
               r4, r5, r6, r7;

        System.out.println("Liczby:");
        System.out.println("r0 = " + r0);
        System.out.println("r1 = " + r1);
        System.out.println("r2 = " + r2);
        System.out.println("r3 = " + r3);

        System.out.println("Operacje:");
        r4 = r1.add(r2);
        r5 = r3.sub(r2);
        r6 = r2.mul(r3);
        r7 = r2.div(r3);

        System.out.println("r1 + r2 = " + r4); //  3/2
        System.out.println("r3 - r2 = " + r5); // -3/2
        System.out.println("r2 * r3 = " + r6); // -1/2
        System.out.println("r2 / r3 = " + r7); // -1/2

        System.out.println("Porównania");
        System.out.println("r2 < r1 : " + (r2.compareTo(r1) == -1 ? "TAK" : "NIE"));
        System.out.println("r1 = r2 : " + (r2.equals(r1) ? "TAK"  : "NIE"));
    }
}