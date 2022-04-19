#include "utils.h"
#include "graph_generators.h"
#include "my_algorithms.h"
#include "testbench.h"


int main(int argc, char *argv[]) {
    // Initialize a random graph G
    leda::graph G;
    int nested_n[] = {12000, 48000, 100000};
    int ring_n[] = {20001, 50001, 100001};
    int levels_n[] = {4*600, 4*1200, 4*1800};
    Durations d;
    // random_connected_graph(G);
    // test_graph(G);
    // nestedSquares_graph(G, 12000);
    // ring_graph(G, 21);
    // fourLevel_graph(G, 16);

    for(int i=0; i<3; i++) {        
        d = timeMe(&nestedSquares_graph, G, nested_n[i]);
        d.print("Nested Squares Graph");
        d = timeMe(&ring_graph, G, ring_n[i]);
        d.print("Ring Graph");
        d = timeMe(&fourLevel_graph, G, levels_n[i]);
        d.print("Four Levels Graph");
    }
}