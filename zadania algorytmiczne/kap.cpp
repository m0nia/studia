#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <climits>

using std::vector;
using std::set;
using std::map;
using std::pair;
using std::make_pair;
using std::cin;
using std::cout;
using coord_t = pair<int, int>;

int distance(coord_t A, coord_t B){
    return std::min(abs(A.first - B.first), abs(A.second - B.second));
}

void add_coordinates_neighbours(int n, vector<coord_t>& coordinates, map<coord_t, vector<coord_t>> & graph){
    for(int i = 0; i < n; i++){
        if(i-1 >= 0){
            graph.at(coordinates[i]).push_back(coordinates[i-1]);
        }
        if(i+1 < n){
            graph.at(coordinates[i]).push_back(coordinates[i+1]);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, x, y;
    coord_t start, end;
    cin >> n;
    vector<coord_t> coordinates; // tablica współrzędnych wysp
    map<coord_t, vector<coord_t>> graph; // mapuje współrzędne wektora do jego x- i y- sąsiadów
    for(int i = 0; i < n; i++){
        cin >> x;
        cin >> y;
        coord_t p = make_pair(x, y);
        coordinates.push_back(p);
        graph.insert(make_pair(p, vector<coord_t>()));
        if(i == 0) start = p;
        if(i == n - 1) end = p;
    }

    sort(coordinates.begin(), coordinates.end()); //sortowanie par po pierwszej współrzędnej
    add_coordinates_neighbours(n, coordinates, graph); //dodajemy x-sąsiadów do grafu

    sort(coordinates.begin(), coordinates.end(),
         [](coord_t p1, coord_t p2){
             if(p1.second == p2.second) return p1.first < p2.first;
             return p1.second < p2.second;
    }); // sortowanie po drugiej współrzędnej
    add_coordinates_neighbours(n, coordinates, graph); // dodajemy y-sąsiadów

    //szukamy dijkstrą najkrótszej ścieżki z wyspy 1 do wyspy n
    set<pair<int, coord_t>> q; // kolejka priorytetowa <długość najlepszej ścieżki do wierzchołka startowego, współrzędne>
    map<coord_t, int> dist; // mapa dystansów od źródła
    for(coord_t c: coordinates) {
        if(c == start) dist.insert(make_pair(start, 0));
        dist.insert(make_pair(c, INT_MAX));
    }
    q.insert(make_pair(0, start));


    while((*q.begin()).second != end){
        pair<int, coord_t> temp = *q.begin();
        int dist1 = temp.first;
        coord_t curr = temp.second;
        q.erase(q.begin());

        vector<coord_t> neighbours = graph.at(curr);
        for(coord_t c: neighbours){
            int dist2 = dist.at(c);
            int new_dist = dist1 + distance(c, curr);
            if(dist2 > new_dist){
                if(dist2 != INT_MAX)
                    q.erase(make_pair(dist2, c));
                dist.erase(c);
                dist.insert(make_pair(c, new_dist));
                q.insert(make_pair(new_dist, c));
            }
        }
    }
    cout << dist.at(end) << '\n';
    return 0;
}