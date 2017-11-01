#include <iostream>
#include <random>

using namespace std;

int main(int argc, char * argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    for (int i = 0; i<=5; i++) {
        for (int j=5; j <= 50000; j*=10)
        cout << "./generator " << i << " " << j << " > " << "Test_" << i << "_" << j<< endl;
    }
    auto *s = (new int [5]);
}