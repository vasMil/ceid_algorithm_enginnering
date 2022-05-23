#include "utils.h"
#include "graphGenerators.h"
#include "algorithms.h"
#include "test.h"
#include "testbench.h"

int main() {
    std::vector<std::pair<int, int> > gridGraphVec = {{30, 1000}, {60, 1000}, {80, 1000}};
    std::vector<std::pair<int, int> > randomGraphVec = {{10000, 20000}, {20000, 40000}, {60000, 120000}};
    std::vector<std::pair<int, int> > costBounds = {{1, 100}, {1, 10000}};
    std::fstream outfile;
    outfile.open("measurements.csv", std::fstream::in | std::fstream::out | std::fstream::app);
    if(!outfile.is_open()) {
        std::cout << "Cannot open measurnaments.csv!" << std::endl;
        return 0;
    }
    for (int i=0; i<costBounds.size(); i++) {
        for (int j=0; j < gridGraphVec.size(); j++) {
            std::cout << "timeMe on random graph with " << randomGraphVec[j].first << " vertices and " 
                << randomGraphVec[j].second << " edges | Cost in [" << costBounds[i].first << ", " << costBounds[i].second << "]" << std::endl;
            timeMe(outfile, "random", &randomGraph, randomGraphVec[j].first, randomGraphVec[j].second, costBounds[i].first, costBounds[i].second);
            std::cout << "timeMe on grid graph with " << gridGraphVec[j].first << " vertices and " 
                << gridGraphVec[j].second << " edges | Cost in [" << costBounds[i].first << ", " << costBounds[i].second << "]" << std::endl;
            timeMe(outfile, "grid", &randomGraph, gridGraphVec[j].first, gridGraphVec[j].second, costBounds[i].first, costBounds[i].second);
        }
    }
    // // Test Dijkstra_SP
    // test_Dijkstra(1000, 2000);
    // test_printSmallGraph_DijkstraSP(10,25);
    // test_A_star(10, 20);
    outfile.close();
    return 0;
}