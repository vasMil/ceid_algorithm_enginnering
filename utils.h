#pragma once
#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/graph/basic_graph_alg.h"
#include "LEDA/core/queue.h"

#include <iostream>
#include <string>
#include <set>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <math.h>
#include <bitset>

#define NUM_OF_NODES 10 /* Info required for random_connected_graph() */
#define NUM_OF_EDGES 12 /* Info required for random_connected_graph() */
#define GREEN "green"
#define BLUE "blue"

#define NBITS 20        /* number of bits in fibonacci node */


void printGraph(const leda::graph& G, std::string site = "graphviz") {
    std::cout << "Printing Graph..." << std::endl;
    std::string sep;
    site == "graphviz" ? sep = " -> " : sep = " ";
    leda::edge e;
    leda::node s, t;
    if(site == "csacademy") {
        forall_nodes(s, G) {
            std::cout << s->id() << std::endl;
        }
    } 
    forall_edges(e, G) {
        s = G.source(e);
        t = G.target(e);
        std::cout << s->id() << sep << t->id() << std::endl;
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

void printLedaList(leda::list<leda::node> list, std::string initialMessage) {
    std::cout << initialMessage;
    for (auto it = list.begin(); it != list.end(); it++) {
        std::cout << (*it)->id() << ", ";
    }
    std::cout << std::endl;
}

bool areListsIdentical(leda::list<leda::node> l1, leda::list<leda::node> l2) {
    // The difference in the succession is due to the way I am iterating through the edges in my_bipar_checker.
    // This may also result to a different odd circle to be returned.
    l1.sort();
    l2.sort();
    auto it1 = l1.begin();
    auto it2 = l2.begin();

    if(l1.size() != l2.size()) return false;
    
    while (it1 != l1.end() && it2 != l2.end()) {
        if ((*it1)->id() != (*it2)->id()) return false;
        it1++; it2++;
    }
    return true;
}

// Given the order and the index n, return nth integer of the generalized(order) fibonacci sequence.
// (indexing starting at 0) 
int generalFib(int order, int n) {
    std::vector<int> fibSeq (n+1, 0);
    int i, j, h;
    fibSeq[order-1] = 1;
    for(i = order; i <= n+1; i++) {
        h = i - order;
        for (j=0; j<order; j++) {
            fibSeq[i] += fibSeq[h+j];
        }
    }
    return fibSeq[n];
}

// Given two bitsets, return the number of different bits.
int countDiffBits(std::bitset<NBITS> a, std::bitset<NBITS> b) {
    int count = 0;
    for (int i=0; i<a.size(); i++) {
        if(a[i] != b[i]) count++;
    }
    return count;
}