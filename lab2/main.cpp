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
    randomGraph(G, cost, 10, 20, 0, 100);
    VertexIter first, last;
    boost::tie(first, last) = boost::vertices(G);
    Vertex L = *first;
    prep_A_star(G);
    std::cout << std::endl << std::endl;
    boost::write_graphviz(std::cout, G, boost::default_writer(), boost::make_label_writer(cost));
    // // Test Dijkstra_SP
    // test_Dijkstra(1000, 2000);
    // test_printSmallGraph_DijkstraSP(10,25);
    return 0;
}