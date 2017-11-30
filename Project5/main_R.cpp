#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <list>

using namespace std;

struct node {
    int node_id = 0;
    list<node *> pre;
    list<node *> next;
    int temp_D = -1;
    bool visited = false;
};


struct edge {
    int weight = 0;
    node *start_node = nullptr;
    node *end_node = nullptr;
};

struct edge_cpr {
    bool operator()(const edge &a, const edge &b) {
        return a.weight > b.weight;
    }

    bool operator()(edge &a, edge &b) {
        return a.weight > b.weight;
    }
};

struct node_cpr {
    bool operator()(const node *a, const node *b) {
        return a->temp_D > b->temp_D;
    }

    bool operator()(node *a, node *b) {
        return a->temp_D > b->temp_D;
    }
};

struct graph {
    map<int, node *> nodes;
    vector<edge *> edges;
};

int main(){
    int node_num, source_node, destination;
    cin >> node_num;
    cin >> source_node;
    cin >> destination;
    graph Big_graph;
    vector<edge *>::iterator it_vec;

    stringstream ss;
    int startn, endn, wei;
    while (!cin.eof()) {
        auto temp_node_start = new node;
        auto temp_node_end = new node;
        auto temp_edge = new edge;
        string str1;
        getline(cin, str1);
        if (str1.empty()) continue;
        ss.clear();
        ss.str(str1);
        ss >> startn >> endn >> wei;

        if (Big_graph.nodes.find(startn) == Big_graph.nodes.end() &&
            Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            temp_node_start->node_id = startn;
            temp_node_end->node_id = endn;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            Big_graph.nodes.insert(make_pair(startn, temp_node_start));
            Big_graph.nodes.insert(make_pair(endn, temp_node_end));
        }

        else if (Big_graph.nodes.find(startn) != Big_graph.nodes.end() &&
                 Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            temp_node_start = (*Big_graph.nodes.find(startn)).second;
            temp_node_end->node_id = endn;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            Big_graph.nodes.insert(make_pair(endn, temp_node_end));
        }

        else if (Big_graph.nodes.find(startn) != Big_graph.nodes.end() &&
                 Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            temp_node_end = (*Big_graph.nodes.find(endn)).second;
            temp_node_start->node_id = startn;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
            Big_graph.nodes.insert(make_pair(startn, temp_node_start));
        }

        else {
            temp_node_start = (*Big_graph.nodes.find(startn)).second;
            temp_node_end = (*Big_graph.nodes.find(endn)).second;
            temp_node_start->next.push_back(temp_node_end);
            temp_node_end->pre.push_back(temp_node_start);
        }

        temp_edge->weight = wei;
        temp_edge->end_node = temp_node_end;
        temp_edge->start_node = temp_node_start;
        Big_graph.edges.push_back(temp_edge);
    }



}