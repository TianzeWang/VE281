
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include <iostream> // for cin, cout and cerr
#include <getopt.h> // required for getopt_long
#include <string>   // for strings


using namespace std;

struct tttEquity {
    int priority;
    string symbol;
    int buyTimeStamp;
    int sellTimeStamp;
    int buyPrice;
    int sellPrice;
};

struct compare_tttEquity {
    // for binary queue "less than" comparision

    // non-const version
    bool operator()(tttEquity & a, tttEquity & b) const {
        if (a.priority < b.priority)
            return true;
        else
            return false;
    }
    // const version
    bool operator()(const tttEquity & a, const tttEquity & b) const {
        if (a.priority < b.priority)
            return true;
        else
            return false;
    }
};

struct Order {
    int uniqueID;
    string clientName;
    int price;
    int amount;
    int expireAt;   // = current timestamp + DURATION
};

struct compare_sellOrder {
    // for priority queue "less than" comparision

    // non-const version
    bool operator()(Order & a, Order & b) const {
        if (a.price < b.price) {
            return true;
        } else if (a.price == b.price) {
            if (a.uniqueID < b.uniqueID) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    // const version
    bool operator()(const Order & a, const Order & b) const {
        if (a.price < b.price) {
            return true;
        } else if (a.price == b.price) {
            if (a.uniqueID < b.uniqueID) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};

struct compare_buyOrder {
    // for priority queue "less than" comparision

    // non-const version
    bool operator()(Order & a, Order & b) const {
        if (a.price > b.price) {
            return true;
        } else if (a.price == b.price) {
            if (a.uniqueID < b.uniqueID) {
                return true;
            } else {
                return false;
            }
        } else { // a.price > b.price
            return false;
        }
    }
    // const version
    bool operator()(const Order & a, const Order & b) const {
        if (a.price > b.price) {
            return true;
        } else if (a.price == b.price) {
            if (a.uniqueID < b.uniqueID) {
                return true;
            } else {
                return false;
            }
        } else { // a.price > b.price
            return false;
        }
    }
};

struct EquityBook {
    string equitySymbol;
    multiset<Order, compare_sellOrder> sellOrderBook;
    multiset<Order, compare_buyOrder> buyOrderBook;
    multiset<int> matchedPriceHistory;
};

struct compare_equityBook {
    // for BST "less than" comparision in lexicographic order

    // non-const version
    bool operator()(EquityBook & a, EquityBook & b) const {
        if (a.equitySymbol < b.equitySymbol) {
            return true;
        } else {
            return false;
        }
    }
    // const version
    bool operator()(const EquityBook & a, const EquityBook & b) const {
        if (a.equitySymbol < b.equitySymbol) {
            return true;
        } else {
            return false;
        }
    }
};

struct clientProfile {
    string clientName = "";
    int countStockBought;
    int countStockSold;
    int netValueTraded;
};

struct compare_clientProfile {
    // for BST "less than" comparision in lexicographic order

    // non-const version
    bool operator()(clientProfile & a, clientProfile & b) const {
        if (a.clientName < b.clientName) {
            return true;
        } else {
            return false;
        }
    }
    // const version
    bool operator()(const clientProfile & a, const clientProfile & b) const {
        if (a.clientName < b.clientName) {
            return true;
        } else {
            return false;
        }
    }
};

int main(int argc, char ** argv) {
    // Used to generate running time info for the report.
    // comment out for submission
//        clock_t timer;
//        timer = clock();

    // step 1. read command line argument
    bool isVerbose = false;
    bool isMedian = false;
    bool isMidPoint = false;
    bool isTransfers = false;

    // helper varialbe for Time-Traveller Trading output
    int feedbackGetOpt = 0;
    int priority = 0;
    multiset<tttEquity, compare_tttEquity> tttEquityBST;
    multiset<tttEquity, compare_tttEquity>::iterator itrtttEquityBST;
    tttEquity * ptrTTTEquity;
    tttEquity tempEquity;

    while (1) {
        static struct option long_options[] = {
                {"verbose", no_argument, 0, 'v'},
                {"median", no_argument, 0, 'm'},
                {"midpoint", no_argument, 0, 'p'},
                {"transfers", no_argument, 0, 't'},
                {"ttt", required_argument, 0, 'g'},
                {0, 0, 0, 0}
        };

        int option_index = 0;
        feedbackGetOpt = getopt_long(argc, argv, "vmptg:", long_options, &option_index);

        if (feedbackGetOpt == -1) {
            break;
        }

        switch (feedbackGetOpt) {
            case 'v':
                isVerbose = true;
                break;
            case 'm':
                isMedian = true;
                break;
            case 'p':
                isMidPoint = true;
                break;
            case 't':
                isTransfers = true;
                break;
            case 'g': {
                // store EQUIY_SYMBOL somewhere; one by one
                tempEquity.priority = priority;
                tempEquity.symbol = optarg;
                tempEquity.buyTimeStamp = -1;
                tempEquity.sellTimeStamp = -1;
                tempEquity.buyPrice = 0;
                tempEquity.sellPrice = 0;
                tttEquityBST.insert(tempEquity);

                priority++;
                break;
            }
            default:
                cerr << "Invalid command line arguments. Exiting program...\n";
                return -1;
        }
    } // end of while(1)

    // DEBUG code
//    cout << "Command line reading complete.\n"
//    << "isVerbose = " << isVerbose << endl
//    << "isMedian = " << isMedian << endl
//    << "isMidPoint = " << isMidPoint << endl
//    << "isTransfers = " << isTransfers << endl;

    // step 2. read cin
    int currentTimeStamp = 0;
    int nextUniqueID = 0;

    int TIMESTAMP = 0;
    string CLIENT_NAME = "";
    string BUY_OR_SELL = "";
    bool isBuy = false;
    string EQUITY_SYMBOL = "";
    int PRICE = 0;
    int QUANTITY = 0;
    int DURATION = 0;
    char temp = '0';

    // trading statisticsx
    int matchPrice = 0;
    int singleSideComission = 0;
    int aggregateCommission = 0;
    int moneyTransferred = 0;
    int countCompletedTrade = 0;
    int countSharesTraded = 0;

    multiset<EquityBook, compare_equityBook> orderBook;
    multiset<EquityBook, compare_equityBook>::iterator itrOrderBook;
    EquityBook * ptrEquityBook;
    multiset<Order, compare_sellOrder> * ptrSellOrderBook;
    multiset<Order, compare_buyOrder> * ptrbuyOrderBook;
    Order * ptrOrder;
    EquityBook tempEquityBook;
    Order tempOrder;
    multiset<Order, compare_sellOrder>::iterator itrSellOrderBook;
    multiset<Order, compare_sellOrder>::iterator tempItrSellOrderBook;
    multiset<Order, compare_buyOrder>::iterator itrBuyOrderBook;
    multiset<Order, compare_buyOrder>::iterator tempItrBuyOrderBook;

    // helper variable for transfers output
    multiset<clientProfile, compare_clientProfile> clientList;
    multiset<clientProfile, compare_clientProfile>::iterator itrClientList;
    clientProfile tempClientProfile;
    clientProfile * ptrClientProfile;
    bool buyerFound = false;
    bool sellerFound = false;

    // helper variable for median output
    multiset<int>::iterator itrMedianMatchedPrice;
    int sizeInInt = 0;
    bool isEven = false;
    int medianPrice = 0;

    // helper variable for midpoint output
    int midPoint = 0;
    int highestBuy = 0;
    int lowestSell = 0;

    // DEBUG output
    // cout << "I am right beofre while (cin >> TIMESTAMP)" << endl;

    // 4. repeat until the end of the day
    while (cin >> TIMESTAMP) {
        // 1. read the next order from input
        cin >> CLIENT_NAME >> BUY_OR_SELL >> EQUITY_SYMBOL
            >> temp >> PRICE >> temp >> QUANTITY >> DURATION;
        if (BUY_OR_SELL == "BUY") {
            isBuy = true;
        } else if (BUY_OR_SELL == "SELL") {
            isBuy = false;
        } else {
            cerr << "Invalid BUY/SELL input. Exiting program.." << endl;
            return -1;
        }

        // DEBUG output
//         cout << currentTimeStamp
//            << " I have read order from " << CLIENT_NAME
//            << " for " << EQUITY_SYMBOL << endl;
//        cout << "Complte read: "
//            << TIMESTAMP << " " << CLIENT_NAME << " " << BUY_OR_SELL << " " << EQUITY_SYMBOL << " "
//            << PRICE << " " << QUANTITY << endl;

        // Time-Traveller Trading
        for (itrtttEquityBST = tttEquityBST.begin(); itrtttEquityBST != tttEquityBST.end(); itrtttEquityBST++) {
            if (itrtttEquityBST->symbol == EQUITY_SYMBOL) {
                ptrTTTEquity = const_cast<tttEquity *>(&(* itrtttEquityBST));
                if (!isBuy) { // this is a sell order, time-travelled can buy
                    // if we have nothing yet OR if we can buy at a lower price
                    if ((ptrTTTEquity->buyTimeStamp == -1) || (ptrTTTEquity->buyPrice > PRICE)) {
                        ptrTTTEquity->buyTimeStamp = TIMESTAMP;
                        ptrTTTEquity->buyPrice = PRICE;
                        // DEBUG output
//                        cout << "Buy timestamp changed to " << ptrTTTEquity->buyTimeStamp
//                            << " and buy price changed to " <<  ptrTTTEquity->buyPrice << endl;
                        if (ptrTTTEquity->sellTimeStamp != -1) {
                            // DEBUG output
//                            cout << "Margin now is " << ptrTTTEquity->maxMargin << endl;
                        }
                    }
                } else { // this is a buy order, time-traverlled can sell
                    if (ptrTTTEquity->buyTimeStamp == -1) {
                        // we do not have anything to sell
                        // DEBUG output
//                        cout << "Buy order from " << CLIENT_NAME
//                        << " ignored as ttt does not own shares." << endl;

                        break; // wait for next order
                    }
                    // if we do not have any offer yet OR if we can sell at a higher price
                    if ((ptrTTTEquity->sellTimeStamp == -1) || (ptrTTTEquity->sellPrice < PRICE)) {
                        ptrTTTEquity->sellTimeStamp = TIMESTAMP;
                        ptrTTTEquity->sellPrice = PRICE;
                        // down here we are sure there exists a buy order

                        // DEBUG output
//                        cout << "Sell timestamp changed to " << ptrTTTEquity->sellTimeStamp
//                            << " and sell price changed to " <<  ptrTTTEquity->sellPrice << endl
//                            << "Margin now is " << ptrTTTEquity->maxMargin << endl;
                    }
                }
            }
        }


        // 2. if order TIMESTAMP != CURRENT_TIMESTAMP
        if (TIMESTAMP != currentTimeStamp) {
            // 2.1 print --median if specified in command line
            if (isMedian) {
                for (itrOrderBook = orderBook.begin(); itrOrderBook != orderBook.end(); itrOrderBook++) {
                    sizeInInt = (int)itrOrderBook->matchedPriceHistory.size();
                    if (sizeInInt == 0) {
                        continue;
                    }
                    if (sizeInInt % 2 == 0) {
                        isEven = true;
                    } else {
                        isEven = false;
                    }
                    sizeInInt = sizeInInt / 2;
                    itrMedianMatchedPrice = itrOrderBook->matchedPriceHistory.begin();
                    std::advance(itrMedianMatchedPrice, sizeInInt);
                    if (isEven) {
                        medianPrice = *itrMedianMatchedPrice;       // the middle
                        medianPrice += *(--itrMedianMatchedPrice);  // + the one before that
                        medianPrice = medianPrice / 2;
                    } else { // size is odd
                        medianPrice = *itrMedianMatchedPrice;
                    }
                    cout << "Median match price of "
                         << itrOrderBook->equitySymbol
                         << " at time "
                         << currentTimeStamp
                         << " is $"
                         << medianPrice << endl;
                }
            }

            // 2.2 print --midpoint if specifed in command line
            if (isMidPoint) {
                for (itrOrderBook = orderBook.begin(); itrOrderBook != orderBook.end(); itrOrderBook++) {
                    if (itrOrderBook->sellOrderBook.empty() || itrOrderBook->buyOrderBook.empty()) {
                        cout << "Midpoint of "
                             << itrOrderBook->equitySymbol
                             << " at time "
                             << currentTimeStamp
                             << " is undefined"
                             << endl;
                        continue;
                    }
                    highestBuy = (* itrOrderBook->buyOrderBook.begin()).price;
                    lowestSell = (* itrOrderBook->sellOrderBook.begin()).price;
                    midPoint = (highestBuy + lowestSell) / 2;
                    cout << "Midpoint of "
                         << itrOrderBook->equitySymbol
                         << " at time "
                         << currentTimeStamp
                         << " is $"
                         << midPoint
                         << endl;
                }
            }

            // 2.3 set CURRENT_TIMESTAMP euqal to order's TIMESTAMP
            currentTimeStamp = TIMESTAMP;
            // 2.4 remove expired order from data structure
            for (itrOrderBook = orderBook.begin(); itrOrderBook != orderBook.end(); itrOrderBook++) {
                // now we are at individual equity book level. Each describes a specific equity.
                // first remove constant-ness
                ptrSellOrderBook = const_cast<multiset<Order, compare_sellOrder>* >(&(itrOrderBook->sellOrderBook));
                ptrbuyOrderBook = const_cast<multiset<Order, compare_buyOrder>* >(&(itrOrderBook->buyOrderBook));
                // then examine sell order
                for (itrSellOrderBook = ptrSellOrderBook->begin(); itrSellOrderBook != ptrSellOrderBook->end(); itrSellOrderBook++) {
                    // now we are at individual orders
                    if ((itrSellOrderBook->expireAt != -1) && (currentTimeStamp >= itrSellOrderBook->expireAt)) {
                        tempItrSellOrderBook = itrSellOrderBook;
                        ptrSellOrderBook->erase(tempItrSellOrderBook);
                    }
                }
                // finally, examin buy order
                for (itrBuyOrderBook = ptrbuyOrderBook->begin(); itrBuyOrderBook != ptrbuyOrderBook->end(); itrBuyOrderBook++) {
                    // now we are at individual orders
                    if ((itrBuyOrderBook->expireAt != -1) && (currentTimeStamp >= itrBuyOrderBook->expireAt)) {
                        tempItrBuyOrderBook = itrBuyOrderBook;
                        ptrbuyOrderBook->erase(tempItrBuyOrderBook);
                    }
                }
            }
        }
        // DEBUG output
        // cout << "I have reached step 3" << endl;

        // 3. execute order
        // first find the orderbook for given equity through linear search
        for (itrOrderBook = orderBook.begin(); itrOrderBook != orderBook.end(); itrOrderBook++) {
            if (itrOrderBook->equitySymbol == EQUITY_SYMBOL) {
                // equity found
                if (isBuy) { // isBuy is true
                    // then check seller BST, first remove constant-ness
                    ptrSellOrderBook = const_cast<multiset<Order, compare_sellOrder>* >(&(itrOrderBook->sellOrderBook));
                    while ((QUANTITY > 0) && (!ptrSellOrderBook->empty())) {
                        if ((* ptrSellOrderBook->begin()).price <= PRICE ) {
                            // fist remove constant-ness
                            ptrOrder = const_cast<Order *>(&(* ptrSellOrderBook->begin()));

                            // calculate match price & record for median output
                            if (ptrOrder->uniqueID < nextUniqueID) {
                                matchPrice = ptrOrder->price;
                            } else {
                                matchPrice = PRICE;
                            }
                            ptrEquityBook = const_cast<EquityBook *>(&(*itrOrderBook));
                            ptrEquityBook->matchedPriceHistory.insert(matchPrice);

                            if (ptrOrder->amount > QUANTITY) {
                                // scenario 1. supply > demand
                                // OUTPUT something
                                if (isTransfers) {
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == CLIENT_NAME) {
                                            // buyer found, update value
                                            buyerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockBought += QUANTITY;
                                            ptrClientProfile->netValueTraded -= QUANTITY * matchPrice;

                                        }
                                        if (buyerFound) {
                                            break;
                                        }
                                    }
                                    // if not in the list, then create new ones
                                    if (!buyerFound) {
                                        // create profile for buyer
                                        tempClientProfile.clientName = CLIENT_NAME;
                                        tempClientProfile.countStockBought = QUANTITY;
                                        tempClientProfile.countStockSold = 0;
                                        tempClientProfile.netValueTraded = -1 * QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == ptrOrder->clientName) {
                                            // seller found, update value
                                            sellerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockSold += QUANTITY;
                                            ptrClientProfile->netValueTraded += QUANTITY * matchPrice;
                                        }
                                        if (sellerFound) {
                                            break;
                                        }
                                    }
                                    // if not in the list, then create new ones
                                    if (!sellerFound) {
                                        // create profile for seller
                                        tempClientProfile.clientName = ptrOrder->clientName;
                                        tempClientProfile.countStockBought = 0;
                                        tempClientProfile.countStockSold = QUANTITY;
                                        tempClientProfile.netValueTraded = QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    buyerFound = false;
                                    sellerFound = false;
                                }
                                if (isVerbose) {
                                    cout << CLIENT_NAME // BUYER_NAME
                                         << " purchased "
                                         << QUANTITY // NUMBER_OF_SHARES
                                         << " shares of "
                                         << EQUITY_SYMBOL // EQUITY_SYMBOL
                                         << " from "
                                         << ptrOrder->clientName // SELLER_NAME
                                         << " for $"
                                         << matchPrice // PRICE
                                         << "/share" << endl;
                                }
                                countSharesTraded += QUANTITY;
                                countCompletedTrade++;
                                moneyTransferred += matchPrice * QUANTITY;
                                singleSideComission = matchPrice * QUANTITY / 100;
                                aggregateCommission += singleSideComission * 2;
                                ptrOrder->amount = ptrOrder->amount - QUANTITY;
                                QUANTITY = 0;
                            } else if (ptrOrder->amount == QUANTITY) {
                                // scenario 2. supply == demand
                                // OUTPUT something
                                if (isTransfers) {
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == CLIENT_NAME) {
                                            // buyer found, update value
                                            buyerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockBought += QUANTITY;
                                            ptrClientProfile->netValueTraded -= QUANTITY * matchPrice;

                                        }
                                        if (buyerFound) {
                                            break;
                                        }
                                    } // end of for loop
                                    // if not in the list, then create new ones
                                    if (!buyerFound) {
                                        // create profile for buyer
                                        tempClientProfile.clientName = CLIENT_NAME;
                                        tempClientProfile.countStockBought = QUANTITY;
                                        tempClientProfile.countStockSold = 0;
                                        tempClientProfile.netValueTraded = -1 * QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == ptrOrder->clientName) {
                                            // seller found, update value
                                            sellerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockSold += QUANTITY;
                                            ptrClientProfile->netValueTraded += QUANTITY * matchPrice;
                                        }
                                        if (sellerFound) {
                                            break;
                                        }
                                    } // end of for loop
                                    // if not in the list, then create new ones
                                    if (!sellerFound) {
                                        // create profile for seller
                                        tempClientProfile.clientName = ptrOrder->clientName;
                                        tempClientProfile.countStockBought = 0;
                                        tempClientProfile.countStockSold = QUANTITY;
                                        tempClientProfile.netValueTraded = QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    buyerFound = false;
                                    sellerFound = false;
                                }
                                if (isVerbose) {
                                    cout << CLIENT_NAME // BUYER_NAME
                                         << " purchased "
                                         << QUANTITY // NUMBER_OF_SHARES
                                         << " shares of "
                                         << EQUITY_SYMBOL // EQUITY_SYMBOL
                                         << " from "
                                         << ptrOrder->clientName // SELLER_NAME
                                         << " for $"
                                         << matchPrice // PRICE
                                         << "/share" << endl;
                                }
                                countSharesTraded += QUANTITY;
                                countCompletedTrade++;
                                moneyTransferred += matchPrice * QUANTITY;
                                singleSideComission = matchPrice * QUANTITY / 100;
                                aggregateCommission += singleSideComission * 2;
                                QUANTITY = 0;
                                ptrSellOrderBook->erase(ptrSellOrderBook->begin());
                            } else {
                                // scenario 3. supply < demand
                                // OUTPUT something
                                if (isTransfers) {
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == CLIENT_NAME) {
                                            // buyer found, update value
                                            buyerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockBought += ptrOrder->amount;
                                            ptrClientProfile->netValueTraded -= ptrOrder->amount * matchPrice;

                                        }
                                        if (buyerFound) {
                                            break;
                                        }
                                    }
                                    // if not in the list, then create new ones
                                    if (!buyerFound) {
                                        // create profile for buyer
                                        tempClientProfile.clientName = CLIENT_NAME;
                                        tempClientProfile.countStockBought = ptrOrder->amount;
                                        tempClientProfile.countStockSold = 0;
                                        tempClientProfile.netValueTraded = -1 * ptrOrder->amount * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == ptrOrder->clientName) {
                                            // seller found, update value
                                            sellerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockSold += ptrOrder->amount;
                                            ptrClientProfile->netValueTraded += ptrOrder->amount * matchPrice;
                                        }
                                        if (sellerFound) {
                                            break;
                                        }
                                    } // end of for loop
                                    if (!sellerFound) {
                                        // create profile for seller
                                        tempClientProfile.clientName = ptrOrder->clientName;
                                        tempClientProfile.countStockBought = 0;
                                        tempClientProfile.countStockSold = ptrOrder->amount;
                                        tempClientProfile.netValueTraded = ptrOrder->amount * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    buyerFound = false;
                                    sellerFound = false;
                                }
                                if (isVerbose) {
                                    cout << CLIENT_NAME // BUYER_NAME
                                         << " purchased "
                                         << ptrOrder->amount // NUMBER_OF_SHARES
                                         << " shares of "
                                         << EQUITY_SYMBOL // EQUITY_SYMBOL
                                         << " from "
                                         << ptrOrder->clientName // SELLER_NAME
                                         << " for $"
                                         << matchPrice // PRICE
                                         << "/share" << endl;
                                }
                                countSharesTraded += ptrOrder->amount;
                                countCompletedTrade++;
                                moneyTransferred += matchPrice * ptrOrder->amount;
                                singleSideComission = matchPrice * ptrOrder->amount / 100;
                                aggregateCommission += singleSideComission * 2;
                                QUANTITY = QUANTITY - ptrOrder->amount;
                                ptrSellOrderBook->erase(ptrSellOrderBook->begin());
                            }
                        } else {
                            // lowest selling price is still too high
                            break;
                        }
                    } // end of while loop
                } else { // isBuy is false, we have a selling customer
                    // then check buyer BST, first remove constant-ness
                    ptrbuyOrderBook = const_cast<multiset<Order, compare_buyOrder>* >(&(itrOrderBook->buyOrderBook));
                    while ((QUANTITY > 0) && (!ptrbuyOrderBook->empty())) {
                        if ((* ptrbuyOrderBook->begin()).price >= PRICE ) {
                            // fist remove constant-ness
                            ptrOrder = const_cast<Order *>(&(* ptrbuyOrderBook->begin()));

                            // calculate match price & record for median output
                            if (ptrOrder->uniqueID < nextUniqueID) {
                                matchPrice = ptrOrder->price;
                            } else {
                                matchPrice = PRICE;
                            }
                            ptrEquityBook = const_cast<EquityBook *>(&(*itrOrderBook));
                            ptrEquityBook->matchedPriceHistory.insert(matchPrice);

                            if (ptrOrder->amount > QUANTITY) {
                                // scenario 1. demand > supply
                                // OUTPUT something
                                if (isTransfers) {
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == CLIENT_NAME) {
                                            // seller found, update value
                                            sellerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockSold += QUANTITY;
                                            ptrClientProfile->netValueTraded += QUANTITY * matchPrice;

                                        }
                                        if (sellerFound) {
                                            break;
                                        }
                                    }
                                    if (!sellerFound) {
                                        // create profile for seller
                                        tempClientProfile.clientName = CLIENT_NAME;
                                        tempClientProfile.countStockBought = 0;
                                        tempClientProfile.countStockSold = QUANTITY;
                                        tempClientProfile.netValueTraded = QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == ptrOrder->clientName) {
                                            // buyer found, update value
                                            buyerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockBought += QUANTITY;
                                            ptrClientProfile->netValueTraded -= QUANTITY * matchPrice;
                                        }
                                        if (buyerFound) {
                                            break;
                                        }
                                    } // end of for loop
                                    if (!buyerFound) {
                                        // create profile for buyer
                                        tempClientProfile.clientName = ptrOrder->clientName;
                                        tempClientProfile.countStockBought = QUANTITY;
                                        tempClientProfile.countStockSold = 0;
                                        tempClientProfile.netValueTraded = -1 * QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    buyerFound = false;
                                    sellerFound = false;
                                }
                                if (isVerbose) {
                                    cout << ptrOrder->clientName // BUYER_NAME
                                         << " purchased "
                                         << QUANTITY // NUMBER_OF_SHARES
                                         << " shares of "
                                         << EQUITY_SYMBOL // EQUITY_SYMBOL
                                         << " from "
                                         << CLIENT_NAME // SELLER_NAME
                                         << " for $"
                                         << matchPrice // PRICE
                                         << "/share" << endl;
                                }
                                countSharesTraded += QUANTITY;
                                countCompletedTrade++;
                                moneyTransferred += matchPrice * QUANTITY;
                                singleSideComission = matchPrice * QUANTITY / 100;
                                aggregateCommission += singleSideComission * 2;
                                ptrOrder->amount = ptrOrder->amount - QUANTITY;
                                QUANTITY = 0;
                            } else if (ptrOrder->amount == QUANTITY) {
                                // scenario 2. demand == supply
                                // OUTPUT something
                                if (isTransfers) {
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == CLIENT_NAME) {
                                            // seller found, update value
                                            sellerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockSold += QUANTITY;
                                            ptrClientProfile->netValueTraded += QUANTITY * matchPrice;

                                        }
                                        if (sellerFound) {
                                            break;
                                        }
                                    }
                                    if (!sellerFound) {
                                        // create profile for seller
                                        tempClientProfile.clientName = CLIENT_NAME;
                                        tempClientProfile.countStockBought = 0;
                                        tempClientProfile.countStockSold = QUANTITY;
                                        tempClientProfile.netValueTraded = QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == ptrOrder->clientName) {
                                            // buyer found, update value
                                            buyerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockBought += QUANTITY;
                                            ptrClientProfile->netValueTraded -= QUANTITY * matchPrice;
                                        }
                                        if (buyerFound) {
                                            break;
                                        }
                                    } // end of for loop
                                    // if not in the list, then create new ones
                                    if (!buyerFound) {
                                        // create profile for buyer
                                        tempClientProfile.clientName = ptrOrder->clientName;
                                        tempClientProfile.countStockBought = QUANTITY;
                                        tempClientProfile.countStockSold = 0;
                                        tempClientProfile.netValueTraded = -1 * QUANTITY * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    buyerFound = false;
                                    sellerFound = false;
                                }
                                if (isVerbose) {
                                    cout << ptrOrder->clientName // BUYER_NAME
                                         << " purchased "
                                         << QUANTITY // NUMBER_OF_SHARES
                                         << " shares of "
                                         << EQUITY_SYMBOL // EQUITY_SYMBOL
                                         << " from "
                                         << CLIENT_NAME // SELLER_NAME
                                         << " for $"
                                         << matchPrice // PRICE
                                         << "/share" << endl;
                                }
                                countSharesTraded += QUANTITY;
                                countCompletedTrade++;
                                moneyTransferred += matchPrice * QUANTITY;
                                singleSideComission = matchPrice * QUANTITY / 100;
                                aggregateCommission += singleSideComission * 2;
                                QUANTITY = 0;
                                ptrbuyOrderBook->erase(ptrbuyOrderBook->begin());
                            } else {
                                // scenario 3. demand < supply
                                // OUTPUT something
                                if (isTransfers) {
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == CLIENT_NAME) {
                                            // seller found, update value
                                            sellerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockSold += ptrOrder->amount;
                                            ptrClientProfile->netValueTraded += ptrOrder->amount * matchPrice;

                                        }
                                        if (sellerFound) {
                                            break;
                                        }
                                    }
                                    if (!sellerFound) {
                                        // create profile for seller
                                        tempClientProfile.clientName = CLIENT_NAME;
                                        tempClientProfile.countStockBought = 0;
                                        tempClientProfile.countStockSold = ptrOrder->amount;
                                        tempClientProfile.netValueTraded = ptrOrder->amount * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
                                        if (itrClientList->clientName == ptrOrder->clientName) {
                                            // buyer found, update value
                                            buyerFound = true;
                                            ptrClientProfile = const_cast<clientProfile *>(&(*itrClientList));
                                            ptrClientProfile->countStockBought += ptrOrder->amount;
                                            ptrClientProfile->netValueTraded -= ptrOrder->amount * matchPrice;
                                        }
                                        if (buyerFound) {
                                            break;
                                        }
                                    } // end of for loop
                                    if (!buyerFound) {
                                        // create profile for buyer
                                        tempClientProfile.clientName = ptrOrder->clientName;
                                        tempClientProfile.countStockBought = ptrOrder->amount;
                                        tempClientProfile.countStockSold = 0;
                                        tempClientProfile.netValueTraded = -1 * ptrOrder->amount * matchPrice;
                                        clientList.insert(tempClientProfile);
                                    }
                                    buyerFound = false;
                                    sellerFound = false;
                                }
                                if (isVerbose) {
                                    cout << ptrOrder->clientName // BUYER_NAME
                                         << " purchased "
                                         << ptrOrder->amount // NUMBER_OF_SHARES
                                         << " shares of "
                                         << EQUITY_SYMBOL // EQUITY_SYMBOL
                                         << " from "
                                         << CLIENT_NAME // SELLER_NAME
                                         << " for $"
                                         << matchPrice // PRICE
                                         << "/share" << endl;
                                }
                                countSharesTraded += ptrOrder->amount;
                                countCompletedTrade++;
                                moneyTransferred += matchPrice * ptrOrder->amount;
                                singleSideComission = matchPrice * ptrOrder->amount / 100;
                                aggregateCommission += singleSideComission * 2;
                                QUANTITY = QUANTITY - ptrOrder->amount;
                                ptrbuyOrderBook->erase(ptrbuyOrderBook->begin());
                            }
                        } else {
                            // highest buying price is still too low
                            break;
                        }
                    } // end of while loop
                }
                break; // and stop searching for equity symbols
            }
            // else: this is not the equity we need; keep searching
        } // end of while (cin)

        // DEBUG output
        // cout << "I am starting to insert unused orders" << endl;

        // execution complete or equity not found
        if ((QUANTITY > 0) && (DURATION != 0)) {// still something left AND not IOC
            // first build the order
            tempOrder.amount = QUANTITY;
            tempOrder.clientName = CLIENT_NAME;
            if (DURATION == -1) {
                tempOrder.expireAt = -1;
            } else {
                tempOrder.expireAt = currentTimeStamp + DURATION;
            }
            tempOrder.price = PRICE;
            tempOrder.uniqueID = nextUniqueID;
            nextUniqueID++;

            // then figure out where to store the order
            if (itrOrderBook == orderBook.end()) {
                // equity symbol not found
                if (isBuy) {
                    // insert into buyOrderBook
                    tempEquityBook.buyOrderBook.clear();
                    tempEquityBook.buyOrderBook.insert(tempOrder);
                    tempEquityBook.sellOrderBook.clear();
                    tempEquityBook.equitySymbol = EQUITY_SYMBOL;
                    orderBook.insert(tempEquityBook);
                } else {
                    // insert into sellOrderBook
                    tempEquityBook.sellOrderBook.clear();
                    tempEquityBook.sellOrderBook.insert(tempOrder);
                    tempEquityBook.buyOrderBook.clear();
                    tempEquityBook.equitySymbol = EQUITY_SYMBOL;
                    orderBook.insert(tempEquityBook);
                }
            } else { // equity symbol found
                if (isBuy) {
                    // insert into buyOrderBook
                    ptrbuyOrderBook = const_cast<multiset<Order, compare_buyOrder>* >(&(itrOrderBook->buyOrderBook));
                    ptrbuyOrderBook->insert(tempOrder);
                } else {
                    // insert into sellOrderBook
                    ptrSellOrderBook = const_cast<multiset<Order, compare_sellOrder>* >(&(itrOrderBook->sellOrderBook));
                    ptrSellOrderBook->insert(tempOrder);
                }
            }
        }


    }

    // 5. output another median and midpoint if necessary
    // median first
    if (isMedian) {
        for (itrOrderBook = orderBook.begin(); itrOrderBook != orderBook.end(); itrOrderBook++) {
            sizeInInt = (int)itrOrderBook->matchedPriceHistory.size();
            if (sizeInInt == 0) {
                continue;
            }
            if (sizeInInt % 2 == 0) {
                isEven = true;
            } else {
                isEven = false;
            }
            sizeInInt = sizeInInt / 2;
            itrMedianMatchedPrice = itrOrderBook->matchedPriceHistory.begin();
            std::advance(itrMedianMatchedPrice, sizeInInt);
            if (isEven) {
                medianPrice = *itrMedianMatchedPrice;       // the middle
                medianPrice += *(--itrMedianMatchedPrice);  // + the one before that
                medianPrice = medianPrice / 2;
            } else { // size is odd
                medianPrice = *itrMedianMatchedPrice;
            }
            cout << "Median match price of "
                 << itrOrderBook->equitySymbol
                 << " at time "
                 << currentTimeStamp
                 << " is $"
                 << medianPrice << endl;
        }
    }

    // the midpoint
    if (isMidPoint) {
        for (itrOrderBook = orderBook.begin(); itrOrderBook != orderBook.end(); itrOrderBook++) {
            if (itrOrderBook->sellOrderBook.empty() || itrOrderBook->buyOrderBook.empty()) {
                cout << "Midpoint of "
                     << itrOrderBook->equitySymbol
                     << " at time "
                     << currentTimeStamp
                     << " is undefined"
                     << endl;
                continue;
            }
            highestBuy = (* itrOrderBook->buyOrderBook.begin()).price;
            lowestSell = (* itrOrderBook->sellOrderBook.begin()).price;
            midPoint = (highestBuy + lowestSell) / 2;
            cout << "Midpoint of "
                 << itrOrderBook->equitySymbol
                 << " at time "
                 << currentTimeStamp
                 << " is $"
                 << midPoint
                 << endl;
        }
    }

    // DEBUG output
    // cout << "I am printing end-of-day output" << endl;

    // 6. print end-of-day output
    cout << "---End of Day---" << endl
         << "Commission Earnings: $" << aggregateCommission << endl
         << "Total Amount of Money Transferred: $" << moneyTransferred << endl
         << "Number of Completed Trades: " << countCompletedTrade << endl
         << "Number of Shares Traded: " << countSharesTraded << endl;

    if (isTransfers) {
        for (itrClientList = clientList.begin(); itrClientList != clientList.end(); itrClientList++) {
            cout << itrClientList->clientName
                 << " bought "
                 << itrClientList->countStockBought
                 << " and sold "
                 << itrClientList->countStockSold
                 << " for a net transfer of $"
                 << itrClientList->netValueTraded
                 << endl;
        }
    }

    for (itrtttEquityBST = tttEquityBST.begin(); itrtttEquityBST != tttEquityBST.end(); itrtttEquityBST++) {
        cout << "Time travelers would buy "
             << itrtttEquityBST->symbol
             << " at time: "
             << itrtttEquityBST->buyTimeStamp
             << " and sell it at time: "
             << itrtttEquityBST->sellTimeStamp
             << endl;
    }


    // timer code
    // comment out for submission
//        timer = clock() - timer;
//        cout << "Program running time is " << ((double)timer / CLOCKS_PER_SEC) << " sec" << endl;
    return 0;
}