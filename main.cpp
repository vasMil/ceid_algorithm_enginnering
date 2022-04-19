#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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
            // Traverse BFS tree backwards, until you find a common ancestor
            // This will always return an odd circle. (I think I can prove this by induction)
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

void fourLevel_graph(leda::graph& G, int k) {
    G.make_undirected();
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

int main(int argc, char *argv[]) {
    bool x, y;
    leda::node v;
    leda::list<leda::node> myV1, V1;
    leda::list<leda::node> myV2, V2;
    // Initialize a random graph G
    leda::graph G;
    // random_connected_graph(G);
    // test_graph(G);
    // nestedSquares_graph(G, 12000);
    // ring_graph(G, 21);
    fourLevel_graph(G, 4);
    printGraph(G, "csacademy");
    std::cout << "Print is done!" << std::endl;
    x = my_bipar_checker(G, myV1, myV2);

    // Make graph undirected
    addComplementaryEdges(G);
    y = leda::Is_Bipartite(G, V1, V2);

    // Output status
    if (x == y && areListsIdentical(V1, myV1) && areListsIdentical(V2, myV2)){
        std::cout << "Leda and my implementation returned the same two lists" << std::endl;
    }
    else if (x != y) {
        std::cout << "Leda returned with " << y << std::endl << "My implementation returned with " << x << std::endl;
    }
    else {
        std::cout << "Lists returned do not match!" << std::endl;
        printLedaList(V1, "V1: ");
        printLedaList(V2, "V2: ");
        printLedaList(myV1, "myV1: ");
        printLedaList(myV2, "myV2: ");
    }
}