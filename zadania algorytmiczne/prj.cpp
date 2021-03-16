#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <queue>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::priority_queue;
using std::make_pair;
using std::max;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n, m, k, what, from;
    cin >> n;
    cin >> m;
    cin >> k;
    int programists_min_nr[n+1]; // pod i-tym indeksem jest minimalna liczba programistów potrzebna do wykonania i-tego projektu
    int nr_of_dependencies[n+1]; // od ilu projektów zależy jeszcze i-ty projekt
    vector<int> dependence[n+1]; // pod i-tym indeksem jest lista projektów zależnych bezpośrednio od projektu i

    for(int i = 1; i <= n; i++){
        cin >> programists_min_nr[i];
        nr_of_dependencies[i] = 0;
    }

    for(int i = 0; i < m; i++){
        cin >> what;
        cin >> from;
        dependence[from].push_back(what);
        nr_of_dependencies[what]++;
    }

    int min_nr = INT_MIN;

    priority_queue<pair<int, int>> possible_projects;
    for(int i = 1; i <= n; i++){
        if(nr_of_dependencies[i] == 0)
            possible_projects.push(make_pair((-1) * programists_min_nr[i],i));
    }

    for(int i = 0; i < k; i++){
        auto p = possible_projects.top();
        possible_projects.pop();
        int project_nr = p.second;
        int required_programists_nr = (-1) * p.first;
        min_nr = max(min_nr, required_programists_nr);
        for(auto d: dependence[project_nr]){
            nr_of_dependencies[d]--;
            if(nr_of_dependencies[d] == 0)
                possible_projects.push(make_pair((-1) * programists_min_nr[d], d));
        }
    }
    cout << min_nr << '\n';

    return 0;
}