#include <iostream>
#include <queue>
#include <getopt.h>

using namespace std;

enum BUY_OR_SELL {
    BUY, SELL
};

struct order {
    int TIMESTAMP;
    string CLIENT_NAME;
    BUY_OR_SELL buy_or_sell;
    string EQUITY_SYMBOL;
    int PRICE;
    int QUANTITY;
    int DURATION;
};

int main(int argc, char *argv[]) {
    bool verbose, median, midpoint, transfers, ttt;
    while (1) {
        static struct option long_option[] = {{"median",    no_argument,       NULL, 'm'},
                                              {"verbose",   no_argument,       NULL, 'v'},
                                              {"midpoint",  no_argument,       NULL, 'p'},
                                              {"transfers", no_argument,       NULL, 't'},
                                              {"ttt",       required_argument, NULL, 'g'},
                                              {0, 0, 0,                              0}};
        auto c = getopt_long(argc, argv, "mvptg:", long_option, NULL);
        if (c == -1) break;
        if (c == 'v') {
            verbose = true;
        }
        else if (c == 'm') {
            median = true;
        }
        else if (c == 'p') {
            midpoint = true;
        }
        else if (c == 't') {
            transfers = true;
        }
        else if (c == 'g') {
            ttt = true;
        }

    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}