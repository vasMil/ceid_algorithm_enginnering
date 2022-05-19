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

    // Print the graph
    // write_graphviz(std::cout, G, boost::default_writer(), boost::make_label_writer(cost));
    
    // Test Dijkstra_SP
    // Test by making use of test.h to compare my implementation with the one boost offers
    test_Dijkstra(1000, 2000);
    
    // Test by printing the SP from first Vertex to last Vertex of the graph
    int cnt = 0;
    VertexIter first, last;
    boost::tie(first, last) = boost::vertices(G);
    Vertex s = *first, t = *(--last);
    
    bool isPath = Dijkstra_SP(G, s, t, cost, pred, cnt);

    if (!isPath) {
        std::cout << "There is no path from s to t" << std::endl;
        return 0;
    }
    std::cout << "Visited " << cnt << " vertices" << std::endl;
    std::cout << "The path: ";
    while (s != t) {
        std::cout << t << " -> ";
        t = boost::source(pred[t], G);
    }
    std::cout << s << std::endl;
    return 0;
}