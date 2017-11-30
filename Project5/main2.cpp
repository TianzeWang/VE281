#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <list>

using namespace std;

struct node {
    int node_id = 0;
//    node *pre = NULL;
//    node *next = NULL;
    list<node *> pre;
    list<node *> next;
    node *path_visit_pre;
    int temp_D = -1;
    bool visited = false;
    vector<node *> adjacent_nodes;
};


struct edge {
    int weight = 0;
    node *start_node = NULL;
    node *end_node = NULL;
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
//    priority_queue<edge *, vector<edge *>, edge_cpr> edges;
};

void set_node_unvisited(graph &graphA) {
//    for (auto it = graphA.nodes.begin(); it != graphA.nodes.end(); it++) {
//        ((*it).second)->visited = false;
//    }
    for (auto &node : graphA.nodes) {
        (node.second)->visited = false;
    }
}

bool DAG(node *temp, vector<node *> node_in) {
    for (auto &it2 : node_in) {
        if (it2 == temp) return false;
    }

    temp->visited = true;
    node_in.push_back(temp);
    if (temp->next.empty()) return true;
    else {
        auto it = temp->next.begin();
        while (it != temp->next.end()){
            auto i2 = *it;
            bool A = DAG(*it, node_in);
            if (!A) return false;
            else it++;
        }
        return true;
/*        bool *A = new bool(temp->next.size());
        int i = 0;
        for (auto &it : temp->next) {
            A[i] = DAG(it, node_in);
            i++;
        }

        for (i=0; i<temp->next.size()){
            if (A[i]== false) return false;
        }

        return true; */
    }
}

/*
bool is_cyclic(graph &graph1) {
    set_node_unvisited(graph1);
    bool DAG = true;
    for (auto it = graph1.nodes.begin(); it != graph1.nodes.end(); it++) {
        if (it->second->visited) continue;
        else {
            node *temp2 = it->second;
            while (true) {
                temp2->visited = true;
                if (temp2->next == NULL) {
                    break;
                }
                else if (temp2->next->visited) {
                    DAG = false;
                    break;
                }
                else {
                    temp2 = temp2->next;
                }
            }
            if (!DAG) {
                break;
            }
        }
    }
    return (!DAG);
}
*/
int main() {
    int node_num, source_node, destination;
    cin >> node_num;
    cin >> source_node;
    cin >> destination;
    graph Big_graph;
//    priority_queue<edge *, vector<edge *>, edge_cpr>::iterator it_vec;
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

    /*
    debug output
    for (auto it = Big_graph.nodes.begin(); it !=Big_graph.nodes.end(); it++) {
        cout << "The starting node is " << (*it).second->node_id << " and its nest node id is " << (*it).second->next->node_id << endl;
    }
     */

    // Shortest Path
    set_node_unvisited(Big_graph);
    priority_queue<node *, vector<node *>, node_cpr> Node_PQ;
    int Pathlen = 0;
    node *nodetemp = (*(Big_graph.nodes.find(source_node))).second;
    nodetemp->temp_D = 0;
    while (true) {
        nodetemp->visited = true;
        if (nodetemp->node_id == destination) {
            cout << "Shortest path length is " << nodetemp->temp_D << endl;
            break;
        }
        // Add all the nodes adjacent
        for (auto it = nodetemp->next.begin(); it != nodetemp->next.end(); it++) {
            for (auto it2 = Big_graph.edges.begin(); it2 != Big_graph.edges.end(); it2++) {
                if ((*it2)->start_node == nodetemp) {
                    if ((*it2)->end_node->temp_D == -1) {
                        (*it2)->end_node->temp_D = nodetemp->temp_D + (*it2)->weight;
                        (*it2)->end_node->path_visit_pre = (*it2)->start_node;
                        Node_PQ.push((*it2)->end_node);
                    }
                    else {
                        if ((*it2)->end_node->temp_D > nodetemp->temp_D + (*it2)->weight) {
                            (*it2)->end_node->temp_D = nodetemp->temp_D + (*it2)->weight;
                            (*it2)->end_node->path_visit_pre = (*it2)->start_node;
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
            nodetemp = Node_PQ.top();
            Node_PQ.pop();
        }
    }

    // DAG or NOT
    vector<node *> node_in;
    set_node_unvisited(Big_graph);
    node *temp = (*Big_graph.nodes.begin()).second;
    bool A = DAG(temp, node_in);
    if (A) {
        cout << "The graph is a DAG" << endl;
    }
    else {
        cout << "The graph is not a DAG" << endl;
    }

    // Minimum Spanning tree

    int weight_all = 0;
    priority_queue<edge, vector<edge>, edge_cpr> PQ, FinalEdges;
    set_node_unvisited(Big_graph);
    auto it = Big_graph.nodes.begin();
    auto beginner = it;
    node *temp3 = it->second;
    while (true) {
        temp3->visited = true;
        // Put all the adjacent edges in the PQ
        for (it_vec = Big_graph.edges.begin(); it_vec != Big_graph.edges.end(); it_vec++) {
            if (((*it_vec)->start_node == temp3 || (*it_vec)->end_node == temp3)
                && !( (*it_vec)->start_node->visited && (*it_vec)->end_node->visited )) {
                PQ.push(*(*it_vec));
            }
        }
        // Get the minimum

        if (PQ.empty()) break;

        if (PQ.top().end_node->visited && PQ.top().start_node->visited) {
            PQ.pop();
            continue;
        }

        if (PQ.top().end_node->visited) {
            temp3 = PQ.top().start_node;
        }

        else {
            temp3 = PQ.top().end_node;
        }

        auto tobeerase = PQ.top();

        PQ.top().end_node->visited = true;
        PQ.top().start_node->visited = true;
        weight_all += PQ.top().weight;
        FinalEdges.push(PQ.top());
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