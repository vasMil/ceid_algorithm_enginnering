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
    std::vector<int> fib_n = {10, 12, 15};
    int fibOrder = 3;
    Durations d;

    // Init function pointer union
    graphGeneratorsUnion gg;

    std::fstream outfile;
    outfile.open("measurements.csv", std::fstream::in | std::fstream::out | std::fstream::app);
    if(!outfile.is_open()) {
        std::cout << "Cannot open measurnaments.csv!" << std::endl;
        return 0;
    }
    for(int i=0; i<3; i++) {
        gg.simpleSig = &nestedSquares_graph;
        d = timeMe(gg, G, nested_n[i]);
        d.save_into_csv(outfile, "nestedSquaresGraph");
        d.print("nestedSquaresGraph");

        gg.simpleSig = &ring_graph;
        d = timeMe(gg, G, ring_n[i]);
        d.save_into_csv(outfile, "ringGraph");
        d.print("ringGraph");

        gg.simpleSig = &fourLevel_graph;
        d = timeMe(gg, G, levels_n[i]);
        d.save_into_csv(outfile, "fourLevelsGraph");
        d.print("fourLevelsGraph");
    }
    gg.orderSig = &generalizedFibonacciCube_graph;
    for(int i=0; i<fib_n.size(); i++) {
        d = timeMe(gg, G, fib_n[i], fibOrder);
        d.save_into_csv(outfile, "generalizedFibonacciCubeGraph");
        d.print("generalizedFibonacciCubeGraph");
    }
    outfile.close();
}