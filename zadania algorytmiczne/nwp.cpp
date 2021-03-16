#include <iostream>
#include <algorithm>
#include <climits>

static const int MAX_LENGTH = 20;


int longest_common_subsequence_almost_increasing(int A[], int B[], int n, int m, int c){
    // w j-tej kolumnie w i-tym wierszu znajduje się maxymalna wartość w ciągu długości i
    // kończąca się na B[j], INT_MAX jeśli nie było do tej pory podciągu długości i
    int lowest_max[MAX_LENGTH + 1][m];

    int min_max_value[MAX_LENGTH + 1]; // pod i-tym indeksem znajdzie się najmniejsza maksymalna wartość
    // wartość w podciągu długości i, znalezionych do tej pory

    for(int i = 0; i < m; i++)
        lowest_max[0][i] = INT_MIN;

    for(int i = 1; i < MAX_LENGTH + 1; i++){
        for(int j = 0; j < m; j++)
            lowest_max[i][j] = INT_MAX;
    }

    int longest = 0;

    for(int i = 0; i < n; i++){
        min_max_value[0] = INT_MIN;
        for(int k = 1; k < MAX_LENGTH + 1; k++)
            min_max_value[k] = INT_MAX;

        for(int j = 0; j < m; j++){

            int prev[MAX_LENGTH + 1];
            std::copy(std::begin(min_max_value), std::end(min_max_value), std::begin(prev));

            for(int l = 1; l < MAX_LENGTH + 1; l++)
                min_max_value[l] = std::min(min_max_value[l], lowest_max[l][j]);

            // jak wczesniej trafilam na jakas rownosc, to chce przekazac dalej jaki byl stan min_max_value przed dodaniem tej jedynki ( nie mogę kilka razy przedłuzyc tym samym)
            if(B[j] == A[i]){ // możemy wydłużyć podciąg, aktualizujemy tablicę lowest_max w kolumnie j-tej

                for(int k = MAX_LENGTH; k > 0; k--){
                    if(prev[k - 1] <= A[i] + c){ // możemy przedłużyć podciąg długości k - 1 dodając na koniec A[i]

                        lowest_max[k][j] = std::min(std::max(prev[k - 1], A[i]), lowest_max[k][j]);
                        longest = std::max(longest, k);
                    }
                }
            }

        }
    }
    return longest;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, m, c;
    std::cin >> n;
    std::cin >> m;
    std::cin >> c;
    int A[n];
    for(int i = 0; i < n; i++)
        std::cin >> A[i];

    int B[m];
    for(int i = 0; i < m; i++)
        std::cin >> B[i];

    std::cout << longest_common_subsequence_almost_increasing(A, B, n, m, c) << "\n";

    return 0;
}