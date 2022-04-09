#include "LEDA/graph/graph.h"
#include "LEDA/core/queue.h"
#include "LEDA/graph/basic_graph_alg.h"
#include <iostream>
#include <string>
#include <list>

#define NUM_OF_NODES 10
#define NUM_OF_EDGES 14

leda::list<leda::node> my_BFS(const leda::graph& G, leda::node s, leda::node_array<int>& dist, leda::node_array<leda::edge>& pred, leda::node_array<std::string>& color) {
    leda::list<leda::node> visitedNodes;
    leda::node v, u;
    leda::edge e;
    // Initialize visited node_array which will be used to keep track of the nodes that have been visited
    leda::node_array<bool> visited(G);

    // Create an empty queue Q
    leda::queue<leda::node> Q;
    // Insert s onto Q and mark s as visited
    visited[s] = 1;
    visitedNodes.push_back(s);
    Q.push(s);

    while(!Q.empty()) {
        // Pop a node
        v = Q.pop();
        // forall_edges that have v as source, get the target node u
        forall_adj_edges(e, v) {
            u = G.target(e);
            // if u is not marked, mark it and insert it onto Q
            if(!visited[u]) {
                visited[u] = 1;
                visitedNodes.push_back(u);
                Q.push(u);
            }
        }
    }

    return visitedNodes;
}

void printGraph(const leda::graph& G) {
    leda::edge e;
    leda::node s, t;
    forall_edges(e, G) {
        s = G.source(e);
        t = G.target(e);
        std::cout << s->id() << " -> " << t->id() << std::endl;
    }
}

void testBFS(const leda::graph& G) {
    leda::node v;
    leda::list<leda::node> myVisitedNodes, visitedNodes;
    leda::node_array<int> dist(G, -1);
    leda::node_array<leda::edge> pred(G, nil);
    leda::node_array<std::string> color(G);
    forall_nodes(v, G) {
        leda::node_array<int> dist(G, -1);
        visitedNodes = BFS(G, v, dist, pred);
        myVisitedNodes = my_BFS(G, v, dist, pred, color);
        if (visitedNodes.size() != myVisitedNodes.size()) std::cout << "my_BFS is incorrect" << std::endl;
        myVisitedNodes.sort();
        visitedNodes.sort();
        leda::list<leda::node>::iterator myIt = myVisitedNodes.begin();
        for (leda::list<leda::node>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); it ++) {
            if ((*it) != (*myIt)) std::cout << "my_BFS is incorrect 2" << std::endl;
            myIt++;
        }
    }
}

int main(int argc, char *argv[]) {
    // Initialize a random graph G
    leda::graph G;
    leda::random_graph(G, NUM_OF_NODES, NUM_OF_EDGES);
    leda::node v;

    // Get its first node
    leda::node start = G.first_node();

    // Define node_arrays for G used in my_BFS
    leda::node_array<int> dist(G, -1);
    leda::node_array<leda::edge> pred(G);
    leda::node_array<std::string> color(G);

    // Run my_BFS
    leda::list<leda::node> visitedNodes = my_BFS(G, start, dist, pred, color);

    // Print out the results
    // printGraph(G);
    // std::cout << "Nodes visited: ";
    // for(leda::list<leda::node>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); it++) {
    //     std::cout << (*it)->id() << ", ";
    // }

    // std::cout << std::endl;

    // visitedNodes = BFS(G, start, dist, pred);
    // std::cout << "LEDA Nodes visited " << visitedNodes.size() << ": ";
    // for(leda::list<leda::node>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); it++) {
    //     std::cout << (*it)->id() << ", ";
    // }
    testBFS(G);
}