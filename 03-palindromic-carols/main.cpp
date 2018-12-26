#include <iostream>
#include <vector>

#define MOD 1000000007

using namespace std;

class Node {

private:
    vector<pair<int, char>> neighbors;

public:
    const vector<pair<int, char>>& get_neighbors() const {
        return neighbors;
    }

    void add_neighbor(int to, char c) {
        neighbors.emplace_back(to, c);
    }
};

vector<vector<vector<int>>> memo;
vector<Node> forwards;
vector<Node> backwards;

int count_palindromes(int start, int target, int mistakes, int max) {
    if(memo[mistakes][start][target] != -1) {
        return memo[mistakes][start][target];
    }

    int add_odd = 0;
    int add_recursive = 0;

    if(start == target) {
        return 1;
    }

    for(pair<int, char> n : forwards[start].get_neighbors()) {
        if(n.first == target) {
            add_odd++;
        }
    }

    for(pair<int, char> n : forwards[start].get_neighbors()) {
        if(n.first != target) {
            for(pair<int, char> m : backwards[target].get_neighbors()) {
                if(n.second == m.second) {
                    add_recursive += count_palindromes(n.first, m.first, mistakes, max);
                    add_recursive %= MOD;
                }
                else if(mistakes < max) {
                    add_recursive += count_palindromes(n.first, m.first, mistakes+1, max);
                    add_recursive %= MOD;
                }
            }
        }
    }

    memo[mistakes][start][target] = (add_recursive + add_odd) % MOD;
    return memo[mistakes][start][target];
}

int main() {
    int cnt = 0;
    int n, k, start, target;
    cin >> n >> k >> start >> target;

    for(int i = 0; i < n; i++) {
        forwards.emplace_back();
        backwards.emplace_back();
    }

    memo = vector<vector<vector<int>>>(k+1, vector<vector<int>>(n, vector<int>(n)));
    for (int i = 0; i < k+1; i++) {
        for (int j = 0; j < n; j++) {
            for (int l = 0; l < n; l++) {
                memo[i][j][l] = -1;
            }
        }
    }

    for(int i = 0; i < n; i++) {
        int positions;
        cin >> positions;
        for(int j = positions; j > 0; j--) {
            int to;
            char c;
            cin >> to >> c;
            forwards[i].add_neighbor(to, c);
            backwards[to].add_neighbor(i, c);
        }
    }

    cnt = count_palindromes(start, target, 0, k);

    cout << cnt << endl;
    return 0;
}