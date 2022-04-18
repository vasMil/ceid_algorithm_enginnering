#pragma once
#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include <iostream>
#include <set>

void printGraph(const leda::graph& G) {
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
