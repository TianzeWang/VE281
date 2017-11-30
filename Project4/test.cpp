//
// Created by 大泽 on 2017/11/25.
//
#include <cmath>
#include <set>
#include <iostream>
#include <deque>
#include <queue>
#include <map>
#include <vector>

using namespace std;

struct order_compare {
    bool operator()(const int &a, const int &b) {
        return a > b;
    }
};

template <typename T>
void print_queue(T &q) {
    while (!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

int main() {
    map<int, string> MAP;
    MAP.insert(make_pair(1, "abc"));
    MAP.insert(make_pair(1, "ace"));
    MAP.insert(make_pair(2, "abc"));
    MAP.insert(make_pair(3, "abc"));
    MAP.insert(make_pair(4, "abc"));
    MAP.insert(make_pair(15, "abc"));
    cout << (++MAP.find(1))->second << (*MAP.find(1)).second;

}