#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <random>

using namespace std;

struct sin{
    int val;
    int nothing =0 ;
};

int main(int argc, char * argv[]) {
    int a[3] = {1,2,3};
    struct sin *s;
    s[0].val = 1;
    s[1].val = 2;
    s[2].val = 3;
    s[3].val = 4;
    for (int i =0; i< sizeof(s)/ sizeof(struct sin); i++){
        cout << s[i].val << " " << endl;
        cout << "i is " << i << endl;
        cout << sizeof(s)/ sizeof(struct sin)<< endl;
        cout << endl;
    }
}