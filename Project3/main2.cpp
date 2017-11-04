#include <iostream>
#include "priority_queue.h"
#include <getopt.h>
//#include "fib_heap.h"
#include "binary_heap.h"
#include "unsorted_heap.h"

using namespace std;

struct pos {
    int height;
    int width;
};

struct Map {
    int path_cost = -1;
    int weight = 0;
    bool is_reached = 0;
    pos P;
    Map *Predecessor = NULL;

    struct compare_t {
        bool operator()(Map a, Map b) const {
            if (a.path_cost != b.path_cost) return a.path_cost > b.path_cost;
            else if (a.P.height != b.P.height) return a.P.height < b.P.height;
            else return a.P.width <= b.P.width;
        }
    };
};

void nonverbose_print(Map *a);

void nonverbose_print(Map *a) {
    if (a->Predecessor != NULL) {
        nonverbose_print(a->Predecessor);
    }

    cout << "(" << a->P.width << ", " << a->P.height << ")" << endl;
}


//void verbose_print(Map *a, int step);
//
//void verbose_print(Map *a, int step) {
//    cout << "Step " << step << endl;
//    cout << "Choose cell (" << a->P.width << "," <<
//}

int main(int argc, char *argv[]) {
    //Read the array
    string str;
    bool verbose = false;
    string implementation;
    while (1) {
        static struct option long_option[] = {{"implementation", required_argument, NULL, 'i'},
                                              {"verbose",        no_argument,       NULL, 'v'},
                                              {0, 0, 0,                                   0}};
        auto c = getopt_long(argc, argv, "vi:", long_option, NULL);
        if (c == -1) break;

//        switch (c) {
//            case 'i':
//                implementation = optarg;
//            case 'v':
//                verbose = true;
//            case (-1):
//                break;
//            default:
//                break;
//        }
        if (c == 'v') {
            verbose = true;
        }
        else if (c == 'i') {
            implementation = optarg;
        }
    }

//        break;// for debug only
// }

/*
    Note that it's in the form of W[y][x]
*/

    int width, height;
    cin >> width;
    cin >> height;
    int start_x, start_y, end_x, end_y;
    cin >> start_x >> start_y;
    cin >> end_x >> end_y;
    int i;
    auto **W = new Map *[height];
    for (i = 0; i < height; i++) {
        W[i] = new Map[width];
    }
    for (i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cin >> W[i][j].weight;
            W[i][j].P.height = i;
            W[i][j].P.width = j;
        }
    }
    priority_queue<Map, Map::compare_t> *PQ;
    if (implementation == "BINARY") {
        PQ = new binary_heap<Map, Map::compare_t>();
    }
    else if (implementation == "UNSORTED") {
        PQ = new unsorted_heap<Map, Map::compare_t>();
    }
    else if (implementation == "FIBONACCI") {
//            PQ = new fib_heap<Map, Map::compare_t>();
    }

// Here begins to Rounting Algorithm
// **********
// **********
// **********
// **********
// **********
//
    int step = 0;
    W[start_y][start_x].path_cost = W[start_y][start_x].weight;
    W[start_y][start_x].is_reached = true;
    PQ->enqueue(W[start_y][start_x]);
    while (!PQ->empty()) {
        Map C = PQ->dequeue_min();
        if (verbose) {
            cout << "Step " << step << endl;
            cout << "Choose cell (" << C.P.width << ", " << C.P.height << ") with accumulated length " << C.path_cost
                 << endl;
        }
        step++;
        Map *N;
        for (int i = 1; i < 5; i++) {
            if (i == 1 && C.P.width + 1 < width) {
                N = &W[C.P.height][C.P.width + 1];
                if (N->is_reached != 1) {
                    N->path_cost = C.path_cost + N->weight;
                    N->is_reached = true;
                    N->Predecessor = &W[C.P.height][C.P.width];
                }
                else continue;
            }
            else if (i == 2 && C.P.height + 1 < height) {
                N = &W[C.P.height + 1][C.P.width];
                if (N->is_reached != 1) {
                    N->path_cost = C.path_cost + N->weight;
                    N->is_reached = true;
//                    N->Predecessor = &C;
                    N->Predecessor = &W[C.P.height][C.P.width];
                }
                else continue;
            }
            else if (i == 3 && C.P.width - 1 >= 0) {
                N = &W[C.P.height][C.P.width - 1];
                if (N->is_reached != 1) {
                    N->path_cost = C.path_cost + N->weight;
                    N->is_reached = true;
                    N->Predecessor = &W[C.P.height][C.P.width];
                }
                else continue;
            }
            else if (i == 4 && C.P.height - 1 >= 0) {
                N = &W[C.P.height - 1][C.P.width];
                if (N->is_reached != 1) {
                    N->path_cost = C.path_cost + N->weight;
                    N->is_reached = true;
                    N->Predecessor = &W[C.P.height][C.P.width];
                }
                else continue;
            }
            else continue;

            if (N->P.width == end_x && N->P.height == end_y) {

                //Here Begins the function of Trace back
                //**************
                //**************
                //**************
                //**************
                //**************
                //**************
                cout << "Cell (" << end_x << ", " << end_y << ") with accumulated length " << W[end_y][end_x].path_cost
                     << " is the ending point." << endl;
                cout << "The shortest path from (" << start_x << ", " << start_y << ") to (" << end_x << ", " << end_y
                     << ") ";
                cout << "is " << W[end_y][end_x].path_cost << "." << endl;
                cout << "Path:" << endl;
                Map c = W[end_y][end_x];
                nonverbose_print(&c);
                return 0;
            }
            else {
                PQ->enqueue(*N);
                if (verbose) {
                    cout << "Cell (" << N->P.width << ", " << N->P.height << ") with accumulated length "
                         << N->path_cost << " is added into the queue." << endl;
                }
            }
        }
    }
//For debug
    /*   for (i = 0; i < height; i++) {
           for (int j = 0; j < width; j++) {
               cout << W[i][j].weight << " ";
           }
           cout << endl;
       }*/
    return 0;
}


