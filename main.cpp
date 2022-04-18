#include <iostream>
#include <string>

#include "utils.h"

#include "LEDA/core/queue.h"

leda::list<leda::node> my_BFS(const leda::graph& G, leda::node s, leda::node_array<int>& dist, leda::node_array<leda::edge>& pred, leda::node_array<std::string>& color) {
    leda::list<leda::node> visitedNodes;
    leda::node v, u;
    leda::edge e;

    // Create an empty queue Q
    leda::queue<leda::node> Q;
    // Insert s onto Q and mark s as visited
    dist[s] = 0;
    color[s] = GREEN;
    pred[s] = nil;
    visitedNodes.push_back(s);
    Q.append(s);

    while(!Q.empty()) {
        // Pop a node
        v = Q.pop();
        // forall_edges that have v as source, get the target node u
        forall_adj_edges(e, v) {
            u = G.opposite(v, e);
            // if u is not marked, mark it and insert it onto Q
            if(dist[u] == -1) {
                visitedNodes.push_back(u);
                dist[u] = dist[v] + 1;
                pred[u] = e;
                color[u] = color[v] == GREEN ? BLUE : GREEN;
                Q.append(u);
            }
        }
    }
    return visitedNodes;
}


bool my_bipar_checker(const leda::graph& G, leda::list<leda::node>& V1, leda::list<leda::node>& V2) {
    // Helping nodes and edges
    leda::edge e;
    leda::node v, u;

    // Initialize node_arrays and the starting node and execute my_BFS()
    leda::node s = G.first_node();
    leda::node_array<int> dist(G, -1);
    leda::node_array<leda::edge> pred(G);
    leda::node_array<std::string> color(G);
    my_BFS(G, s, dist, pred, color);

    forall_edges(e, G) {
        v = G.source(e);
        u = G.target(e);
        if (color[v] == color[u]) {
            // Use BFS' pred node_array to find the common ancestor
            do {
                if(dist[v] > dist[u]) {
                    V1.append(v);
                    v = G.opposite(v, pred[v]);
                }
                else {
                    V1.append(u);
                    u = G.opposite(u, pred[u]);
                }
            }  while (v != u);
            V1.append(v);
            return false;
        }
    }

    forall_nodes(v, G) {
        color[v] == GREEN ? V1.append(v) : V2.append(v);
    }
    return true;
}

int main(int argc, char *argv[]) {
    leda::node v;
    leda::list<leda::node> V1;
    leda::list<leda::node> V2;
    // Initialize a random graph G
    leda::graph G;
    // random_connected_graph(G);
    test_graph(G);

    // Make graph undirected
    addComplementaryEdges(G);

    printGraph(G);
    
    leda::Is_Bipartite(G, V1, V2);
    // Print the odd circle LEDA found
    std::cout << "LEDA V1: ";
    for (auto it = V1.begin(); it != V1.end(); it++) {
        std::cout << (*it)->id() << ", ";
    }
    std::cout << std::endl;

    std::cout << "LEDA V2: ";
    for (auto it = V2.begin(); it != V2.end(); it++) {
        std::cout << (*it)->id() << ", ";
    }
    std::cout << std::endl;

    V1.clear();
    V2.clear();

    // Run my_bipar_checker()
    my_bipar_checker(G, V1, V2);
    // Print the odd circle I found
    std::cout << "My V1: ";
    for (auto it = V1.begin(); it != V1.end(); it++) {
        std::cout << (*it)->id() << ", ";
    }
    std::cout << std::endl;

    std::cout << "My V2: ";
    for (auto it = V2.begin(); it != V2.end(); it++) {
        std::cout << (*it)->id() << ", ";
    }
    std::cout << std::endl;
}