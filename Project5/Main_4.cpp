#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
//#include <list>

using namespace std;

struct node {
    int node_id = 0;
    vector<node *> pre;
    vector<node *> next;
    int temp_D = -1;
    bool visited = false;
    vector<pair<int, node *>> neighbour_edges;
    int degree = 0;

//    explicit node(int nodeid) : node_id(nodeid) {};
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

        for (int i = 0; i < node_num; ++i) {
            auto temp = new node;
            temp->node_id = i;
            Big_graph.nodes.insert(make_pair(i, temp));
        }
        auto temp_node_start = (*Big_graph.nodes.find(startn)).second;
        auto temp_node_end = (*Big_graph.nodes.find(endn)).second;
        temp_node_start->next.push_back(temp_node_end);
        temp_node_end->pre.push_back(temp_node_start);
        temp_edge->end_node = temp_node_end;
        temp_edge->start_node = temp_node_start;
        temp_node_start->neighbour_edges.emplace_back(make_pair(wei, temp_node_end));
        ++temp_node_end->degree;
        temp_edge->weight = wei;
        Big_graph.edges.push_back(temp_edge);
    }

    auto graph_temp = Big_graph;
    // Shortest Path

    set_node_unvisited(Big_graph);
    priority_queue<node *, vector<node *>, node_cpr> Node_PQ;
    auto node_temp = Big_graph.nodes.find(source_node)->second;
    node_temp->temp_D = 0;
    while (true) {
        node_temp->visited = true;
        if (node_temp->node_id == destination) {
            cout << "Shortest path length is " << node_temp->temp_D << endl;
            break;
        }

        for (auto it = node_temp->neighbour_edges.begin(); it != node_temp->neighbour_edges.end(); it++) {
            if (it->second->temp_D == -1) {
                it->second->temp_D = node_temp->temp_D + it->first;
                Node_PQ.push(it->second);
            }
            else if (it->second->temp_D > node_temp->temp_D + it->first) {
                it->second->temp_D = node_temp->temp_D + it->first;
            }
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

    Big_graph = graph_temp;
    // DAG or NOT
    vector<node *> node_in;
//    set_node_unvisited(Big_graph);
//    node *temp = (*Big_graph.nodes.begin()).second;
//    if (DAG(temp, node_in)) {
//        cout << "The graph is a DAG" << endl;
//    }
//    else {
//        cout << "The graph is not a DAG" << endl;
//    }
    vector<node *> node_no_income;
    for (auto &node : Big_graph.nodes) {
//        cout << node.first << " " << node.second->degree <<  endl;
        if (node.second->degree == 0) {
            node_no_income.push_back(node.second);
        }
    }
    int countALL = 0;
    while (!node_no_income.empty()) {
        auto temp = node_no_income.back();
        node_no_income.pop_back();
        ++countALL;
        for (auto &edge : temp->neighbour_edges) {
            if (edge.second->degree > 0) --edge.second->degree;
            if (edge.second->degree == 0) node_no_income.push_back(edge.second);
        }
    }
    if (countALL == node_num) cout << "The graph is a DAG" << endl;
    else cout << "The graph is not a DAG" << endl;

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