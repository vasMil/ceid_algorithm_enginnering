#pragma once
#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/graph/basic_graph_alg.h"

#include <iostream>
#include <string>
#include <set>

#define NUM_OF_NODES 10
#define NUM_OF_EDGES 12
#define GREEN "green"
#define BLUE "blue"

void printGraph(const leda::graph& G) {
    std::cout << "Printing Graph..." << std::endl;
    leda::edge e;
    leda::node s, t;
    forall_edges(e, G) {
        s = G.source(e);
        t = G.target(e);
        std::cout << s->id() << " -> " << t->id() << std::endl;
    }
}

// Converts a graph (either a directed or undirected one) to a directed one, with all initial edges and their anti-parallel edges of the input graph.
// If there are loops remove them from G.
// Parallel edges will also be ommited.
void addComplementaryEdges(leda::graph& G) {
    if (!G.is_directed()) G.make_directed();
    leda::edge e;
    leda::node v, u;
    // Will ensure no parallel edges exist
    std::set<std::pair<leda::node, leda::node> > temp;
    forall_edges(e, G) {
        v = G.source(e);
        u = G.target(e);

        G.del_edge(e);
        if (u == v) continue; // Omit loops

        temp.insert(std::make_pair(v, u));
    }
    // Add all edges found (except loops) with their anti-parallel edges
    for(auto it = temp.begin(); it != temp.end(); it++) {
        G.new_edge(it->first, it->second);
        G.new_edge(it->second, it->first);
    }
}

// An awful way to get a random, connected graph
void random_connected_graph(leda::graph& G) {
    leda::list<leda::node> visitedNodes;
    do {
        // void random_graph(graph& G, int n, int m, bool no_anti_parallel_edges, bool loopfree, bool no_parallel_edges)
        leda::random_graph(G, NUM_OF_NODES, NUM_OF_EDGES, true, true, true);
        leda::node s = G.first_node();
        leda::node_array<int> dist(G, -1);
        leda::node_array<leda::edge> pred(G);
        visitedNodes = leda::BFS(G, s, dist, pred);
    } while (visitedNodes.size() != NUM_OF_NODES);
}

void printLedaList(leda::list<leda::node> list, std::string initialMessage) {
    std::cout << initialMessage;
    for (auto it = list.begin(); it != list.end(); it++) {
        std::cout << (*it)->id() << ", ";
    }
    std::cout << std::endl;
}

bool areListsIdentical(leda::list<leda::node> l1, leda::list<leda::node> l2) {
    auto it1 = l1.begin();
    auto it2 = l2.begin();

    if(l1.size() != l2.size()) return false;
    
    while (it1 != l1.end() && it2 != l2.end()) {
        if ((*it1)->id() != (*it2)->id()) return false;
        it1++; it2++;
    }
    return true;
}