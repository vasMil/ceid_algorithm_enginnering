#include "LEDA/graph/graph.h"
#include "LEDA/core/queue.h"
#include "LEDA/graph/basic_graph_alg.h"
#include <iostream>
#include <string>
#include <list>

#define NUM_OF_NODES 10
#define NUM_OF_EDGES 14
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
    Q.push(s);

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
    leda::node v, u;
    leda::list<leda::node> myVisitedNodes, visitedNodes;
    leda::node_array<std::string> color(G);
    leda::node_array<leda::edge> pred(G, nil), myPred(G, nil);

    forall_nodes(v, G) {
        leda::node_array<int> dist(G, -1), myDist(G, -1);

        visitedNodes = BFS(G, v, dist, pred);
        myVisitedNodes = my_BFS(G, v, myDist, myPred, color);
        if (visitedNodes.size() != myVisitedNodes.size()) {
            std::cout << "my_BFS is incorrect, myVisitedNodes.size() = " << myVisitedNodes.size() << ", visitedNodes.size() = " << visitedNodes.size() << std::endl;
            return;
        }
        myVisitedNodes.sort();
        visitedNodes.sort();

        leda::list<leda::node>::iterator myIt = myVisitedNodes.begin();
        for (leda::list<leda::node>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); it ++) {
            if ((*it) != (*myIt)) std::cout << "my_BFS has a mistake in visitedNodes" << std::endl;
            myIt++;
        }
        forall_nodes(u, G) {
            if(pred[v] != myPred[v]) std::cout << "my_BFS has a mistake in pred" << std::endl;
            if(dist[v] != myDist[v]) std::cout << "my_BFS has a mistake in dist" << std::endl;
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
    leda::node_array<leda::edge> pred(G, nil);
    leda::node_array<std::string> color(G);

    // Run my_BFS
    leda::list<leda::node> visitedNodes = my_BFS(G, start, dist, pred, color);

    // std::cout << "Starting node has id: " << start->id() << std::endl;
    // leda::list<leda::node> visitedNodes = BFS(G, start, dist, pred);
    // forall_nodes(v, G) {
    //     std::cout << v->id() << " pred = " << pred[v] << std::endl;
    // }
    // std::cout << "LEDA Nodes visited " << visitedNodes.size() << ": ";
    // for(leda::list<leda::node>::iterator it = visitedNodes.begin(); it != visitedNodes.end(); it++) {
    //     std::cout << (*it)->id() << ", ";
    // }
    testBFS(G);
    // forall_nodes(v, G) {
    //     std::cout << "id: " << v->id() << ", level: " << dist[v] << ", color: " << color[v] << std::endl;
    // }
}