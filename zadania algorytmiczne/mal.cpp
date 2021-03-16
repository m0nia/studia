#include <iostream>

using std::cin;
using std::cout;

size_t smallest_power_of_two(int n){
    size_t i = 1;
    while(i < n)
        i *= 2;
    return i;
}

// left, right - granice przedziału, w którym aktualnie się znajdujemy
// a, b - szukany przedział
// zawsze a >= left i b <= right
void paint_highway(int i, int left, int right, int a, int b, char c, int segment_highway[], size_t size){
    if(i >= size)
        return;
    if(left == a && right == b) { // kolorujemy cały przedział [a,b] na dany kolor
        segment_highway[i] = c == 'B' ? right - left + 1 : 0;
        return;
    }

    if(segment_highway[i] == 0 || segment_highway[i] == right - left + 1){ // propagujemy w dół
        if(2*i < size) segment_highway[2*i] = segment_highway[i] / 2;
        if(2*i + 1 < size) segment_highway[2*i + 1] = segment_highway[i] / 2;
    }

    int mid = (left + right)/2;
    if(b <= mid){
        paint_highway(2*i, left, mid, a, b, c, segment_highway, size);
    } else if(a > mid){
        paint_highway(2*i + 1, mid + 1, right, a, b, c, segment_highway, size);
    } else{
        paint_highway(2*i, left, mid, a, mid, c, segment_highway, size);
        paint_highway(2*i + 1, mid + 1, right, mid + 1, b, c, segment_highway, size);
    }
    int white_left = 2*i < size ? segment_highway[2*i] : 0;
    int white_right = 2*i + 1 < size ? segment_highway[2*i +1] : 0;
    segment_highway[i] = white_left + white_right;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, m,  a, b;
    char c;
    cin >> n;
    cin >> m;
    size_t size = smallest_power_of_two(n);
    int segment_highway[2*size]; // w węźle jest informacja ile jest pomalowanych węzłów na biało w podrzewie
    segment_highway[1] = 0;

    for(int i = 0; i < m; i++){
        cin >> a;
        cin >> b;
        cin >> c;
        paint_highway(1, 1, size, a, b, c, segment_highway, 2*size);
        cout << segment_highway[1] << '\n';
    }
}