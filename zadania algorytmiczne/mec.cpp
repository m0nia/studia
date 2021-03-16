#include <iostream>
#include <unordered_set>

using std::cout;
using std::cin;
using std::endl;
using std::unordered_set;

int main() {
    std::ios_base::sync_with_stdio(false);

    uint32_t n, m;
    uint32_t player_nr;
    cin >> n;
    cin >> m;

    unsigned long long int players_array[n + 1]; // pod i-tym indeksem jest zawodnik o numerze i
    unsigned long long int power_of_two = 1; // na początku 2^0

    for(int i = 0 ; i < n + 1; i++)
        players_array[i] = 0;

    for(size_t i = 0; i < m; i++){

        for(size_t j = 1; j <= n/2; j++)
            cin >> player_nr;

        for(size_t j = n/2 + 1; j <= n; j++){ // j-ty zawodnik jest w drugiej drużynie
            cin >> player_nr;
            players_array[player_nr] += power_of_two;
        }
        power_of_two *= 2;
    }

    unordered_set<unsigned long long int> matches;
    bool found_equal_numbers = false;

    for(int j = 1; j <= n; j++){
        if(matches.count(players_array[j]) == 0)
            matches.insert(players_array[j]);
        else {
            found_equal_numbers = true;
            break;
        }
    }

    if(found_equal_numbers)
        cout << "NIE" << endl;
    else
        cout << "TAK" << endl;

    return 0;
}
