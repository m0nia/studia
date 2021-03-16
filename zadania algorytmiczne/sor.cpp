#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;

int main() {
    int m = 1000000000;
    int n;
    cin >> n;
    int seq[n]; // ciąg który mamy odsortować
    for(int i = 0; i < n; i++)
        cin >> seq[i];

    /* w i-tym wierszu i j-tej kolumnie znajduje się liczba sposób uzyskania za pomocą sortowania komórkowego
     * ciągu długości i+1 (a[j], a[j+1], ..., a[j + i]) – takich, że ostatni dopisany element to a[j]
     */
    int last_on_the_left[n][n];

    /* w i-tym wierszu i j-tej kolumnie znajduje się liczba sposób uzyskania za pomocą sortowania komórkowego
    * ciągu długości i+1 (a[j], a[j+1], ..., a[j + i]) – takich, że ostatni dopisany element to a[j + i]
    */
    int last_on_the_right[n][n];

    /* Uzupełniam pierwszy wiersz w obu tablicach – na ile sposobów można uzyskać poszczególne ciągi długości 1.
     * Można je uzyskać na jeden sposób - nie ma znaczenia, czy pierwszą liczbę umieścimy na prawo czy na lewo
     * od pustego napisu.
     * Liczba sposobów uzyskania za pomocą sortowania komórkowego ciągu (a[j],...a[j + i - 1]) to suma
     * last_on_the_left[i][j] + last_on_the_right[i][j] FIXME!!!!!!!!!!!!!!!!!!
     * Aby uzyskać dobre wyniki w pierwszych wierszach, należy więc zainicjalizować pierwszy wiersz jednej tablicy jedynkami,
     * a pierwszy wiersz drugiej tablicy inicjalizujemy zerami.
     * Można by też zainicjalizować oba wiersza jedynkami, ale wtedy otrzymalibyśmy dwa razy za duży wynik.
     */
    for(int j = 0; j < n; j++) {
        last_on_the_left[0][j] = 1;
        last_on_the_right[0][j] = 0;
    }

    for(int i = 1; i < n; i++){ // rozpatrujemy ciągi długości (i + 1)
        for(int j = 0; j + i < n; j++){
            // rozpatrujemy (seq[j], seq[j + 1], ..., seq[j + i])
            int ile = 0;

            // rozpatrujemy przypadek, że seq[j] był ostatnio dodany
            if(seq[j] < seq[j + 1]) //mogliśmy dodać seq[j] na lewo jeśli seq[j + 1] był ostatnio dodany
                ile += last_on_the_left[i - 1][j + 1];
            if(seq[j] < seq[j + i]) // mogliśmy dodać seq[j] na lewo, jeśli seq[j + i] był ostatnio dodany
                ile += last_on_the_right[i - 1][j + 1];
            // ^ te dwa warunki są takie same jak mamy ciągi długości 2
            if(ile >= m)
                ile = ile % m;
            last_on_the_left[i][j] = ile;

            ile = 0;
            // rozpatrujemy przypadek, że seq[j + i] był ostatnio dodany
            if(seq[j + i] > seq[j]) // mogliśmy dodać seq[j + i] na prawo, jeśli seq[j] był ostatnio dodany
                ile += last_on_the_left[i - 1][j];
            if(seq[j + i] > seq[j + i - 1]) // mogliśmy dodać seq[j + i] na prawo, jeśli seq[j + i - 1] był ostatnio dodany
                ile += last_on_the_right[i - 1][j];
            if(ile >= m)
                ile = ile % m;
            last_on_the_right[i][j] = ile;
        }
    }

    cout << (last_on_the_left[n - 1][0] + last_on_the_right[n - 1][0]) % m << endl;
    return 0;
}
