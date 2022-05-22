#include "utils.h"
#include "graphGenerators.h"
#include "algorithms.h"
#include "test.h"
#include "testbench.h"

int main() {
    std::fstream outfile;
    outfile.open("measurements.csv", std::fstream::in | std::fstream::out | std::fstream::app);
    if(!outfile.is_open()) {
        std::cout << "Cannot open measurnaments.csv!" << std::endl;
        return 0;
    }
    
    timeMe(outfile, "random", &randomGraph, 1000, 2000, 1, 100);
    // // Test Dijkstra_SP
    // test_Dijkstra(1000, 2000);
    // test_printSmallGraph_DijkstraSP(10,25);
    // test_A_star(1000, 5000);
    return 0;
}