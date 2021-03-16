#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
using std::cin;

using std::vector;

void print(int A[], int n){
    for(int i = 1; i <= n; i++){
        std::cout << A[i] << " ";
    }
    std::cout << "\n";
}

int calculate_size(int n){ //ile bitów potrzeba do zapisania liczby n
    int i = 0;
    while(n > 0){
        n /= 2;
        i++;
    }
    return i;
}
void find_nodes_depth(int i, int n, int depth, int g[], int left_child[], int right_child[]){
    if(i != -1){
        g[i] = depth;
        find_nodes_depth(left_child[i], n, depth + 1, g, left_child, right_child);
        find_nodes_depth(right_child[i], n, depth + 1, g, left_child, right_child);
    }
}

// zwraca numer wierzchołka znajdującego się d krawędzi nad wierzchołkiem startowym
int find_upper_node(int start_node, int d, const vector<vector<int>>& jumps){
    int i = 0;
    int current_node = start_node;
    while(d > 0){
        if(d % 2 == 1){
            assert(current_node != -1);
            // przenosimy się do wierzchołka znajdującego się 2^i krawędzi nad current_node
            current_node = jumps[i][current_node];
        }
        d /= 2;
        i++; //aktualizujemy i
    }
    return current_node;
}

// int h[] -- pod i-tym indeksem znajduje się wysokość podrzewa zaczepionego w i-tym wierzchołku
// int deepest_leafs[] -- pod i-tym indeksem znajduje się najgłębszy liść w podrzewie zaczepionym w i-tym wierzchołku
int find_deepest_leafs_and_subtrees_height(int i, int n, int* leaf, int h[], int deepest_leafs[], int left_child[], int right_child[]){
    if(i == -1)
        return -1;
    if(left_child[i] == -1 && right_child[i] == -1){
        *leaf = i;
        h[i] = 0; // wysokość poddrzewa zaczepionego w liściu wynosi 0
        deepest_leafs[i] = i; //FIXME: może -1 ?
        return 0;
    }
    int left_leaf, right_leaf;
    int d_l = find_deepest_leafs_and_subtrees_height(left_child[i], n, &left_leaf, h, deepest_leafs, left_child, right_child);
    int d_r = find_deepest_leafs_and_subtrees_height(right_child[i], n, &right_leaf,h, deepest_leafs, left_child, right_child);

    if(d_l > d_r){
        *leaf = left_leaf;
        h[i] = 1 + d_l;
        deepest_leafs[i] = left_leaf;
    } else {
        *leaf = right_leaf;
        h[i] = 1 + d_r;
        deepest_leafs[i] = right_leaf;
    }
    return h[i];
}

// int h[] -- pod i-tym indeksem znajduje się wysokość podrzewa zaczepionego w i-tym wierzchołku
// int deepest_leafs[] -- pod i-tym indeksem znajduje się najgłębszy liść w podrzewie zaczepionym w i-tym wierzchołku
void find_furthest_nodes(int i, int longest_path_up, int furthest_node_up, int longest_up_lca, int n, int longest_path[], int furthest_node[], int lca[], int h[], int deepest_leafs[], int left_child[], int right_child[]){
    if(i == -1)
        return;

    if(longest_path_up > h[i]){
        longest_path[i] = longest_path_up;
        furthest_node[i] = furthest_node_up;
        lca[i] = longest_up_lca;
    } else {
        longest_path[i] = h[i];
        furthest_node[i] = deepest_leafs[i];
        lca[i] = i; // jeśli najdalszy node jest w poddrzewie, to oznaczamy jako lca tych dwóch węzłów jako wierzchołek tego podrzewa
    }
    int left = left_child[i];
    int right = right_child[i];

    // jeśli nie ma prawego syna to po prostu przekazuję longest_path_up
    if(right == - 1 || 1 + longest_path_up > 2 + h[right]){
        find_furthest_nodes(left, 1 + longest_path_up, furthest_node_up, longest_up_lca, n, longest_path, furthest_node, lca, h, deepest_leafs, left_child, right_child);
    } else {
        find_furthest_nodes(left, 2 + h[right], deepest_leafs[right], i, n, longest_path, furthest_node, lca, h, deepest_leafs, left_child, right_child);
    }

    if(left == -1 || 1 + longest_path_up > 2 + h[left]){
        find_furthest_nodes(right, 1 + longest_path_up, furthest_node_up, longest_up_lca, n, longest_path, furthest_node, lca, h, deepest_leafs, left_child, right_child);
    } else {
        find_furthest_nodes(right, 2 + h[left], deepest_leafs[left], i,n, longest_path, furthest_node, lca, h, deepest_leafs, left_child, right_child);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n;
    cin >> n;
    int parent[n+1];
    parent[1] = -1;
    int left_child[n+1];
    int right_child[n+1];

    for(int i = 1; i <= n; i++){
        int left, right;
        cin >> left;
        cin >> right;
        left_child[i] = left;
        right_child[i] = right;
        if(left != -1)
            parent[left] = i;
        if(right != -1)
            parent[right] = i;
    }

    // w i-tym wierszu, w j-tej kolumnie znajduje się numer wierzchołka, znajdujący się 2^i krawędzi ponad wierzchołkiem j
    // najdłuższy "skok" w górę, jaki możemy zrobić ma długość 2^[log(n-1)], gdzie [..] to podłoga
    // skoki mają więc długości ze zbioru {2^0, 2^1, ... 2^[log(n-1)]}, możemy więc wykonać 1+[log(n-1)] skoków
    int jumps_nr = calculate_size(n-1);//1 + (int)floor(log(n)); //albo sufit z logn, wtedy czesc moze być -1 nie wiemmmmmmmm:(


    vector<vector<int>> jumps;
    vector<int> first_row;
    first_row.push_back(0);
    for(int i = 1; i <= n; i++) {
        first_row.push_back(parent[i]);

    }
    jumps.push_back(first_row);

    for(int j = 1; j < jumps_nr; j++){
        vector<int> v;
        v.push_back(0);
        for(int i = 1; i <= n; i++){
            int ancestor_1, ancestor_2;
            ancestor_1 = jumps[j-1][i]; // numer wierzchołka znajdującego się 2^(j-1) krawędzi nad wierzchołkiem i
            if(ancestor_1 != -1)
                ancestor_2 = jumps[j-1][ancestor_1]; // numer wierzchołka znajdującego się 2^(j-1) krawędzi nad ancestor1, czyli 2*2^(j-1) = 2^j krawędzi nad wierzchołkiem i
            else
                ancestor_2 = -1;
            v.push_back(ancestor_2);
            //jumps[j][i] = ancestor_2;
        }
        jumps.push_back(v);
    }

    // wyznaczyć dla każdego wierzchołka u najbardziej oddalony od niego wierzchołek v i ich wspólnego przodka (o ile istnieje)
    int longest_path[n+1]; // pod i-tym indeksem jest długość ścieżki do najbardziej oddalonego wierzchołka od wierzchołka i
    int furthest_node[n+1]; // pod i-tym indeksem jest numer wierzchołka, który jest najbardziej oddalony od wierzchołka i
    int lca[n+1]; // pod i-tym indeksem jest najniższy wspólny wierzchołek wierzchołków i oraz farhest_node[i]

    int h[n + 1];
    int deepest_leafs[n+1];
    int g[n + 1]; // głębokości węzłów
    int deepest_leaf;

    find_deepest_leafs_and_subtrees_height(1, n,  &deepest_leaf,h, deepest_leafs, left_child, right_child);
    find_nodes_depth(1, n, 0, g, left_child, right_child);
    find_furthest_nodes(1, 0, 1, 1, n, longest_path, furthest_node, lca, h, deepest_leafs, left_child, right_child);


    int m, a, d;
    cin >> m;
    for(int j = 0; j < m; j++){
        cin >> a;
        cin >> d;
        if(d > longest_path[a]) {
            std::cout << -1 << '\n';
        } else {
            // najdalszy wierzchołek znajduje się w tym samym poddrzewie:
            // - albo lca[a] = a - wtedy a jest korzeniem poddrzewa, a najbardziej oddalony wierzchołek jest najgłębszym liściem zaczepionym w poddrzewie a
            // - albo najdalszy wierzchołek jest jednocześnie korzeniem - to znaczy, że a jest liściem
            //najdalszy wierzchołek znajduje się w innym poddrzewie
            // wtedy lca[a] != a ani lca[a] != furthest_node[a]


            if(lca[a] == a){
                // najbardziej oddalony wierzchołek znajduje się w podrzewie zaczepionym w wierzchołku a
                // idziemy do wierzchołka znajdującego się d(a, y) - d krawędzi nad furthest_node[a]
                // w tym wypadku furthest_node[a] jest liściem, więc d(a, y) = h[a]
                // będziemy to robić wykonując "skoki" o 2^i dla pewnych i
                std::cout << find_upper_node(furthest_node[a], g[furthest_node[a]] - g[a] - d, jumps) << '\n';
            } else if (lca[a] == furthest_node[a]) {
                std::cout << find_upper_node(a, d, jumps) << '\n';
                // najbardziej oddalony wierzchołek znajduje się w drugim podrzewie lca[a]
                // dwie możliwości:
                // 1. szukany wierzchołek oddalony o d od a jest na ścieżce od a do lca[a], albo
                // 2. szukany wierzchołek jest na ścieżce od lca[a] do furthest_node[a]
            } else{
                int temp = g[a] - g[lca[a]];
                if(g[a] - g[lca[a]] >= d) { // długość ścieżki od i do lca[i] jest nie mniejsza niż d
                    // wtedy po prostu znajdujemy node znajdujący się d krawędzi nad wierzchołkiem a
                    std::cout << find_upper_node(a, d, jumps) << '\n';

                } else {
                    // znajdujemy wierzchołek znajdujący się  x = d - d(a, lca[a]) krawędzi poniżej lca[a]
                    int down = d - (g[a] - g[lca[a]]);
                    int up = g[furthest_node[a]] - g[lca[a]] - down;
                    // czyli d(lca[a], furthest_node[a]) - x krawędzi ponad furthest_node[a]
                    std::cout << find_upper_node(furthest_node[a], up, jumps) << '\n';
                }
            }
        }
    }

    return 0;
}
