#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

struct node {
    int node_id = 0;
    node *pre = NULL;
    node *next = NULL;
    bool visited = false;
    int edge_to_next = 0;
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

struct graph {
    map<int, node *> nodes;
    vector<edge *> edges;
//    priority_queue<edge *, vector<edge *>, edge_cpr> edges;
};

void set_node_unvisited(graph &graphA) {
    for (auto it = graphA.nodes.begin(); it != graphA.nodes.end(); it++) {
        ((*it).second)->visited = false;
    }
}

bool is_adjacent(graph &graphA, node *nodeA, node *nodeB) {
    auto it = graphA.nodes.find(nodeA->node_id);
    return (it->second->next == nodeB || it->second->pre == nodeB);
}

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

int main() {
    int node_num, source_node, destination;
    cin >> node_num;
    cin >> source_node;
    cin >> destination;
    graph Big_graph;
//    priority_queue<edge *, vector<edge *>, edge_cpr>::iterator it_vec;
    vector<edge *>::iterator it_vec;


    int startn, endn, wei;
    while (cin) {
        auto temp_node_start = new node;
        auto temp_node_end = new node;
        auto temp_edge = new edge;
        cin >> startn >> endn >> wei;

        if (Big_graph.nodes.find(startn) == Big_graph.nodes.end() &&
            Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            temp_node_start->node_id = startn;
            temp_node_end->node_id = endn;
            temp_node_start->next = temp_node_end;
            temp_node_end->pre = temp_node_start;
            temp_node_start->edge_to_next = wei;
            Big_graph.nodes.insert(make_pair(startn, temp_node_start));
            Big_graph.nodes.insert(make_pair(endn, temp_node_end));
        }

        else if (Big_graph.nodes.find(startn) != Big_graph.nodes.end() &&
                 Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            temp_node_start = (*Big_graph.nodes.find(startn)).second;
            temp_node_end->node_id = endn;
            temp_node_end->pre = temp_node_start;
            temp_node_start->next = temp_node_end;
            temp_node_start->edge_to_next = wei;
            Big_graph.nodes.insert(make_pair(endn, temp_node_end));
        }

        else if (Big_graph.nodes.find(startn) != Big_graph.nodes.end() &&
                 Big_graph.nodes.find(endn) == Big_graph.nodes.end()) {
            temp_node_end = (*Big_graph.nodes.find(endn)).second;
            temp_node_start->node_id = startn;
            temp_node_start->next = temp_node_end;
            temp_node_end->pre = temp_node_start;
            temp_node_start->edge_to_next = wei;
            Big_graph.nodes.insert(make_pair(startn, temp_node_start));
        }

        else {
            temp_node_start = (*Big_graph.nodes.find(startn)).second;
            temp_node_end = (*Big_graph.nodes.find(endn)).second;
            temp_node_end->pre = temp_node_start;
            temp_node_start->next = temp_node_end;
            temp_node_start->edge_to_next = wei;
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
    int Pathlen = 0;
    node *nodetemp = (*(Big_graph.nodes.find(source_node))).second;
    while (true) {
        if (nodetemp->node_id == destination) {
            cout << "Shortest path length is " << Pathlen << endl;
            break;
        }
        else if (nodetemp->visited) {
            cout << "No path exists!" << endl;
            break;
        }
        nodetemp->visited = true;
        Pathlen += nodetemp->edge_to_next;
        nodetemp = nodetemp->next;
    }

    set_node_unvisited(Big_graph);

    // DAG or NOT
    bool DAG = true;
    for (auto it = Big_graph.nodes.begin(); it != Big_graph.nodes.end(); it++) {
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
    if (DAG) cout << "The graph is a DAG" << endl;
    else cout << "The graph is not a DAG" << endl;

    //Total weight of MST
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

//        for (it_vec = Big_graph.edges.begin(); it_vec != Big_graph.edges.end(); ) {
//            if ((*it_vec) == tobeerase) Big_graph.edges.erase(it_vec);
//            else it_vec++;
//        }

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

    // Use Krusual's Method istead
//    int Weight_All = 0;
//    while (! Big_graph.edges.empty()){
//
//        auto temp3 = Big_graph.edges.top();
//
//
//
//        temp3->end_node->visited = true;
//        temp3->start_node->visited = true;
//        Weight_All += temp3->weight;
//
//    }
}


