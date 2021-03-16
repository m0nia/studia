#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;
using std::max;

using vector_2D = vector<vector<int>>;
bool max_cond = true;

int choose(int a, int b){
    if(max_cond) return a > b? a: b;
    if(a == 0) return b;
    if(b == 0) return a;
    return a < b ? a: b;
}

vector<int> arr_max(const vector<int>& v1, const vector<int>& v2){
    if(v1.empty()) // tak chyba nigdy nie będzie
        return vector<int>(v2);
    if(v2.empty())
        return vector<int>(v1);

    int size = v1.size();
    vector<int> max_v(size);
    for(int i = 1; i < size; i++){
        max_v[i] = choose(v1[i], v2[i]);
    }
    return max_v;
}

// arr ma wartości dodatnie
// kiedy l==r dotarliśmy do liścia, czyli przedziału jednego elementowego - max z tego przedziału to po prostu jedyna wartość w tym przedziale
int build_max_segment_tree(int i, int l, int r, vector<int>& segment_tree, const vector<int>& arr){
    if(l >= arr.size()) return 0; // w szczególności wtedy też r>=l>arr.size()
    if(l == r) {
        segment_tree[i] = arr[l];
        return segment_tree[i];
    }
    int mid = (l + r)/2;
    int max_left = build_max_segment_tree(2*i, l, mid, segment_tree, arr);
    int max_right = build_max_segment_tree(2*i + 1, mid + 1, r,segment_tree, arr);
    segment_tree[i] = choose(max_left, max_right);
    return segment_tree[i];
}

// przedział [l,r] to przedział na współrzędnych x
// wartości węzłów w tym drzewie to drzewa przedziałowe po współrzędnych y
void build_max_2D_segment_tree(int i, int l, int r, vector_2D& segment_tree_2D, const vector_2D& arr, int segment_size_y){
    if(l >= arr.size()) return;
    if(l == r){
        segment_tree_2D[i] = vector<int>(2*segment_size_y);
        build_max_segment_tree(1, 1, segment_size_y, segment_tree_2D[i], arr[l]);
        return;
    }
    int mid = (l + r)/2;
    build_max_2D_segment_tree(2*i, l, mid, segment_tree_2D, arr, segment_size_y);
    build_max_2D_segment_tree(2*i + 1, mid + 1, r, segment_tree_2D, arr, segment_size_y);
    // 2*i i 2*i + 1 < segment_tree_2D.size() bo tu przechodzą węzły które nie są liśćmi
    segment_tree_2D[i] = arr_max(segment_tree_2D[2*i], segment_tree_2D[2*i+1]);
}

void print_arr(const vector<int>& arr){
    for(int x : arr)
        cout << x << ' ';
    cout << '\n';
}

void print_2D_arr(const vector<vector<int>>& arr){
    for(int i = 1; i < arr.size(); i++){
        for(int j = 1; j < arr[i].size(); j++)
            cout << arr[i][j] << ' ';
        cout << '\n';
    }
}

// Oblicza najmniejszą potęgę 2 większą od n
int size(int n){
    int i = 1;
    while(i < n) i *= 2;
    return i;
}

//indeks i jest zawsze w dobrym przedziale -- jak dojdziemy do liścia to nie schodzimy głębiej
int search_segment_tree(int i, int l, int r, int y1, int y2, const vector<int>& segment_tree){
    if(l == y1 && r == y2)
        return segment_tree[i];
    int mid = (l+r)/2;
    if(y2 <= mid){
        return search_segment_tree(2*i, l, mid, y1, y2, segment_tree);
    } else if (y1 > mid){
        return search_segment_tree(2*i+1, mid+1, r, y1, y2, segment_tree);
    } else {
        return choose(search_segment_tree(2*i, l, mid, y1, mid, segment_tree),
                      search_segment_tree(2*i +1, mid+1, r, mid+1, y2, segment_tree));
    }
}
// zawsze  l <= x1, x2 <= r i x1 <= x2
int search_2D_segment_tree(int i, int l, int r, int x1, int x2, int y1, int y2, int segment_size_y, const vector_2D& segment_tree_2D){
    // warunek brzegowy - liść
    if(l == x1 && r == x2){ // znaleźliśmy przedział którego szukamy
        // przeszukaj drzewo przedziałowe segment_tree[i] w poszukiwaniu przedziału [y1, y2]
        return search_segment_tree(1, 1, segment_size_y, y1, y2, segment_tree_2D[i]);
    }
    int mid = (l + r)/2;
    if(x2 <= mid) {// szukaj w lewej połowie
        return search_2D_segment_tree(2*i, l, mid, x1, x2, y1, y2, segment_size_y, segment_tree_2D);
    } else if (x1 > mid){ // szukaj w prawej połowie
        return search_2D_segment_tree(2*i+1, mid+1, r, x1, x2, y1, y2, segment_size_y, segment_tree_2D);
    } else { // x2 > mid && x1 <= mid, szukaj w obu połowach
        return choose(search_2D_segment_tree(2*i, l, mid, x1, mid, y1, y2, segment_size_y, segment_tree_2D),
                      search_2D_segment_tree(2*i+1, mid+1, r, mid+1, x2, y1, y2, segment_size_y, segment_tree_2D));

    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, m, k, segment_size_x, segment_size_y;
    cin >> n; // ile wierszy
    cin >> m; // ile kolumn
    cin >> k; // liczba zapytań
    segment_size_x = size(n);
    segment_size_y = size(m);
    vector<vector<int>> arr(n+1); // żyzność
    for(int i = 1; i <= n; i++){
        arr[i] = vector<int>(m+1);
        for(int j = 1; j <= m; j++)
            cin >> arr[i][j];
    }
    vector_2D segment_tree_2D_max(2 * segment_size_x);
    vector_2D segment_tree_2D_min(2 * segment_size_x);
    build_max_2D_segment_tree(1, 1, segment_size_x, segment_tree_2D_max, arr, segment_size_y);
    max_cond = false;
    build_max_2D_segment_tree(1, 1, segment_size_x, segment_tree_2D_min, arr, segment_size_y);

    int x1, x2, y1, y2;
    for(int i = 0; i < k; i++){
        cin >> x1;
        cin >> y1;
        cin >> x2;
        cin >> y2;

        max_cond = true;
        int max_z = search_2D_segment_tree(1, 1, segment_size_x,x1+1, x2+1, y1+1,y2+1,segment_size_y, segment_tree_2D_max);
        max_cond = false;
        int min_z = search_2D_segment_tree(1, 1, segment_size_x,x1+1, x2+1, y1+1,y2+1,segment_size_y, segment_tree_2D_min);
        cout << max_z - min_z << '\n';
    }

    return 0;
}