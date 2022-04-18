#pragma once
#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include <iostream>

void printGraph(const leda::graph& G) {
    leda::edge e;
    leda::node s, t;
    forall_edges(e, G) {
        s = G.source(e);
        t = G.target(e);
        std::cout << s->id() << " -> " << t->id() << std::endl;
    }
}
