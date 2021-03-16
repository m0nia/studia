#include <iostream>
#include <algorithm>
#include <climits>

#define ODD 1
#define EVEN 0
#define NONE -1

int parityOf(int a){
    if(a % 2 == 0)
        return EVEN;
    return ODD;
}

void fill_sufix_sum_array(long long int sum[], int prices[], int n){
    sum[n - 1] = prices[n - 1];
    for(int i = n - 2; i >= 0; i--){
        sum[i] = sum[i + 1] + prices[i];
    }
}

void fill_min_on_the_right_array(int prices[], int min[], int n, int p){
    if(parityOf(prices[n - 1]) == p) {
        min[n - 1] = prices[n - 1];
    } else {
        min[n - 1] = NONE;
    }

    for(int i = n - 2; i >= 0; i--){
        if(parityOf(prices[i]) == p){
            min[i] = prices[i];
        } else {
            min[i] = min[i + 1];
        }
    }
}

void fill_max_on_the_left_array(int prices[], int max[], int n, int p){
    if(parityOf(prices[0]) == p) {
        max[0] = prices[0];
    } else {
        max[0] = NONE;
    }

    for(int i = 1; i < n; i++){
        if(parityOf(prices[i]) == p){
            max[i] = prices[i];
        } else {
            max[i] = max[i - 1];
        }
    }
}

int main()
{
    int n; // licza wszystkich produktów, 1 <= n <= 1 000 000
    int m; // liczba dni, 1 <= m <= 1 000 000
    int k; // liczba produktów kupowanych w danym dniu, 1 <= k <= n

    std::cin >> n;

    long long int sum[n]; // pod i-tym indeksem znajduje się suma elementów tablicy prices z przedziały [i, ..., n - 1]
    int min_odd[n]; // pod i-tym indeksem znajduje się najmniejsza liczba nieparzysta z przedziału [i, ..., n - 1]
    int min_even[n]; // pod i-tym indeksem znajduje się najmniejsza liczba parzysta z przedziału [i, ..., n - 1]
    int max_odd[n]; // pod i-tym indeksem znajduje się największa liczba nieparzysta z przedziału [0, ..., i]
    int max_even[n]; // pod i-tym indeksem znajduje się największa liczba parzysta z przedziału [0, ..., i]

    int prices[n];
    for(int i = 0; i < n; i++)
        std::cin >> prices[i];

    std::cin >> m;

    fill_sufix_sum_array(sum, prices, n);

    fill_min_on_the_right_array(prices, min_odd, n, ODD);
    fill_min_on_the_right_array(prices, min_even, n, EVEN);

    fill_max_on_the_left_array(prices, max_odd, n, ODD);
    fill_max_on_the_left_array(prices, max_even, n, EVEN);


    for(int i = 0; i < m; i++) {
        std::cin >> k;
        if(sum[n - k] % 2 == 1){
            printf("%lld\n", sum[n - k]);
        } else {
            long long int result;
            int diff1, diff2;
            diff1 = diff2 = INT_MAX;
            if(min_odd[n - k] != NONE && max_even[n - k] != NONE && n - k != 0)
                diff1 = min_odd[n - k] - max_even[n - k - 1];
            if(min_even[n - k] != NONE && max_odd[n - k] != NONE && n - k != 0)
                diff2 = min_even[n - k] - max_odd[n - k - 1];

            result = (diff1 == INT_MAX && diff2 == INT_MAX) ? -1 : sum[n - k] - std::min(diff1, diff2);

            printf("%lld\n", result);
        }
    }
    return 0;
}