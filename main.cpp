#include <iostream>
#include <string>

#include "utils.h"

#include "LEDA/core/queue.h"
#include "LEDA/graph/basic_graph_alg.h"

#define NUM_OF_NODES 10
#define NUM_OF_EDGES 5
#define GREEN "green"
#define BLUE "blue"

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
            return false;
        }
    }
}

int main(int argc, char *argv[]) {
    leda::node v;

    // Initialize a random graph G
    leda::graph G;
    // void random_graph(graph& G, int n, int m, bool no_anti_parallel_edges, bool loopfree, bool no_parallel_edges)
    leda::random_graph(G, NUM_OF_NODES, NUM_OF_EDGES, true, true, true);
    G.is_directed() ? std::cout << "G is directed" << std::endl : std::cout << "G is undirected" << std::endl;
    G.make_undirected(); std::cout << "Making G undirected, using LEDA" << std::endl;
    G.is_directed() ? std::cout << "G is directed" << std::endl : std::cout << "G is undirected" << std::endl;
    printGraph(G);
    addComplementaryEdges(G); std::cout << "Adding complementart edges to an undirected graph: " << std::endl; 
    G.is_directed() ? std::cout << "G is directed" << std::endl : std::cout << "G is undirected" << std::endl;
    printGraph(G);
}