#include "utils.h"
#include "my_algorithms.h"
#include "testbench.h"


int main(int argc, char *argv[]) {
    // bool x, y;
    // leda::node v;
    leda::list<leda::node> V1;
    leda::list<leda::node> V2;
    // Initialize a random graph G
    leda::graph G;
    // random_connected_graph(G);
    // test_graph(G);
    // nestedSquares_graph(G, 12000);
    // ring_graph(G, 21);

    // fourLevel_graph(G, 4);
    // printGraph(G, "csacademy");
    // std::cout << "Print is done!" << std::endl;
    // x = my_bipar_checker(G, myV1, myV2);

    // // Make graph undirected
    // addComplementaryEdges(G);
    // y = leda::Is_Bipartite(G, V1, V2);

    // // Output status
    // if (x == y && areListsIdentical(V1, myV1) && areListsIdentical(V2, myV2)){
    //     std::cout << "Leda and my implementation returned the same two lists" << std::endl;
    // }
    // else if (x != y) {
    //     std::cout << "Leda returned with " << y << std::endl << "My implementation returned with " << x << std::endl;
    // }
    // else {
    //     std::cout << "Lists returned do not match!" << std::endl;
    //     printLedaList(V1, "V1: ");
    //     printLedaList(V2, "V2: ");
    //     printLedaList(myV1, "myV1: ");
    //     printLedaList(myV2, "myV2: ");
    // }

    timeMe(&ring_graph, G, 10, &my_bipar_checker, V1, V2);
}