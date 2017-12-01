#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
//#include <list>

using namespace std;

struct node {
    int node_id = 0;
//    list<node *> pre;
//    list<node *> next;
    vector<node *> pre;
    vector<node *> next;
    int temp_D = -1;
    bool visited = false;
//    explicit node (int node_id): node_id(node_id);
};


struct edge {
    int weight = 0;
    node *start_node = nullptr;
    node *end_node = nullptr;
};

struct edge_cpr {
    bool operator()(const edge &a, const edge &b) {
        /*if (a.weight == b.weight) return a.start_node->node_id > b.start_node->node_id;
        else */return a.weight > b.weight;
    }

    bool operator()(edge &a, edge &b) {
        /*if (a.weight == b.weight) return a.start_node->node_id > b.start_node->node_id;
        else*/ return a.weight > b.weight;
    }
};

struct node_cpr {
    bool operator()(const node *a, const node *b) {
        /*if (a->temp_D == b->temp_D) return a->node_id > b->node_id;
        else*/ return a->temp_D > b->temp_D;
    }

    bool operator()(node *a, node *b) {
        /*if (a->temp_D == b->temp_D) return a->node_id > b->node_id;
        else */return a->temp_D > b->temp_D;
    }
};


struct graph {
    map<int, node *> nodes;
    vector<edge *> edges;
};

inline void set_node_unvisited(graph &graphA) {
    for (auto &node : graphA.nodes) {
        (node.second)->visited = false;
    }
}

inline bool DAG(node *temp, vector<node *> node_in) {
    for (auto &it2 : node_in) {
        if (it2 == temp) return false;
    }
    temp->visited = true;
    node_in.push_back(temp);
    if (temp->next.empty()) return true;
    else {
        auto it = temp->next.begin();
        while (it != temp->next.end()) {
            if (!DAG(*it, node_in)) return false;
            else it++;
        }
        return true;
    }
}

int main() {
    int node_num, source_node, destination;
    cin >> node_num;
    cin >> source_node;
    cin >> destination;
    graph Big_graph = graph();
    vector<edge *>::iterator it_vec;

    stringstream ss;
    int startn, endn, wei;
    while (!cin.eof()) {

        auto temp_edge = new edge;
        string str1;
        getline(cin, str1);
        if (str1.empty()) continue;
        ss.clear();
        ss.str(str1);
        ss >> startn >> endn >> wei;

        if (Big_graph.nodes.find(startn) == Big_graph.nodes.end() &&
            Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            auto temp_node_start = new node;
            auto temp_node_end = new node;
            temp_node_start->node_id = startn;
            temp_node_end->node_id = endn;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            Big_graph.nodes.insert(make_pair(startn, temp_node_start));
            Big_graph.nodes.insert(make_pair(endn, temp_node_end));
            temp_edge->end_node = temp_node_end;
            temp_edge->start_node = temp_node_start;
        }
        else if (Big_graph.nodes.find(startn) != Big_graph.nodes.end() &&
                 Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            auto temp_node_end = new node;
            auto temp_node_start = (*Big_graph.nodes.find(startn)).second;
            temp_node_end->node_id = endn;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            Big_graph.nodes.insert(make_pair(endn, temp_node_end));
            temp_edge->end_node = temp_node_end;
            temp_edge->start_node = temp_node_start;
        }
        else if (Big_graph.nodes.find(startn) != Big_graph.nodes.end() &&
                 Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            auto temp_node_start = new node;
            auto temp_node_end = (*Big_graph.nodes.find(endn)).second;
            temp_node_start->node_id = startn;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            Big_graph.nodes.insert(make_pair(startn, temp_node_start));
            temp_edge->end_node = temp_node_end;
            temp_edge->start_node = temp_node_start;
        }
        else {
            auto temp_node_start = (*Big_graph.nodes.find(startn)).second;
            auto temp_node_end = (*Big_graph.nodes.find(endn)).second;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            temp_edge->end_node = temp_node_end;
            temp_edge->start_node = temp_node_start;
        }
        temp_edge->weight = wei;
        Big_graph.edges.push_back(temp_edge);
    }


    // Shortest Path
    /*
    set_node_unvisited(Big_graph);
    priority_queue<node *, vector<node *>, node_cpr> Node_PQ;
    node *node_temp = (*(Big_graph.nodes.find(source_node))).second;
    node_temp->temp_D = 0;
    while (true) {
        node_temp->visited = true;
        if (node_temp->node_id == destination) {
            cout << "Shortest path length is " << node_temp->temp_D << endl;
            break;
        }
        // Add all the nodes adjacent
        for (auto it = node_temp->next.begin(); it != node_temp->next.end(); ++it) {
            // Set edge unvisited and using while loop
            for (auto &edge : Big_graph.edges) {
                if (edge->start_node == node_temp) {
                    if (edge->end_node->temp_D == -1) {
                        edge->end_node->temp_D = node_temp->temp_D + edge->weight;
                        Node_PQ.push(edge->end_node);
                    }
                    else {
                        if (edge->end_node->temp_D > node_temp->temp_D + edge->weight) {
                            edge->end_node->temp_D = node_temp->temp_D + edge->weight;
                        }
                    }
                }
            }
//            for (auto it2 = Big_graph.edges.begin(); it2 != Big_graph.edges.end(); it2++) {
//                if ((*it2)->start_node == node_temp) {
//                    if ((*it2)->end_node->temp_D == -1) {
//                        (*it2)->end_node->temp_D = node_temp->temp_D + (*it2)->weight;
//                        Node_PQ.push((*it2)->end_node);
//                    }
//                    else {
//                        if ((*it2)->end_node->temp_D > node_temp->temp_D + (*it2)->weight) {
//                            (*it2)->end_node->temp_D = node_temp->temp_D + (*it2)->weight;
//                        }
//                    }
//                }
//            }
        }

        if (Node_PQ.empty()) {
            cout << "No path exists!" << endl;
            break;
        }
        else {
            node_temp = Node_PQ.top();
            Node_PQ.pop();
        }
    }
    */

    set_node_unvisited(Big_graph);
    priority_queue<node *, vector<node *>, node_cpr> Node_PQ;
    node &node_temp = *(*(Big_graph.nodes.find(source_node))).second;
    node_temp.temp_D = 0;
    while (true) {
        node_temp.visited = true;
        if (node_temp.node_id == destination) {
            cout << "Shortest path length is " << node_temp.temp_D << endl;
            break;
        }
        // Add all the nodes adjacent
        for (auto it = node_temp.next.begin(); it != node_temp.next.end(); ++it) {
            // Set edge unvisited and using while loop
            for (auto &edge : Big_graph.edges) {
                if (edge->start_node->node_id == node_temp.node_id) {
                    if (edge->end_node->temp_D == -1) {
                        edge->end_node->temp_D = node_temp.temp_D + edge->weight;
                        Node_PQ.push(edge->end_node);
                    }
                    else {
                        if (edge->end_node->temp_D > node_temp.temp_D + edge->weight) {
                            edge->end_node->temp_D = node_temp.temp_D + edge->weight;
                        }
                    }
                }
            }
        }

        if (Node_PQ.empty()) {
            cout << "No path exists!" << endl;
            break;
        }
        else {
            node_temp = *Node_PQ.top();
            Node_PQ.pop();
        }
    }

    // DAG or NOT
    vector<node *> node_in;
    set_node_unvisited(Big_graph);
    node *temp = (*Big_graph.nodes.begin()).second;
    if (DAG(temp, node_in)) {
        cout << "The graph is a DAG" << endl;
    }
    else {
        cout << "The graph is not a DAG" << endl;
    }

    // Minimum Spanning tree

    int weight_all = 0;
    priority_queue<edge, vector<edge>, edge_cpr> PQ;
    set_node_unvisited(Big_graph);
    auto it = Big_graph.nodes.begin();
    auto beginner = it;
    auto temp3 = it->second;
    while (true) {
        temp3->visited = true;
        // Put all the adjacent edges in the PQ
        for (it_vec = Big_graph.edges.begin(); it_vec != Big_graph.edges.end(); ++it_vec) {
            if (((*it_vec)->start_node == temp3 || (*it_vec)->end_node == temp3)
                && !((*it_vec)->start_node->visited && (*it_vec)->end_node->visited)) {
                PQ.push(*(*it_vec));
            }
        }
        // Get the minimum

        if (PQ.empty()) break;
        else if (PQ.top().end_node->visited && PQ.top().start_node->visited) {
            PQ.pop();
            continue;
        }
        else if (PQ.top().end_node->visited) {
            temp3 = PQ.top().start_node;
        }
        else {
            temp3 = PQ.top().end_node;
        }
        PQ.top().end_node->visited = true;
        PQ.top().start_node->visited = true;
        weight_all += PQ.top().weight;
//        FinalEdges.push(PQ.top());
        PQ.pop();
    }

    it = Big_graph.nodes.begin();
    bool have_answer = true;
    while (it != Big_graph.nodes.end()) {
        if (!it->second->visited) {
            have_answer = false;
            break;
        }
        it++;
    }
    if (have_answer) {
        cout << "The total weight of MST is " << weight_all << endl;
    }
    else {
        cout << "No MST exists!" << endl;
    }

    return 0;
}