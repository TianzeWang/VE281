//
// Created by 大泽 on 2017/12/1.
//

#include <iostream>
#include <cstdlib>
#include <random>

using namespace std;

int main (){
    cout << 125 << endl;
    cout << 0 <<endl;
    cout << 125 << endl;

    for (int i = 0; i < 125; i++){
        for (int j =0; j <i ; j++){
            auto temp = rand() % 566;
            cout << i << " " << j << " " << temp;
        }
    }
}