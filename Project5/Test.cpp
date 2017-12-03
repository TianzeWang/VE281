#include <iostream>
#include <cstdlib>
#include <set>

using namespace std;

struct compare {
    bool operator()(const int &a, const int &b) {
        return a > b;
    }
};

int main() {
    set<int, compare> set1;
    for (int i=0; i<20;i++){
        set1.insert(i);
    }
    cout << *set1.begin() << endl;
    set1.erase(set1.begin());
    cout << *set1.begin() << endl;
}
