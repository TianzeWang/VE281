#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <list>
#include "priority_queue.h"
#include "fib_heap.h"
#include "unsorted_heap.h"
#include "priority_queue.h"
using namespace std;

struct pos {
    int height;
    int width;
};

struct Map {
    int path_cost = -1;
    int weight = 0;
    bool is_reached = 0;
    pos *P = nullptr;
    Map *Predecessor = nullptr;
};

class Priority_Queue : public vector<Map> {
public:
    virtual Map dequeueMin()=0;

    virtual Map enqueue(Map a)=0;
};

/*int Priority_Queue::dequeueMin() {
    int i;
    auto temp = this->begin();
    for (i=0;i<this->size();i++){
        swap(, temp);
    }
}*/

void Routing_Algorithm(Map **W, int start_x, int start_y, int end_x, int end_y, int WIDTH, int HEIGHT);

void trace_back_path();

void Routing_Algorithm(Map **W, int start_x, int start_y, int end_x, int end_y, int WIDTH, int HEIGHT) {
    Priority_Queue PQ;
    W[start_x][start_y].path_cost = W[start_x][start_y].weight;
    W[start_x][start_y].is_reached = 1;
    PQ.enqueue(W[start_x][start_y]);
    while (!PQ.empty()) {
        auto C = PQ.dequeueMin();
        Map *N;
        for (int i = 1; i < 5; i++) {
            if (i == 1 && C.P->width + 1 < WIDTH && C.is_reached != 1) {
                N = &W[C.P->height][C.P->width + 1];
                N->path_cost = C.path_cost + N->weight;
                N->is_reached = 1;
                *N->Predecessor = C;
            }
            if (i == 2 && C.P->height + 1 < HEIGHT && C.is_reached != 1) {
                N = &W[C.P->height + 1][C.P->width];
                N->path_cost = C.path_cost + N->weight;
                N->is_reached = 1;
                *N->Predecessor = C;
            }
            if (i == 3 && C.P->width - 1 >= 0 && C.is_reached != 1) {
                N = &W[C.P->height][C.P->width - 1];
                N->path_cost = C.path_cost + N->weight;
                N->is_reached = 1;
                *N->Predecessor = C;
            }
            if (i == 4 && C.P->height - 1 >= 0 && C.is_reached != 1) {
                N = &W[C.P->height - 1][C.P->width];
                N->path_cost = C.path_cost + N->weight;
                N->is_reached = 1;
                *N->Predecessor = C;
            }
        }
        if (N->P->width == end_x && N->P->height == end_y) {
            trace_back_path();
            return;
        } else PQ.enqueue(*N);
    }
}

// Needs to be finished later
void trace_back_path() {

}

int main() {
    //Read the array
    int width, height;
    cin >> width;
    cin >> height;
    int start_x, start_y, end_x, end_y;
    cin >> start_x >> start_y;
    cin >> end_x >> end_y;
    int i;
    Map **W = new Map *[height];
    for (i = 0; i < height; i++) {
        W[i] = new Map[width];
    }
    for (i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cin >> W[i][j].weight;
            W[i][j].P->height = i;
            W[i][j].P->width = j;
        }
    }

    Routing_Algorithm(W, start_x, start_y, end_x, end_y, width, height);
//For debug
    /*   for (i = 0; i < height; i++) {
           for (int j = 0; j < width; j++) {
               cout << W[i][j].weight << " ";
           }
           cout << endl;
       }*/
}

