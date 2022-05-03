#pragma once
#include "utils.h"

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


void nestedSquares_graph(leda::graph& G, int n) {
    G.make_undirected();
    leda::node x;
    leda::queue<leda::node> Q;
    leda::node arr[4];
    for (int i=0; i < n/4; i++) {
        for (int j=0; j<4; j++) {
            arr[j] = G.new_node();
            Q.append(arr[j]);
            if (j == 0) {
                continue;
            }
            else if (j == 3) {
                G.new_edge(arr[3], arr[0]);
            }
            G.new_edge(arr[j], arr[j-1]);
        }
        if (Q.size() - 4 == 0) continue;
        for (int j=0; j<4; j++) {
            G.new_edge(arr[j], Q.pop());
        }
    }
}


void ring_graph(leda::graph& G, int n) {
    G.make_undirected();
    leda::node first, prev, temp;
    first = G.new_node();
    prev = first;
    for(int i=1; i<n; i++) {
        temp = G.new_node();
        G.new_edge(prev, temp);
        prev = temp;
    }
    G.new_edge(prev, first);
}


void fourLevel_graph(leda::graph& G, int n) {
    G.make_undirected();
    int k = n/4;

    int i, j, randIndex;
    leda::node arr[4][k];

    // Initialize random seed
    srand (time(NULL));

    // Create four sets of k nodes each
    for(i=0; i<4; i++) {
        for(j=0; j<k; j++) {
            arr[i][j] = G.new_node();
        }
    }
    for(i=0; i<3; i++) {
        // Add the first k edges (v^i_j -> v^(i+1)_j)
        for(j=0; j<k; j++) {
            G.new_edge(arr[i][j], arr[i+1][j]);
        }

        // At each level (i) select one of the k nodes (v) and add edges (v, u^(i+1)_j for all j)
        randIndex = rand()%k;
        for(j=0; j<k; j++) {
            if(randIndex == j) continue; // I have already placed this edge
            G.new_edge(arr[i][randIndex], arr[i+1][j]);
        }
    }
    
    // Add two extra edges (s,t) and (v,u), where s in L1, t in L3, v in L2, u in L4
    G.new_edge(arr[0][rand()%k], arr[2][rand()%k]);
    G.new_edge(arr[1][rand()%k], arr[3][rand()%k]);
}


// Order: https://en.wikipedia.org/wiki/Generalizations_of_Fibonacci_numbers#:~:text=in%20the%20OEIS)-,Fibonacci%20numbers%20of%20higher%20order,-%5Bedit%5D
// argument n -> Counting from 0, find the nth fibonacci (of order order) number.
// It is passed by argument so I may return the number of nodes the graph will have.
// Paper: https://dmtcs.episciences.org/2165/pdf
void generalizedFibonacciCube_graph(leda::graph& G, int order, int& n) {
    G.make_undirected();
    // Find the number of nodes required
    int nNodes = generalFib(order, n);
    n = nNodes;
    if (nNodes == 0) return;
    // Number of bits required
    const int nbits = std::ceil(std::log2(nNodes));
    int i;

    std::string binStr;
    std::string fibStr = std::string(order, '1');

    leda::node v, u;
    for (i = 0; i < nNodes; i++) {
        v = G.new_node();
    }
    i = -1;
    leda::node_array<int> bin(G);
    forall_nodes(v, G) {
        do {
            binStr = std::bitset<NBITS>(++i).to_string();
        } while (binStr.find(fibStr) != std::string::npos);
        bin[v] = i;
        // std::cout << std::bitset<10>(i).to_string() << std::endl;
    }
    forall_nodes(v, G) {
        forall_nodes(u, G) {
            if(u==v || countDiffBits(bin[u], bin[v]) > 1) continue;
            G.new_edge(v,u);
        }
    }
}