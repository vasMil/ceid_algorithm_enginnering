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
    LowerBoundPMap lb = boost::get(&NodeInfo::lowerBound, G);

    // Create the random graph
    randomGraph(G, cost, 1000, 5000, 0, 100);
    // // Print it
    // boost::write_graphviz(std::cout, G, boost::default_writer(), boost::make_label_writer(cost));
    // std::cout << std::endl << std::endl;
    
    // Get the starting vertex s and the target vertex t
    VertexIter first, last;
    boost::tie(first, last) = boost::vertices(G);
    Vertex s = *first, t = *(--last);
    // Init counter to 0 for Dijkstra
    int cnt = 0;
    bool isPath;

    // Run Dijkstra
    std::cout << "Dijkstra_SP Running..." << std::endl;
    isPath = Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    std::cout << "Dijkstra_SP Done..." << std::endl << std::endl;

    // Print the path and the total cost
    std::cout << "Printing Dijkstra_SP results..." << std::endl << std::endl;
    printPath(G, s, t, pred, dist, isPath, cnt);
    std::cout << "Printing Dijkstra_SP results Done..." << std::endl << std::endl;

    // Preprocess costs for A*
    std::cout << "A* preprocessing Running..." << std::endl;
    prep_A_star(G, t);
    std::cout << "A* preprocessing Done..." << std::endl;

    // Clear counter
    cnt = 0;

    // Run A*
    std::cout << "A* Running..." << std::endl;
    cost = boost::get(&EdgeInfo::cost, G);
    isPath = Dijkstra_SP(G, s, t, cost, pred, dist, cnt, GraphOper::getInstance());
    std::cout << "A* Done..." << std::endl << std::endl;

    // Fix dist
    dist[t] = dist[t] + lb[s];

    // Print the path and the total cost
    std::cout << "Printing A* results..." << std::endl << std::endl;
    printPath(G, s, t, pred, dist, isPath, cnt);
    std::cout << "Printing A* results Done..." << std::endl << std::endl;

    // // Test Dijkstra_SP
    // test_Dijkstra(1000, 2000);
    // test_printSmallGraph_DijkstraSP(10,25);
    return 0;
}