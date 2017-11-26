#include <iostream>
#include <queue>
#include <getopt.h>
#include <set>
#include <map>


using namespace std;

enum BUY_OR_SELL {
    BUY, SELL
};

struct order {
    int TIMESTAMP;
    int id;
    string CLIENT_NAME;
    BUY_OR_SELL buy_or_sell;
    string EQUITY_SYMBOL;
    int PRICE;
    int QUANTITY;
    int DURATION;
    bool isdone = false;
};

struct order_compare {
    bool operator()(order &a, order &b) {
        if (a.TIMESTAMP == b.TIMESTAMP) return a.id < b.id;
        else return a.TIMESTAMP < b.TIMESTAMP;
    }
};

struct compare_buy_order {
    bool operator()(order &a, order &b) {
        if (a.PRICE == b.PRICE) return a.id < b.id;
        else return a.PRICE > b.PRICE;
    }

    bool operator()(const order &a, const order &b) {
        if (a.PRICE == b.PRICE) return a.id < b.id;
        else return a.PRICE > b.PRICE;
    }
};

struct compare_sell_order {
    bool operator()(order &a, order &b) {
        if (a.PRICE == b.PRICE) return a.id < b.id;
        else return a.PRICE < b.PRICE;
    }

    bool operator()(const order &a, const order &b) {
        if (a.PRICE == b.PRICE) return a.id < b.id;
        else return a.PRICE < b.PRICE;
    }
};

struct Client {
    string client_name;
    int stock_buy;
    int stock_sell;
    int amount_traded;
};

struct Equity {
    int id;
    int buy_time;
    int sell_time;
    int buy_price;
    int sell_price;
    string Equity_NAME;
};

struct Big_Order {
    string EQUITY_SYMBOL;
    multiset<order, compare_buy_order> Buy;
    multiset<order, compare_sell_order> Sell;
    multiset<int> Price_dealt;
};

struct compare_big_order {
    bool operator()(order &a, order &b) {
        return a.EQUITY_SYMBOL < b.EQUITY_SYMBOL;
    }

    bool operator()(const order &a, const order &b) {
        return a.EQUITY_SYMBOL < b.EQUITY_SYMBOL;
    }
};

void midpoint();

void median();

void Deal_With_Expired_Order();


int main(int argc, char *argv[]) {
    bool verbose = false, median = false, midpoint = false, transfers = false, ttt = false;

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
    int current_timestamp = 0, id = 0;
    int timestamp;
    string client_name;
    string buy_or_sell1;
    string equity_symbol;
    int price;
    int quantity;
    int duration;
    char note;// used to indicate # and $
    multiset<order, compare_buy_order> Buy;
    multiset<order, compare_sell_order> Sell;
//    multiset<order, std::less>::iterator Buy_iter, Sell_iter;
//    multiset<order, order_compare> OrderALL;
//    multiset<order, order_compare>::iterator it;
    multiset<Big_Order, compare_big_order> OrderAll;
    multiset<Big_Order, compare_big_order>::iterator it;
    multiset<order, compare_buy_order>::iterator BuyIt;
    multiset<order, compare_sell_order>::iterator SellIt;

    char c;

    // Read
    while ((cin >> timestamp).get(c)) {
        if (c == '\n') break;
        order Read_temp;
        cin >> client_name >> buy_or_sell1 >> equity_symbol >> note >> price >> note >> quantity >> duration;
        Read_temp.id = id;
        Read_temp.TIMESTAMP = timestamp;
        Read_temp.CLIENT_NAME = client_name;
        Read_temp.DURATION = duration;
        Read_temp.EQUITY_SYMBOL = equity_symbol;
        Read_temp.QUANTITY = quantity;
        Read_temp.DURATION = duration;
        if (buy_or_sell1 == "BUY") {
            Read_temp.buy_or_sell = BUY;
            Sell.insert(Read_temp);
        }
        else {
            Read_temp.buy_or_sell = SELL;
            Buy.insert(Read_temp);
        }
        id++;

        // Sell and Buy and match
        midpoint();
        median();

        current_timestamp = timestamp;

        //Deal_With_Expired_Order();
        //Erase from temp
        /*
         *
        for (SellIt = Sell.begin(); SellIt != Sell.end(); SellIt++) {
            if (SellIt->DURATION != -1 && SellIt->DURATION + SellIt->TIMESTAMP <= current_timestamp) {
                Sell.erase(SellIt);
            }
        }

        for (BuyIt = Buy.begin(); BuyIt != Buy.end(); BuyIt++) {
            if (BuyIt->DURATION != -1 && BuyIt->DURATION + BuyIt->TIMESTAMP <= current_timestamp) {
                Buy.erase(BuyIt);
            }
        }
        *
        */

        //Erase from OrderAll
        for (it = OrderAll.begin(); it != OrderAll.end(); it++) {
            for (SellIt = (it->Sell).begin(); SellIt != (it->Sell).end(); SellIt++) {
                if (SellIt->DURATION != -1 && SellIt->DURATION + SellIt->TIMESTAMP <= current_timestamp) {
                    it->Sell.erase(SellIt);
                }
            }
            for (BuyIt = (it->Buy).begin(); BuyIt != (it->Sell).end(); BuyIt++) {
                if (BuyIt->DURATION != -1 && BuyIt->DURATION + BuyIt->TIMESTAMP <= current_timestamp) {
                    it->Buy.erase(BuyIt);
                }
            }
        }

        // Match the order:
        //  Logics: 1. Deal with Buy order
        //          2. Check whether the orders in Sell can be matched with buy (!isdone)
        //          3. Divide into two cases: all bought/ partial bought
        //          4. If all bought, two cases: once or several times
        //          5. If partial, add the remaining part to the order book.
        for (it = OrderAll.begin(); it != OrderAll.end(); it++) {
            if (it->EQUITY_SYMBOL == equity_symbol) { // Match and Dealt
                // Match and Dealt the order. already excluded the expired items deletion before.

                // Case A: Buy order comes
                // First judge the existing of Sell order

                /* Can get Optimized by using a new data structure only to store the current trading*/

                if (Read_temp.buy_or_sell == BUY) {
                    for (SellIt = (it->Sell).begin(); SellIt != (it->Sell).end(); SellIt++) {
                        // Then judge in loog, for Sell QUAN < Buy QUAN, should stop as long as the temp is done
                        if (Read_temp.isdone) break;
                        else if (!SellIt->isdone) {
                            // Case A.1, Sell's QUAN >= Buy's QUAN, which is always the final case.
                            if (SellIt->QUANTITY >= Read_temp.QUANTITY && Read_temp.PRICE > SellIt->PRICE) {
                                SellIt->QUANTITY -= Read_temp.QUANTITY;
                                Read_temp.QUANTITY = 0;
                                Read_temp.isdone = true;
                                if (SellIt->QUANTITY == 0) {
                                    SellIt->isdone = 1;
                                }
                                it->Price_dealt.insert(Read_temp.PRICE);
                            }
                            // Case A.2, Sell's QUAN < Buy's QUAN, will recursive to Case A.1 or to Case A.3
                            if (SellIt->QUANTITY <= Read_temp.QUANTITY && Read_temp.PRICE > SellIt->PRICE) {
                                SellIt->QUANTITY = 0;
                                SellIt->isdone = 1;
                                Read_temp.QUANTITY -= SellIt->QUANTITY;
                            }
                        }
                    }
                    // Case A.3
                    if (!Read_temp.isdone && Read_temp.DURATION != 0) {
                        it->Buy.insert(Read_temp);
                    }
                }
                    //Case B: Sell Order Comes
                else {
                    for (BuyIt = (it->Buy).begin(); BuyIt != (it->Buy).end(); BuyIt++) {
                        if (Read_temp.isdone) break;
                        else if (!BuyIt->isdone) {
                            if (BuyIt->QUANTITY >= Read_temp.QUANTITY && Read_temp.PRICE < BuyIt->PRICE) {
                                BuyIt->QUANTITY -= Read_temp.QUANTITY;
                                Read_temp.QUANTITY = 0;
                                Read_temp.isdone = true;
                                if (BuyIt->QUANTITY == 0) {
                                    BuyIt->isdone = 1;
                                }
                                it->Price_dealt.insert(Read_temp.PRICE);
                            }
                            // Case B.2
                            if (BuyIt->QUANTITY <= Read_temp.QUANTITY && Read_temp.PRICE < BuyIt->PRICE) {
                                BuyIt->QUANTITY = 0;
                                BuyIt->isdone = 0;
                                Read_temp.QUANTITY -= BuyIt->QUANTITY;
                            }
                        }
                    }
                    if (!Read_temp.isdone && Read_temp.DURATION != 0) {
                        it->Sell.insert(Read_temp);
                    }
                }
                // Case C: Reamins some undealt orders, should be put back

            }
                // Remain another case that dealts without

            else { // Not found and create
                Big_Order bigorderTemp;
                if (buy_or_sell1 == "BUY") {
                    bigorderTemp.Buy.insert(Read_temp);
                }
                else {
                    bigorderTemp.Sell.insert(Read_temp);
                }
                bigorderTemp.EQUITY_SYMBOL = Read_temp.EQUITY_SYMBOL;
                OrderAll.insert(bigorderTemp);
            }
        }

    }

    // At the end of day, output
    cout << "---End of Day---" << endl;
    cout << "Commission Earnings: " << endl;
    cout << "Total Amount of Money Transferred: " << endl;
}

//void Add_to_Sell (order Temp, )
