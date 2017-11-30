//
// Created by 大泽 on 2017/11/25.
//
#include <cmath>
#include <set>
#include <iostream>
#include <deque>
#include <queue>
#include <vector>

using namespace std;

struct order_compare {
    bool operator()(const int &a, const int &b) {
        return a > b;
    }
};

//int main(){
////    multiset<int, order_compare> s;
////    multiset<int, order_compare>::iterator it;
////    s.insert(1);
////    s.insert(3);
////    s.insert(5);
////    s.insert(2);
////    s.insert(2);
////    for (it = s.begin(); it !=s.end(); it++){
////        cout << *it << endl;
////    }
//    priority_queue PQ;
//
//    return 0;
//}

template <typename T>
void print_queue(T &q) {
    while (!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

int main() {
    std::priority_queue<int> q;

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q.push(n);

    print_queue(q);

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q.push(n);
    q.pop();
    print_queue(q);
    cout << q.top() << endl;

    std::priority_queue<int, std::vector<int>, std::greater<int> > q2;

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q2.push(n);
    print_queue(q2);

    print_queue(q2);

    cout << q2.top() << endl;
//    q2.pop();
    // Using lambda to compare elements.
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1);};
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q3.push(n);

    print_queue(q3);
//    int a;
//    int i;
//    int sum = 0;
//    bool isPrime = true;
//    for (a = 2; a <= 100; a++) {
//        isPrime = true;
//        for (i = 2; i < a; i++) {
//            if (a % i == 0) {
//                isPrime = false;
//                break;
//            }
//        }
//        if (isPrime) {
//            sum += a;
//        }
//    }
//    cout << sum;
}