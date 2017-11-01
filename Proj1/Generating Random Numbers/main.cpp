#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <random>

using namespace std;

int main(int argc, char * argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int p = atoi(argv[3]);
    cout << n << endl;
    cout << m << endl;
    cout << p << endl;
    srand((unsigned) time(NULL));
    for (int i=0;i<m;i++){
        cout << mrand48() << endl;
    }
}