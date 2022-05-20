#include "utils.h"
#include "graphGenerators.h"
#include "algorithms.h"
#include "test.h"

int main() {
    // Declaire graph G
    Graph G;
    // Get the property maps
    CostPMap cost = boost::get(&EdgeInfo::cost, G);
    PredPMap pred = boost::get(&NodeInfo::pred, G);
    DistPMap dist = boost::get(&NodeInfo::dist, G);

    // Create the random graph
    randomGraph(G, cost, 80, 150, 0, 1000);
    
    // Test Dijkstra_SP
    // Test by making use of test.h to compare my implementation with the one boost offers
    // test_Dijkstra(1000, 2000);
    
    // Test by printing the SP from first Vertex to last Vertex of the graph
    test_printSmallGraph_DijkstraSP(10,25);
    return 0;
}