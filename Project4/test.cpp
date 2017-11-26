//
// Created by 大泽 on 2017/11/25.
//

#include <set>
#include <iostream>
using namespace std;
struct order_compare {
    bool operator()(int &a, int &b) {
        return a > b;
    }
};

int main(){
    multiset<int, order_compare> s;
    multiset<int, order_compare>::iterator it;
    s.insert(1);
    s.insert(3);
    s.insert(5);
    s.insert(2);
    for (it = s.begin(); it !=s.end(); it++){
        cout << *it << endl;
    }
    return 0;
}