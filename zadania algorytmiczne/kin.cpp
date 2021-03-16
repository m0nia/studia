#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int m = 1000000000;
    int n;
    int k;
    std::cin >> n;
    std::cin >> k;
    int p[n];
    for (size_t i = 0; i < n; i++)
        std::cin >> p[i];

    int inversions_count[n];
    for(int i = 0; i < n; i++)
        inversions_count[i] = 1;

    int segment_tree_size = 1;
    while (segment_tree_size < n){
        segment_tree_size *= 2;
    }
    segment_tree_size *= 2;
    int segment_tree[segment_tree_size];

    for(int i = 0 ; i < segment_tree_size; i++)
        segment_tree[i] = 0;

    for(size_t l = 0; l < k - 1; l++) {

        for (size_t i = 0; i < n; i++) {
            int left_bound = 1;
            int right_bound = n;
            int j = 1;
            int inversions_in_the_right_subtree = 0;
            while(left_bound != p[i] || right_bound != p[i]){
                int mid = (left_bound + right_bound)/2;
                segment_tree[j] += inversions_count[i];
                segment_tree[j] = segment_tree[j] % m;
                if(p[i] <= mid){
                    right_bound = mid;
                    if(2 * j + 1 < segment_tree_size) {
                        inversions_in_the_right_subtree += segment_tree[2 * j + 1];
                        inversions_in_the_right_subtree = inversions_in_the_right_subtree % m;
                    }
                    j = 2 * j;
                } else {
                    left_bound = mid + 1;
                    j = 2 * j + 1;
                }
            }
            segment_tree[j] += inversions_count[i];
            segment_tree[j] = segment_tree[j] % m;
            inversions_count[i] = inversions_in_the_right_subtree;
            inversions_count[i] = inversions_count[i] % m;
        }
        for(int i = 0 ; i < segment_tree_size; i++)
            segment_tree[i] = 0;
    }

    int sum = 0;
    for(size_t i = 0; i < n; i++) {
        sum += inversions_count[i];
        sum = sum % m;
    }
    std::cout << sum << '\n';

}